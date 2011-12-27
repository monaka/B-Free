/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* $Id: GraphicOp.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static char rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/init/libMittenCmd/GraphicOp.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

#include <types.h>
#include "CellMemory.h"
#include "mitten-command.h"

static struct {
  long orgX;
  long orgY;
  long x;
  long y;
  long color;
  long mode;
} path;

ER
gsave (void)
{
  path.orgX = path.orgY = 0;
  path.x = path.y = 0;
  path.color = 0;
  path.mode = 0;

  return E_OK;
}

ER
translate(void)
{
  ER result;
  Cell *c;
  long x;
  long y;

  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&y, c);
  MITN_ERROR_CHECK(result);
  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&x, c);
  MITN_ERROR_CHECK(result);

  path.orgX = x;
  path.orgY = y;

  return E_OK;
}

ER
moveto(void)
{
  ER result;
  Cell *c;
  long x;
  long y;

  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&y, c);
  MITN_ERROR_CHECK(result);
  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&x, c);
  MITN_ERROR_CHECK(result);

  path.x = x;
  path.y = y;

  return E_OK;
}

ER
rmoveto(void)
{
  ER result;
  Cell *c;
  long x;
  long y;

  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&y, c);
  MITN_ERROR_CHECK(result);
  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&x, c);
  MITN_ERROR_CHECK(result);

  path.x += x;
  path.y += y;

  return E_OK;
}

ER
setcolor(void)
{
  ER result;
  Cell *c;
  long color;

  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&color, c);
  MITN_ERROR_CHECK(result);

  path.color = color;

  return E_OK;
}

ER
setmode(void)
{
  ER result;
  Cell *c;
  long mode;

  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&mode, c);
  MITN_ERROR_CHECK(result);

  path.mode = mode;

  return E_OK;
}

ER
box(void)
{
  long width;
  long height;
  Cell *c;
  ER result;

  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&height, c);
  MITN_ERROR_CHECK(result);
  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&width, c);
  MITN_ERROR_CHECK(result);

  result = box_vga (path.x + path.orgX,
		    path.y + path.orgY,
		    width,
		    height,
		    path.color);
  MITN_ERROR_CHECK(result);

  return E_OK;
}

ER
fillbox(void)
{
  long width;
  long height;
  Cell *c;
  ER result;

  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&height, c);
  MITN_ERROR_CHECK(result);
  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&width, c);
  MITN_ERROR_CHECK(result);

  result = fillbox_vga (path.x + path.orgX,
			path.y + path.orgY,
			width,
			height,
			path.color);
  MITN_ERROR_CHECK(result);

  return E_OK;
}

ER
lineto(void)
{
  long x;
  long y;
  Cell *c;
  ER result;

  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&y, c);
  MITN_ERROR_CHECK(result);
  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&x, c);
  MITN_ERROR_CHECK(result);

  result = line_vga (path.x + path.orgX,
		     path.y + path.orgY,
		     x + path.orgX,
		     y + path.orgY,
		     path.color,
		     path.mode);
  MITN_ERROR_CHECK(result);

  path.x = x;
  path.y = y;

  return E_OK;
}

ER
rlineto(void)
{
  long x;
  long y;
  Cell *c;
  ER result;

  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&y, c);
  MITN_ERROR_CHECK(result);
  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&x, c);
  MITN_ERROR_CHECK(result);

  result = line_vga (path.x + path.orgX,
		     path.y + path.orgY,
		     path.x + path.orgX + x,
		     path.y + path.orgY + y,
		     path.color,
		     path.mode);
  path.x += x;
  path.y += y;

  MITN_ERROR_CHECK(result);

  return E_OK;
}

ER
draw_window(void)
{
  int x;
  int y;
  int width;
  int height;
  Cell *c;
  ER result;

  eval ("gsave 4 copy 4 2 roll translate");

  /* 一番外側の長方形 (バックグラウンド) */
  eval ("2 copy 2 setcolor fillbox");

  /* 一番外側の長方形 */
  eval ("2 copy 1 setcolor box");

  /* タイトルバー */
  eval ("2 setmode 1 setcolor 0 18 moveto");
  eval ("2 copy pop 0 rlineto");

  /* タイトルバーの中身 */
  eval ("3 setcolor 20 2 moveto");
  eval ("2 copy pop 25 sub 14 box");

  /* parameter for shade-band */
  eval ("1 1 moveto");
  eval ("2 sub exch 2 sub");

  /* shade-band (y, x) */
  eval ("2 copy");
  eval ("7 setcolor 0 rlineto 1 setcolor 0 exch rlineto");
  eval ("neg 0 rlineto 7 setcolor neg 0 exch rlineto");

  /* 中身の長方形 */
  eval ("5 20 moveto");
  eval ("2 copy");
  eval ("45 sub exch 30 sub exch 2 copy");
  eval ("3 setcolor fillbox exch");
  /* shade-band (y, x) */
  eval ("2 copy");
  eval ("1 setcolor 0 rlineto 7 setcolor 0 exch rlineto");
  eval ("neg 0 rlineto 1 setcolor neg 0 exch rlineto");

  /* スクロールバー (たて) */
  eval ("2 copy");
  eval ("5 setcolor exch 20 sub 20 moveto 45 sub 15 exch box");
  
  /* スクロールバー (よこ) */
  eval ("2 copy");
  eval ("5 setcolor 20 sub 5 exch moveto 30 sub 15 box");
  
  /* 変形 BOX */
  eval ("2 copy");
  eval ("5 setcolor 20 sub exch 20 sub exch moveto 15 15 box");
	

#if 0
  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&height, c);
  MITN_ERROR_CHECK(result);
  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&width, c);
  MITN_ERROR_CHECK(result);
  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&y, c);
  MITN_ERROR_CHECK(result);
  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&x, c);
  MITN_ERROR_CHECK(result);

#if 0
  line_vga (x + width - 1, y, x + width - 1, y + height, 0, 1);
  line_vga (x, y + height - 1, x + width, y + height - 1, 0, 1);

  box_vga (x + 5, y + 20, width - 30, height - 45, 4);			/* 中身の長方形 */
  fillbox_vga (x + 7, y + 22, width - 34, height - 49, 7);
  line_vga (x, y + 18, x + width, y + 18, 0, 2);				/* タイトルバー */
  box_vga (x + 20, y + 2, width - 25, 14, 3);				/* タイトルバーの中身 */

  box_vga (x + width - 20, y + 20, 15, height - 45, 5);				/* スクロールバー (たて) */
  box_vga (x + 5, y + height - 20, width - 30, 15, 5);				/* スクロールバー (よこ) */
  box_vga (x + width - 20, y + height - 20, 15, 15, 5);				/* 変形 BOX */
#endif

  set_bitmap (x + 2, y + 2, mark_width, mark_height, sizeof (mark_bits), mark_bits, 0, 7);	/* タイトルバーのアイコン */

  set_bitmap (x + width - 20, y + height - 20, movebox_width, movebox_height, sizeof (movebox_bits), movebox_bits, 0, 6);	/* 変形 BOX のアイコン */
#endif

  return E_OK;
}

#define SCROLL_UP   0
#define SCROLL_DOWN 1

#include "../init.h"

static void test_mesh(int dist,int t)
{
  int x,y;
  line_vga (0,0,639,479,8,0,1);
  for(x=y=0;(x<640)||(y<480);x+=dist,y+=dist){
    if(x<640){
      line_vga(x,0,x,479,8,1);
      //line_vga(x+t,0,x+t,479,8,1);
    }
    if(y<480){
      line_vga(0,y,639,y,8,1);
      //line_vga(0,y+t,639,y+t,8,1);
    }
  }
}

#define MakeMask(index)         (1 << (index))

void disp_smask(W x)
{
W mask,k,i,t;
  for(mask=0,k = 7 - (x % 8U); k >= 0; --k)
    {
      mask |= MakeMask(k);
    }
  printf("smask = ");
  for(i=7;i>=0;i--){
    t = MakeMask(i);
    if((mask&t)==t){
      printf("1");
    }else{
      printf("0");
    }
  }
  printf("B\n");
}
void disp_emask(W x)
{
W mask,k,i,t;
  for(mask=0,k = (x % 8U); k > 0; --k)
    {
      mask |= MakeMask(8-k);
    }
  printf("emask = ");
  for(i=7;i>=0;i--){
    t = MakeMask(i);
    if((mask&t)==t){
      printf("1");
    }else{
      printf("0");
    }
  }
  printf("B\n");
}

extern char tolower(char c);
ER
scroll (void)
{
  int x,y;
  char c;

  graphic_mode ();

  printf("scroll test v.0.01\n");
  printf("  end -> push Q key\n");
  printf("  push any key to continue\n");

  c = tolower(getc(stdin));
  if(c=='q'){
    return E_OK;
  }

  line_vga (0,0,639,479,8,0);
  for(x=y=0;(x<640)||(y<480);x+=5,y+=5){
    test_mesh(8,0);
    if(((x+303)<640)&&((y+203)<480)){
      disp_smask(13+x);
      disp_emask(303+x);
      box_vga (13+x-1,13+y-1,290+1,190+1,5);
      scroll_vga (SCROLL_UP,13+x,13+y,290,190,37,3);
    }

    c = tolower(getc(stdin));
    if(c=='q'){
      return E_OK;
    }
    fillbox_vga(0,0,640,480,0);

    c = tolower(getc(stdin));
    if(c=='q'){
      return E_OK;
    }
    test_mesh(8,3);
    if(((x+207)<640)&&((y+107)<480)){
      disp_smask(30+x);
      disp_emask(207+x);
      box_vga (30+x-1,63+y-1,177+1,117+1,5);
      scroll_vga (SCROLL_DOWN,30+x,63+y,177,117,47,4);
    }

    c = tolower(getc(stdin));
    if(c=='q'){
      return E_OK;
    }
    fillbox_vga(0,0,640,480,0);

    c = tolower(getc(stdin));
    if(c=='q'){
      return E_OK;
    }
  }
  line_vga (639,0,0,479,8,0);
}
