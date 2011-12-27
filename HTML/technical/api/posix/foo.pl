#!/usr/bin/perl


while (<>)
{
    chop $_;
    $name = $_;
    open (FD, ">$_.html") || die "cannot open file $_";

    printf (FD "<HTML>\n");
    printf (FD "<HEAD>\n");
    printf (FD "<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=x-euc-jp\">\n");
    printf (FD "<TITLE>\n");
    printf (FD "B-Free/POSIX system call/$name\n");
    printf (FD "</TITLE>\n");
    printf (FD "</HEAD>\n");
    printf (FD "\n");
    printf (FD "<BODY BGCOLOR=\"#FFFFFF\">\n");
    printf (FD "\n");
    printf (FD "<H1>$name</H1>\n");
    printf (FD "<HR>\n");

    printf (FD "<B>呼び出し形式</B>\n");
    printf (FD "\n");
    printf (FD "<P>\n");
    
    printf (FD "<B>機能説明</B>\n");
    printf (FD "\n");
    printf (FD "<P>\n");
    
    printf (FD "<B>返り値</B>\n");
    printf (FD "\n");
    printf (FD "<P>\n");
    
    printf (FD "<B>エラー番号</B>\n");
    printf (FD "\n");
    printf (FD "<P>\n");
    
    printf (FD "<B>補注</B>\n");
    printf (FD "\n");
    printf (FD "<P>\n");
    
    printf (FD "<B>参照</B>\n");
    printf (FD "\n");
    printf (FD "<P>\n");

    printf (FD "<HR>\n");
    printf (FD "<DIV ALIGN=\"RIGHT\"><A HREF=\"syscall.html\">POSIX システムコール一覧へ戻る</A></DIV>\n");
    printf (FD "<DIV ALIGN=\"RIGHT\"><A HREF=\"syscall.html\">B-Free OS API 一覧へ戻る</A></DIV>\n<P>\n");
    
    printf (FD "<HR>\n");
    printf (FD "<font size=3>Copyright (C) 1996,1997 B-Free Project<BR></font>\n");
    printf (FD "<HR>\n");

    printf (FD "</BODY>\n");
    printf (FD "</HTML>\n");

    close (FD);
}
