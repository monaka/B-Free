#!/usr/bin/perl
#

# ヘッダ部分の除去
#
while (<>)
{
    chop $_;
    if (length ($_) <= 0) {
	last;
    }
}


$_ = <>;
($changed_by) = /^Changed by (.*)$/;

$_ = <>;

$_ = <>; chop $_;
($date) = $_;

$_ = <>;
($directory) = /^.* of \/usr\/local\/src\/master\/B-Free\/(.*)$/;

$_ = <>;
$_ = <>;
$_ = <>;

$file = "";
if (/^Added Files/ || /^Modified Files/) {
    while (<>) {
	chop $_;
	if (/^Log Message/) {
	    last;
	}

	/\s*(.*)$/;
	$file .= $1;
    }
}


# 空白行の削除
while (<>) {
    if (/^\S+/) {
	last;
    }
}

$log = "";
while () {
    if (length ($_) <= 2) {
	last;
    }
    $log = $log . $_;
    $_ = <>;
}


if (length ($file) <= 0) {
    # ディレクトリの追加/削除
    ($parent_dir) = $directory =~ /^(.*)\/\S*/;
    ($file) = $directory =~ /.*\/(\S*)/;
    $log =~ s/\//\/ /g;
    printf ("%s\n%s\n%s\n%s\n%s\n",
	    $date,
	    $changed_by,
	    $parent_dir,
	    $file,
	    $log);
} else {
    printf ("%s\n%s\n%s\n%s\n%s\n",
	    $date,
	    $changed_by,
	    $directory,
	    $file, 
	    $log);
}


0;
