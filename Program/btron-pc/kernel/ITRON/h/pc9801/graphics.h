/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 *
 *
 */

#ifndef __PC9801_GRAPHICS_H_
#define __PC9801_GRAPHICS_H_	1


/* Graphics Command */
#define Draw_Null	0x00000000		/* ���⤷�ʤ����ޥ��			*/
#define Draw_Line	0x00000001		/* 2���֤���ľ�������褹�륳�ޥ��	*/
#define Draw_Circle	0x00000002		/* �ߤ��������ޥ��			*/
#define Draw_Box	0x00000003		/* Ȣ���������ޥ��			*/


#define Clear_Display	0x00010000		/* �������Τ�õ��			*/

#define Reset_Graphics	0x00100000		/* ����ե��å����֥����ƥ�Υꥻ�å�	*/

#define Init_Graphics	0x10000000		/* ����ե��å����֥����ƥ�ν����	*/


/* ����ե��å��ǥХ����������׵������Ȥ��˻��Ѥ��빽¤��
 */
struct graphic_packet
{
  int	command;
  union
    {
      struct 
	{
	  W	x1,y1;
	  W	x2,y2;
	} line;
    } b;
};


#endif /* __PC9801_GRAPHICS_H_ */

