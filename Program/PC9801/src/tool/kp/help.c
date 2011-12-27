/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Source: /cvsroot/bfree-info/B-Free/Program/PC9801/src/tool/kp/help.c,v $
 *
 * $Date: 2011/12/27 17:13:35 $
 * $Author: liu1 $
 *
 *
 * description:
 *
 */

static char	version_id[] = "$Source: /cvsroot/bfree-info/B-Free/Program/PC9801/src/tool/kp/help.c,v $ $Revision: 1.1 $";

#include "kp.h"

void
short_help ()
{
  printf ("kp -- TRON kanji preprocessor version %s.%s\n",
	  major_version,
	  minor_version);
  printf ("usagse: kp [options] < [input-file] > [output-file]\n");
  printf ("more help: exec 'kp -help'\n");
}

void
long_help (void)
{
  printf ("kp -- TRON kanji preprocessor version %s.%s\n",
	  major_version,
	  minor_version);
  printf ("usagse: kp [options] < [input-file] > [output-file]\n\n");

  printf ("option list.\n");
  printf ("-help, -h:\t\tlong help.\n");
  printf ("-input-code, -ic:\tinput languge code (default is Shift JIS)\n");
  printf ("-input, -i:\t\tinput file (default standard input)\n");
  printf ("-output, -o:\t\toutput file (default standard output)\n");
}
