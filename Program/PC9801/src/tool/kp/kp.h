/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Source: /cvsroot/bfree-info/B-Free/Program/PC9801/src/tool/kp/kp.h,v $
 *
 * $Date: 2011/12/27 17:13:35 $
 * $Author: liu1 $
 *
 *
 * description:
 *
 */

#ifndef __KP_H__
#define __KP_H__	1

#include <stdio.h>
#include <strings.h>

#define OK	1
#define NG	0

#define MAX_LINE	100

extern char	major_version[];
extern char	minor_version[];
extern char	*input_language;
extern char	*language_table[];

extern int	check_language (char *lang);

extern void	short_help (void);
extern void	long_help (void);

extern void	convert_string (FILE *in, FILE *out);
extern void	convert_char (FILE *in, FILE *out);
extern void	output_char (FILE *out, int ch);

#endif __KP_H__



