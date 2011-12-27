/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Source: /cvsroot/bfree-info/B-Free/Program/PC9801/src/tool/kp/top_level.c,v $
 *
 * $Date: 2011/12/27 17:13:35 $
 * $Author: liu1 $
 *
 *
 * description:
 *
 */

static char	version_id[] = "$Source: /cvsroot/bfree-info/B-Free/Program/PC9801/src/tool/kp/top_level.c,v $ $Revision: 1.1 $";

#include "kp.h"

extern shift_to_jis (unsigned short);

/************************************************************************
 *	top_level --- プログラムの本当のmain
 *
 *	引数	input_file_name ---	入力するファイル名
 *		output_file_name ---	出力するファイル名
 *
 *	返り値	なし
 *
 *	処理
 *
 */
void
top_level (char *input_file_name, char *output_file_name)
{
  FILE	*in, *out;
  int	ch;
  char	line[MAX_LINE];

  if (input_file_name == NULL)
    {
      in = stdin;
    }
  else
    {
      in = fopen (input_file_name, "r");
      if (in == NULL)
	{
	  fprintf (stderr, "Can't file open. (%s)\n", input_file_name);
	  exit (0);
	}
    }
  
  if (output_file_name == NULL)
    {
      out = stdout;
    }
  else
    {
      out = fopen (output_file_name, "w");
      if (in == NULL)
	{
	  fprintf (stderr, "Can't file open. (%s)\n", input_file_name);
	  exit (0);
	}
    }

  fprintf (out, "# 1 \"%s\"\n", input_file_name);
  ch = getc (in);
  while (ch != EOF)
    {
      if (ch == 'L')
	{
	  ch = getc (in);
	  if (ch == '"')
	    {
	      convert_string (in, out);
	    }
	  else if (ch == '\'')
	    {
	      convert_char (in, out);
	    }
	  else
	    {
	      putc ('L', out);
	      putc (ch, out);
	    }
	}
      else if (ch == '\'')
	{
	  putc (ch, out);
	  ch = getc (in);
	  while ((ch != '\'') && (ch != '\n'))
	    {
	      putc (ch, out);
	      ch = getc (in);
	    }
	  putc (ch, out);
	}
      else if (ch == '"')
	{
	  putc (ch, out);
	  ch = getc (in);
	  while ((ch != '\"') && (ch != '\n'))
	    {
	      putc (ch, out);
	      ch = getc (in);
	    }
	  putc (ch, out);
	}
      else
	{
	  putc (ch, out);
	}
      ch = getc (in);
    }
}

void
convert_string (FILE *in, FILE *out)
{
  int			ch, ch2;
  unsigned short	output;

  putc ('"', out);
  ch = getc (in);
  while ((ch != '"') && (ch != EOF) && (ch != '\n'))
    {
      if (ch >= 0x80)
	{
	  /* kanji */
	  ch2 = getc (in);
	  if (ch2 == EOF)
	    return;
	  if (strcmp (input_language, "euc") == 0)
	    {
	      output_char (out, ch  & 0x7f);
	      output_char (out, ch2 & 0x7f);
	    }
	  else
	    {
	      output = shift_to_jis ((((ch & 0xff) << 8) | (ch2 & 0xff)));
#ifdef UJIS
	      output_char (out, output >> 8 | 0x80);
	      output_char (out, output & 0xff | 0x80);
#else
	      output_char (out, (output >> 8) & 0xff);
	      output_char (out, output & 0xff);
#endif /* UJIS */	  
	    }
	}
      else
	{
	  putc (ch, out);
	}
      ch = getc (in);
    }
  fprintf (out, "\\0\"");
}

void
convert_char (FILE *in, FILE *out)
{
  int			ch, ch2;
  unsigned short	output;

  ch = getc (in);
  if (ch >= 0x80)
    {
      /* kanji */
      ch2 = getc (in);
      if (strcmp (input_language, "euc") == 0)
	{
	  output_char (out, ch  & 0x7f);
	  output_char (out, ch2 & 0x7f);
	}
      else
	{
	  output = shift_to_jis (((ch << 8) | (ch2 & 0xff)));
#ifdef UJIS
	  output_char (out, output >> 8 | 0x80);
	  output_char (out, output & 0xff | 0x80);
#else
	  output_char (out, output >> 8 & 0xff);
	  output_char (out, output & 0xff);
#endif /* UJIS */
	}
    }
  else
    {
      putc (ch, out);
    }
  ch = getc (in);
  if (ch == '\'')
    fprintf (stderr, "illegal character (0x%x)\n", ch);
}

void
output_char (FILE *out, int ch)
{
  fprintf (out, "\\x%x", ch);
}
