/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 *	BOOT で使用する構造体の定義
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/boot.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $
 *
 *
 * $Log: boot.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.7  1995-09-21 15:50:34  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.6  1995/06/04  03:35:04  night
 * machine_info 構造体の追加。
 *
 * Revision 1.5  1995/01/13  13:54:52  night
 * 註釈に RCS マクロが含まれていたので、再登録するたびに註釈がおかしくなっ
 * ていた。註釈を変更した。
 *
 * Revision 1.4  1995/01/13  13:51:47  night
 * module_info 構造体に name 要素を追加。
 * module_type に driver と lowlib を追加。
 *
 * Revision 1.3  1995/01/09  14:10:11  night
 * Header: /usr/local/src/master/btron/src/boot/2nd/boot.h,v 1.4 1995/01/13 13:51:47 night Exp $ と、 Log: boot.h,v $
 * を追加。
 *
 */

#ifndef __BOOT_H__
#define __BOOT_H__	1

#define MAX_MODULE_NAME	40

enum module_type 
{
  kernel	= 0,
  user		= 1,
  lowlib	= 2, 
  server      	= 3,
  driver	= 4
};


/* 複数のモジュールを読み込むためのヘッダ
 *
 *	ヘッダは以下の要素からできている
 *
 *	1) cookie:	マジック番号(ヘッダ識別用)
 *	2) count:	モジュール数
 *	3) module_info:	各モジュールの情報
 *	   length
 *	   vaddr
 *	   entry
 *	   mode
 */
struct module_info
{
  int		length;		/* ディスク上での大きさ */
  int		mem_length;	/* メモリ中での大きさ; 
				   ヘッダを含まず、BSS 領域を含む */
  unsigned int		vaddr;	/* 仮想アドレス	*/
  unsigned int		paddr;	/* 実アドレス	*/
  unsigned int		entry;
  enum module_type	type;
  char		name[MAX_MODULE_NAME];
};

struct machine_info
{
  unsigned int	ext_mem;
  unsigned int	real_mem;
  unsigned int	base_mem;
};

struct boot_header
{
  unsigned char		cookie;		/* 0001 に固定 */
  int			count;
  struct machine_info	machine;
  struct module_info	modules[0];
};


#endif /* __BOOT_H__ */
