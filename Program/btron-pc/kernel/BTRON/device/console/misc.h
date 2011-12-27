/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: misc.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  2000-07-09 16:46:18  kishida0
 * add misc.c prototype define
 *
 */
#ifndef _CONSOLE_MISC_H_
#define _CONSOLE_MISC_H_
void set_curpos (W x, W y);
void move_curpos (W x, W y);
void goto_cursol (W x, W y);
ER write_char (UB ch);
void clear_console (void);
void clear_rest_line(void);
void clear_rest_screen(void);
#endif // __CONSOLE_MISC_H_
