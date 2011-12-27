#!/usr/bin/perl
#
#


sub parse_entry
{
    local ($line) = @_;
    
    $line =~ /\s*(\S*)\s*(\S*)\s*(\S*)\s*(\S*)\s*\((.*)\).*$/;
    ($1, $2, $3, $4, $5);
}


sub construct_func
{
    local ($file, $function, $arg, $ret) = @_;

     if ($file =~ /\+.*/)
     {
 	open (FP, ">>$file") || die "Cannot file open\n";
     }
     else
     {
 
 	open (FP, ">$file") || die "Cannot file open\n";
 
 	printf (FP "/*\n");
 	printf (FP "\n");
 	printf (FP "B-Free Project ‚Ì¶¬•¨‚Í GNU Generic PUBLIC LICENSE ‚É]‚¢‚Ü‚·B\n");
 	printf (FP "\n");
 	printf (FP "GNU GENERAL PUBLIC LICENSE\n");
 	printf (FP "Version 2, June 1991\n");
 	printf (FP "\n");
 	printf (FP "(C) B-Free Project.\n");
 	printf (FP "\n");
 	printf (FP "*/\n");
 	printf (FP "/* POSIX Library misc function.\n");
 	printf (FP "*/\n");
 	printf (FP "\n");
 
	printf (FP "/* @(#)\$Header\$  */\n");
	printf (FP "static char rcsid[] = \"\@(#)\$Id\$\";\n");

	printf (FP "\n\n#include \"../native.h\"\n");
    }
	

#    open (FP, ">> $file") || die "Cannot open file.\n";

#    printf (FP "\n\n\n");
    printf (FP "\n\n");
#    printf (FP "typedef char *string;\n\n");

    printf (FP "/* %s \n", $function);
    printf (FP " *\n");
    printf (FP " */\n");
    printf (FP "$ret\n");

#    printf (FP "%s (%s)\n", $function, $arg);
    printf (FP "%s (", $function);
    if ($arg =~ /^void/)
    {
	printf (FP "void)\n");
    }
    else
    {
	$c_flag = 0;
	foreach $argent (split (/,/, $arg))
	{
	    $argent =~ /\s*(.*)/;
	    ($type, $name) = split (/ /, $1, 2);
	
	    if ($c_flag == 1)
	    {
		printf (FP ", ");
	    }

	    if ($type =~ /string/)
	    {
		printf (FP "char *%s", $name);
	    }
	    else
	    {
		printf (FP "%s %s", $type, $name);
	    }
	    $c_flag = 1;
	}
	printf (FP ")\n");
    }
    printf (FP "{\n");

    if ($arg =~ /^void/)
    {
	$function =~ tr/[a-z]/[A-Z]/;
	printf (FP "  return (call_lowlib (PSC_%s));\n", $function);
    }
    else
    {
	$function =~ tr/[a-z]/[A-Z]/;
	printf (FP "  return (call_lowlib (PSC_%s", $function);
	foreach $argent (split (/,/, $arg))
	{
	    $argent =~ /\s*(.*)/;
	    ($type, $name) = split (/ /, $1, 2);
	
	    if ($type =~ /string/)
	    {
		printf (FP ", strlen (%s)", $name);
		printf (FP ", %s", $name);
	    }
	    else
	    {
		if ($type =~ /struct/)
		{
		    $name =~ /\s*(\S*)\s*(\S*)/;
		    $name = $2;
		}

		if ($name =~ /\*(.*)/)
		{
		    $name = $1;
		}

		if ($name =~ /\s*(\S*)\[.*\]/)
		{
		    $name = $1;
		}

		printf (FP ", %s", $name);
	    }
	}
	printf (FP "));\n");
    }
    
    printf (FP "}\n\n\n");

    close (FP);
}


#
#
#

printf ("argc = %d\n", $#ARGV + 1);
$genfile=$ARGV[0];
$makefile=$ARGV[1];
#$outdir=$ARGV[1];
#$outfile=$ARGV[1];



printf ("Generate file = %s\n", $genfile);

open (GENFILE, "<$genfile") || die "Cannot open file.\n";
open (MAKEFILE, ">>$makefile") || die "Cannot open file.\n";

printf (MAKEFILE "#\nOBJS=\\\n");

while (<GENFILE>)
{
    chop $_;
    ($fname, $ftype, $ffile, $fretval, $farg) = &parse_entry ($_);
    if ($ftype =~ /Native/)
    {
	printf ("function %s, file %s, type %s, argument = (%s)\n", $fname, $ffile, $ftype, $farg);
	&construct_func ($ffile, $fname, $farg, $fretval);
	printf (MAKEFILE "\t%s\\\n", $ffile);

#	&construct_func ($outfile, $fname, $farg, $fretval);
    }
    else
    {
	printf ("skip %s\n", $fname);
    }

}

close (MAKEFILE);

0;
