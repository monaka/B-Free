/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
**
**
*/

#include <stdio.h>

char table[] = 
{
  'a', 'b', 'L'
};

char code[] = L"これはＪＩＳのコードです";
char code2[] = "これはＪＩＳのコードです";

void
main ()
{
  char *p;

  printf ("This is JIS code.\n");
  for (p = code; *p != '\0'; p++)
    {
      printf ("<%x>", *p);
    }
  putchar ('\n');
  printf ("%s\n", string_tron_to_shift (code));
  printf ("%s\n", string_tron_to_shift (string_shift_to_tron (code2)));
}
