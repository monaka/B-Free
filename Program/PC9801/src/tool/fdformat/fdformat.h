/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* fdformat.h 
 */

#include <stdio.h>

typedef unsigned char	B;

struct fd_header
{
  B	boot[512];		/* BOOT �ΰ� */
  
};

