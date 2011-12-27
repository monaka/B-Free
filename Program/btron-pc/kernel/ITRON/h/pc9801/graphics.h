/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

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
#define Draw_Null	0x00000000		/* 何もしないコマンド			*/
#define Draw_Line	0x00000001		/* 2点間を結ぶ直線を描画するコマンド	*/
#define Draw_Circle	0x00000002		/* 円を描くコマンド			*/
#define Draw_Box	0x00000003		/* 箱を描くコマンド			*/


#define Clear_Display	0x00010000		/* 画面全体を消去する			*/

#define Reset_Graphics	0x00100000		/* グラフィックサブシステムのリセット	*/

#define Init_Graphics	0x10000000		/* グラフィックサブシステムの初期化	*/


/* グラフィックデバイスへ描画要求を送るときに使用する構造体
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

