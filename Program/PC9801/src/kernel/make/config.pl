#!/usr/bin/perl

$count = 1;
$paddr = 0x10000;

sub exec_size {
    local ($fname) = @_;

    open (EXECFILE, "./getsize $fname |");
    $size = <EXECFILE>;
    return ($size);
}


sub exec_entry {
    local ($fname) = @_;

    open (EXECFILE, "./getentry $fname |");
    $entry = <EXECFILE>;
    chop ($entry);
    return ($entry);
}

sub roundup {
    local ($dest, $align) = @_;

    printf STDERR "\$dest = $dest, \$align = $align\n";
    printf STDERR "($dest + ($align - 1)) / $align) * $align = %d\n", 
                   int(($dest + ($align - 1)) / $align) * $align;
    return ( int(($dest + ($align - 1)) / $align) * $align);
}


# 
# 入力
#
while ($line = <STDIN>) {
    $_ = $line;
    if (/^#/) {
	;
    } elsif (($fname[$count],$vaddr[$count],$paddr[$count],$type[$count],$name[$count]) = /(\S*)\s*(\S*)\s*(\S*)\s*(\S*)\s*(".*")/) {
	$count += 1;
    }
}

#
# 出力
#
printf "#include \"../../boot/2nd/boot.h\"\n\n";
printf "int nmodule = %d;\n\n\n", $count - 1;
printf "struct module_info module_table[] =\n";
printf "{\n";

$i = 1;
while ($i < $count) {
    printf "\t{%d, ",            &roundup ((-s $fname[$i]), 1024);
    printf "%d, ",              &exec_size ($fname[$i]);
    printf "%s, %s, ",        $vaddr[$i], $paddr[$i];
    printf "%s, $type[$i], ", &exec_entry ($fname[$i]);
    printf "%s},\n", $name[$i];
#    $paddr += &exec_size ($fname[$i]);
    $i += 1;
}

printf "\t0, 0, 0, 0, 0, 0\n};\n\n\n";

printf "char *module_file[] = \n{\n";
$i = 1;
while ($i < $count) {
    printf "\t\"$fname[$i]\", \n";
    $i += 1;
}
printf "\t0\n};\n";

0;
