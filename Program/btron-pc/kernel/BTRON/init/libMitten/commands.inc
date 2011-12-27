/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* $Id: commands.inc,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: commands.inc,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.18  2000-07-31 18:37:52  kishida0
 * add debug I/F
 *
 * Revision 1.17  2000/01/23 15:53:34  kishida0
 * キーボード切替え用コマンドとグラフィック画面スクロール用のコマンド(test)を
 * 追加した
 * いまいち自信なし
 *
 * Revision 1.16  1999/11/10 10:25:30  naniwa
 * to support execve
 *
 * Revision 1.15  1999/07/30 08:20:59  naniwa
 * add cfork
 *
 * Revision 1.14  1999/07/24 04:35:54  naniwa
 * add pmemstat
 *
 * Revision 1.13  1999/07/09 08:26:14  naniwa
 * add task-list
 *
 * Revision 1.12  1999/03/15 08:41:26  monaka
 * Minor fix. operator 'posix_close' renamed to 'posix-close'
 *
 * Revision 1.11  1998/12/19 03:38:33  monaka
 * Functions for dosfs has commented out
 *
 * Revision 1.10  1998/12/18 02:29:04  monaka
 * Major modify. The stack data structure was re-born. It was no change for adding new operators.
 *
 * Revision 1.9  1998/12/12 03:14:03  monaka
 * Many operators (especially graphics) were changed.
 *
 * Revision 1.8  1998/12/10 15:04:06  night
 * Delete MITN_OPERATOR("test", test_print, 0).
 *
 * Revision 1.7  1998/12/08 08:10:25  monaka
 * Include files changed.
 *
 * Revision 1.6  1998/12/08 05:38:50  monaka
 * I init has been included the Mitten Scripting Language since this release.
 * But, it's just a first preview. So you can't use many convenience
 * features. Mitten works extension scripting environment like Tcl, Guile,
 * Windows Scripting Host, and so on. If you wished, you may connect your
 * function written in C to Mitten operator. If you wished, you may call
 * Mitten from C.
 *
 * Revision 1.5  1998/12/02 09:39:33  monaka
 * Some option parameters collected.
 *
 * Revision 1.4  1998/12/02 01:03:45  monaka
 * Major modified for implementing scripting language.
 * But there are a partly connection to user interface.
 *
 * Revision 1.3  1998/11/26 21:32:42  monaka
 * init/graphic.c has moved to init/libMittenCmd/graphic.c and modified for command 'draw-window'.
 *
 * Revision 1.2  1998/11/26 15:24:12  monaka
 *
 * Attributes for built-in commands added.
 * It indicates function call type, the number of argument, and so on.
 *
 * Revision 1.1  1998/11/26 13:53:14  monaka
 *
 * init/command.c has split and moved to some subdirs.
 * This means it was changed how to add a built-in function.
 *
 */
/* Mitten operators */
MITN_OPERATOR("pop", pop, 1)
MITN_OPERATOR("exch", exch, 2)
MITN_OPERATOR("dup", dup, 1)
MITN_OPERATOR("copy", copy, 1)
MITN_OPERATOR("roll", roll, 2)
MITN_OPERATOR("clear", clear, 0)
MITN_OPERATOR("count", count, 0)
MITN_OPERATOR("mark", mark, 0)
MITN_OPERATOR("cleartomark", cleartomark, 0)
MITN_OPERATOR("counttomark", counttomark, 0)

MITN_OPERATOR("neg",  MitnMath_Neg, 1)
MITN_OPERATOR("add",  MitnMath_Add, 2)
MITN_OPERATOR("sub",  MitnMath_Sub, 2)
MITN_OPERATOR("mul",  MitnMath_Mul, 2)
MITN_OPERATOR("idiv", MitnMath_Div, 2)

MITN_OPERATOR("help", help, 0)
MITN_OPERATOR("==",  PopAndPrint,  1)
MITN_OPERATOR("stack-status", MitnStack_Status, 0)
TRAD_OPERATOR("echo", echo, 1)
#ifndef _DEBUG_ON_UNIX
MITN_OPERATOR("gc", MitnStack_GarbageCollect, 0)
#endif

/* DOS filesystem. */
/*TRAD_OPERATOR("dattach",dattach, MITN_OP_ARGS_UNKNOWN)*/
/*TRAD_OPERATOR("dls", dos_ls, 1)*/
/*TRAD_OPERATOR("dcat", dos_cat, 1)*/
/*TRAD_OPERATOR("root", root_cat, 1)*/

TRAD_OPERATOR("openfd", open_fd, MITN_OP_ARGS_UNKNOWN)
TRAD_OPERATOR("readfd", read_fd, MITN_OP_ARGS_UNKNOWN)
TRAD_OPERATOR("writefd", write_fd, MITN_OP_ARGS_UNKNOWN)
MITN_OPERATOR("reset", reset, 1)
MITN_OPERATOR("falldown", falldown, 1)
MITN_OPERATOR("vget", vget_region, 0)
MITN_OPERATOR("task-list", print_task_list, 0)
MITN_OPERATOR("pmemstat", pmemstat, 0)
MITN_OPERATOR("cexec", cexec, 1)

  /* POSIX 関係 */
TRAD_OPERATOR("pcat", posix_cat, 2)
MITN_OPERATOR("pdir", posix_dir, 1)
TRAD_OPERATOR("pdump", posix_dump, 1)
TRAD_OPERATOR("pexec", posix_exec_test, 1)
MITN_OPERATOR("pfork", posix_fork_test, 0)
TRAD_OPERATOR("pmount", posix_mount, 2)
MITN_OPERATOR("pnoaction", posix_noaction, 0)
TRAD_OPERATOR("posix", posix, 1)
TRAD_OPERATOR("pvmdump", posix_vm_dump, 1)
MITN_OPERATOR("posix-open", p_open, 2)
MITN_OPERATOR("posix-write", p_write, 2)
MITN_OPERATOR("posix-close", p_close, 1)
TRAD_OPERATOR("pwrite", posix_writefile, 2)

  /* グラフィック関係 */
MITN_OPERATOR("graphic-mode", graphic_mode, 0)
MITN_OPERATOR("text-mode", text_mode, 0)
MITN_OPERATOR("gsave", gsave, 0)
MITN_OPERATOR("translate", translate, 2)
MITN_OPERATOR("moveto", moveto, 2)
MITN_OPERATOR("rmoveto", rmoveto, 2)
MITN_OPERATOR("setcolor", setcolor, 1)
MITN_OPERATOR("setmode", setmode, 1)
MITN_OPERATOR("box", box, 2)
MITN_OPERATOR("fillbox", fillbox, 2)
MITN_OPERATOR("lineto", lineto, 2)
MITN_OPERATOR("rlineto", rlineto, 2)
#if 0
TRAD_OPERATOR("graph", graphic, MITN_OP_ARGS_UNKNOWN)
#endif
MITN_OPERATOR("draw-window", draw_window, 4)
MITN_OPERATOR("scroll", scroll, 0);

  /* lowlib 関係 */
MITN_OPERATOR("lowlib", lowlib_stat, 0)
TRAD_OPERATOR("load", lowlib_load, 1)

  /* IDE/HD 関係 */
MITN_OPERATOR("idestat", ide_stat, 0)
MITN_OPERATOR("idegeo", ide_geometric, 0)
TRAD_OPERATOR("ideread", ide_read, 1)

  /* Ethernet ドライバ関係 */
TRAD_OPERATOR("ether", ether, MITN_OP_ARGS_UNKNOWN)

  /* その他 */
MITN_OPERATOR("malloc", malloc_test, 0)
MITN_OPERATOR("rootinfo", root_info, 0)

MITN_OPERATOR("k106jp", k106jp, 0)
MITN_OPERATOR("k101us", k101us, 0)

MITN_OPERATOR("pmemdump", pmemdump, 1)

#if 0
#endif
