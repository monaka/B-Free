#!/usr/bin/perl
#
#
#

%month_table = ('Jan', 1, 'Feb', 2, 'Mar', 3, 'Apr', 4, 'May', 5, 'Jun', 6,
		'Jul', 7, 'Aug', 8, 'Sep', 9, 'Oct', 10,'Nov',11,'Dec', 12);


if ( $#ARGV >= 0)
{
    $index_prefix = $ARGV[0];
}
else
{
    $index_prefix = ".";
}

# エントリを読み込む
$index = 0;
while (<STDIN>) {
    $date[$index] = $_;
    $author[$index] = <STDIN>;
    $dir[$index] = <STDIN>;
    $file[$index] = <STDIN>;
    while (<STDIN>) {
	if (length ($_) <= 1) {
	    last;
	}
	$log[$index] = $log[$index] . $_  . '<BR>';
    }
    $index++;
}

# 読み込み終了

#open (DATE, "date|");
#$nowtime = <DATE>;
#chop ($nowtime);
#close (<DATE>);


# 最終変更時刻を取得
($month, $day, $hour, $minutes, $second, $timezone, $year) = $date[$index - 1] =~
    /\S{3}\s(\S*)\s+(\S*)\s+(\S*):(\S*):(\S*)\s+(\S{3})\s(\S{4})/;

print <<EOF;
<HTML>
<HEAD>
    <META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=x-euc-jp">
    <META HTTP-EQUIV="Content-Language" CONTENT="ja">
    <TITLE>Source update information.</TITLE>
</HEAD>

<BODY BGCOLOR=#FFFFFF>
<H1>Source update information $month/$year</H1>
<P>
<P>
EOF


printf ("<P>\n");
printf ("<HR>\n");

open (header, "header.html") || die "cannot open header.html\n";
while (<header>)
{
    if (/<A HREF=\"(.*)\">/)
    {
	printf ("<A HREF=\"%s/%s\">\n", $index_prefix, $1);
    }
    else
    {
	print $_;
    }
}
close (header);

printf ("<HR>\n");

print <<EOF;
<P>
<TABLE BORDER CELLPADDING=2 CELLSPACING=0 WIDTH="100%">
<TR>
    <TD WIDTH=100>when
    <TD WIDTH=150>who
    <TD WIDTH=200>directory
    <TD WIDTH=200>file
    <TD>description
</TR>
EOF



# 逆順で出力
for ($index-- ;$index >= 0; $index--) {
    ($month, $day, $hour, $minutes, $second, $timezone, $year) = $date[$index] =~
	/\S{3}\s(\S*)\s+(\S*)\s+(\S*):(\S*):(\S*)\s+(\S{3})\s(\S{4})/;

    printf ("<TR>\n");
    $dir_string = $dir[$index];
    $dir_string =~ s/\//\/ /g;

    printf ("<TD VALIGN=top>%s/%s/%s<BR>%s:%s:%s %s",
	    $year, $month_table{$month}, $day, $hour, $minutes, $second, $timezone);

    printf ("<TD VALIGN=top>%s",
	    $author[$index]);

    printf ("<TD VALIGN=top ALIGN=left>%s",
	    $dir_string);

    printf ("<TD VALIGN=top>");
    ($file[$index]) = $file[$index] =~ /(.*)\s*$/;
    chop ($dir[$index]);
    $file[$index] =~ s/Removed Files://g;
    $file[$index] =~ s/Added Files://g;
    $file[$index] =~ s/Tag:\s+\d+\.\d+//g;
    for (split / /, $file[$index]) {
	printf ("<A HREF=\"http://www.tron-net.gr.jp/cgi-bin/cvsweb/%s/$_\">$_</A><BR>\n",
		$dir[$index]);
    }
    printf ("<TD VALIGN=top>%s\n", $log[$index]);
    printf ("</TR>\n");
}

print <<EOF;
</TABLE>
</BODY>
</HTML>
EOF
0;
