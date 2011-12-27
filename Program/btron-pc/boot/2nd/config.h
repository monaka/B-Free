/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/**************************************************************************
 *
 *
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__	1

#include "h/config_boot.h"

#define MAJOR_VER	0
#define MINOR_VER	2

#define MAX_PAGEENT	(1024 * 5)	/* ���� 16 M ʬ�Υڡ�������ȥ� */
#define MAX_DIRENT	1024

#define BASE_MEM	(640 * 1024)	

#define MALLOC_SIZE	(1024 * 100)	/* 100K �Х��Ȥ� malloc �Ѥ˻� */
					/* �� */

#define BOOT_PATH	"/system/btron386"

extern struct file_system	root_fs;
extern struct file		root;
extern int			debug_flag;

void	banner (void);
void	panic (char*);
void	print_binary (int);

#endif __CONFIG_H__
