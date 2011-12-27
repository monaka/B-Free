#!/usr/bin/perl

sub MakeLibCode ($$$@)
{
    my ($dir, $func_name, $description, @flds) = @_;

    if (! -d "$dir0/lib") {
	mkdir "$dir0/lib", 0755;
    }
    $dir = "$dir0/lib/$dir";
    if (! -d $dir) {
	mkdir $dir, 0755;
    }

    $file = "$dir/$func_name.c";
    open FD, ">$file";

    print FD "/*\nB-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。\nGNU GENERAL PUBLIC LICENSE\nVersion 2, June 1991\n\n(C) B-Free Project.\n*/\n\n";
    print FD "#include <sys/syscall_nr.h>\n";
    print FD "#include <types.h>\n\n";
    print FD "/*\n** Function: $func_name\n** Description: $description\n*/\n\n";
    print FD "ER\n$func_name (".join(",", @flds).")\n";
    print FD "{\n\treturn call_btron(BSYS_\U$func_name\E";
    if ($flds[0] ne "void") {
	grep (s/.*\s//, @flds);
	print FD ", ".join(", ", @flds);
    }
    print FD ");\n}\n";

    close FD;
}

sub MakeLowlibCode ($$$@)
{
    my ($dir, $func_name, $description, @flds) = @_;

    if (! -d "$dir0/lowlib") {
	mkdir "$dir0/lowlib", 0755;
    }
    $dir = "$dir0/lowlib/$dir";
    if (! -d $dir) {
	mkdir $dir, 0755;
    }

    $file = "$dir/$func_name.c";
    open FD, ">$file";

    print FD "/*\nB-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。\nGNU GENERAL PUBLIC LICENSE\nVersion 2, June 1991\n\n(C) B-Free Project.\n*/\n\n";
    print FD "#include <sys/syscall_nr.h>\n";
    print FD "#include <types.h>\n\n";
    print FD "/*\n** Function: $func_name\n** Description: $description\n*/\n\n";
    print FD "ER\nbsys_$func_name (VP *argp)\n";
    print FD "{\n";
    print FD "  ER error;\n  struct btron_request req;\n  struct btron_response res;\n\n";
    print FD "  struct bsc_$func_name *args = (struct bsc_$func_name *)argp;\n";
    grep (s/.*\s//, @flds);
    for $i (@flds) {
	print FD "  req.param.par_$func_name.$i = args->$i;\n";
    }

    print FD "\n  error = _make_connection(BSC_\U$func_name\E, &req, &res);\n\n";
    print FD "  if (error != E_OK)\n";
    print FD "  {\n";
    print FD "    /* What should I do? */\n";
    print FD "  }\n\n";
    print FD "  return (res.status);\n";
    print FD "}\n";

    close FD;
}

$magic = <>;
chomp $magic;
die "This is not systemcall define file.\n" if ($magic ne '#Systemcall defines 1.0');

die "Directory ./autogen not found.\n" if (! -d "./autogen" );

$dir0 = "./autogen/include";
if (! -d $dir0) {
    warn "Directory $dir0 is not available...\n";
    mkdir $dir0, 0755;
    warn "Created.\n";
}

open SYSNO_FD, ">$dir0/syscall_nr.h" || die "Can't open $dir0/syscall_nr.h\n";
print SYSNO_FD << '__SYSPKT_HDR__';
#ifndef __BTRON_SYSCALL_NR_H__
#define __BTRON_SYSCALL_NR_H__
/*
B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。
GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.
*/

__SYSNO_HDR__

open SYSPKT_FD, ">$dir0/syscall_packets.h" || die "Can't open $dir0/syscall_packets.h\n";
print SYSPKT_FD << '__SYSPKT_HDR__';
#ifndef __BTRON_SYSCALL_PACKETS_H__
#define __BTRON_SYSCALL_PACKETS_H__
/*
B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。
GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.
*/

__SYSPKT_HDR__

open BTRONPKT_FD, ">$dir0/btron_packet.h" || die "Can't open $dir0/btron_packet.h\n";
print BTRONPKT_FD <<'__BTRONPKT_HDR__';
#ifndef __BTRON_PACKET_H__
#define __BTRON_PACKET_H__
/*
B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。
GNU GENERAL PUBLIC LICENSE\nVersion 2, June 1991\n\n(C) B-Free Project.
*/

struct btron_response
{
  ID	receive_port;		/* 要求受信用のポート	*/
  W	msg_length;		/* メッセージの長さ	*/
  W	operation;		/* 要求番号(システムコールに対応)	*/
  W	errno;			/* エラー番号 */
  W	status;			/* ステータス(リターン値) */

  W	ret1;			/* サブに使用するリターン値 */
  W	ret2;			/* サブに使用するリターン値 */
};

/* BTRON マネージャへの要求メッセージの構造体 */
struct btron_request
{
  ID	receive_port;		/* 要求受信用のポート	*/
  ID	caller;			/* 呼び出し元のタスク ID */
  ID	procid;			/* 呼び出し元のプロセス ID */
  W	msg_length;		/* メッセージの長さ	*/
  W	operation;		/* 要求番号(システムコールに対応)	*/

  union {
__BTRONPKT_HDR__

open SYSFUNCS_FD, ">$dir0/syscall_funcs.h" || die "Can't open $dir0/syscall_funcs.h\n";
print SYSFUNCS_FD << '__SYSFUNCS_HDR__';
#ifndef __BTRON_SYSCALL_FUNCS_H__
#define __BTRON_SYSCALL_FUNCS_H__
/*
B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。
GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.
*/

__SYSFUNCS_HDR__

$count = 1;
while(<>) {
    chomp;
    @flds = split(',');

    $hdr = shift @flds;
    $dir0 = "autogen/$hdr";
    $dir = shift @flds;
    $description = pop(@flds);
    $func_name   = shift(@flds);

    if (! -d $dir0) {
	warn "Directory $dir0 is not available...\n";
	mkdir $dir0, 0755;
	warn "Created.\n";
    }

    MakeLibCode $dir, $func_name, $description, @flds;
    MakeLowlibCode $dir, $func_name, $description, @flds;

    print SYSNO_FD "#define BSC_\U$func_name\E\t\t$count\t /* $description */\n";
    $count++;

    print SYSPKT_FD "/* $func_name */\n";
    print SYSPKT_FD "struct bsc_$func_name {\n";
    for $i (@flds) {
	print SYSPKT_FD "  $i;\n";
    }
    print SYSPKT_FD "};\n\n";

    print BTRONPKT_FD "    struct bsc_$func_name\t\tpar_$func_name;\n";
    print SYSFUNCS_FD "extern W bsc_${func_name}_f (struct btron_request *);\n";
}

print SYSNO_FD "\n#endif /*__BTRON_SYSCALL_NR_H__*/\n";
close SYSNO_FD;

print SYSPKT_FD "\n#endif /*__BTRON_SYSCALL_PACKETS_H__*/\n";
close SYSPKT_FD;

print BTRONPKT_FD "  } param;\n};\n\n#endif /*__BTRON_PACKET_H__*/\n";
close BTRONPKY_FD;

print SYSFUNCS_FD "\n#endif /*__BTRON_SYSCALL_FUNCS_H__*/\n";
close SYSFUNCS_FD;
