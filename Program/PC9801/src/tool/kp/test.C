#include <stdio.h>

void
main ()
{
  char *p = L"漢字コードでの出力＼ｎ";

  while (*p != '\0')
    {
      putchar (*p | 0x80);
      p++;
    }
}
