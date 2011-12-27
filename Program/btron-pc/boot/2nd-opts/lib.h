#ifndef __LIB_H__
#define __LIB_H__

#include "types.h"

extern void boot_printf (char *fmt, ...);
extern WORD strlen (BPTR s);
extern WORD strnlen (BPTR s, WORD len);
extern WORD strcpy (BPTR s1, BPTR s2);
extern WORD strncpy (BPTR s1, BPTR s2, WORD n);
extern WORD strncpy_with_key (BPTR s1, BPTR s2, WORD n, int key);
extern BPTR strcat (BPTR s1, BPTR s2);
extern BPTR strncat (BPTR s1, BPTR s2, WORD n);
extern ULONG strcmp (BPTR s1, BPTR s2);
extern ULONG strncmp (BPTR s1, BPTR s2, WORD n);
extern char *strchr (char *s, int ch);
extern char *strnchr (char *s, int ch, int n);

extern int toupper (int ch);

extern void write_vram (int x, int y, int ch, int attr);
extern void set_cursor_pos (int x, int y);
extern void scroll_up ();

extern int atoi (char *s);
extern int string_to_number (char *s, int base);

extern void bcopy (char *src, char *dest, int length);
extern void bzero (char *src, int length);

#endif



