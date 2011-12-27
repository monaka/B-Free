/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* ctype.h
 *
 *
 */

#ifndef __H_CTYPE_H__
#define __H_CTYPE_H__	1

/* ISCONTROL --- ����������ȥ���ʸ�����ɤ�����Ĵ�٤�
 */
#define ISCONTROL(ch)	(!(ch & 0xe0))

/* CONTROL ---  �������Ϥ��줿ʸ���򥳥�ȥ���ʸ�����Ѵ����롣
 */
#define CONTROL(ch)	(ch & 0x1f)

#define isspace(ch)	((ch == ' ') || (ch == '\n') || (ch == '\t'))



#endif /* __H_CTYPE_H__ */
