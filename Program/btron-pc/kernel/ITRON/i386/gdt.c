/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
** gdt.c --- GDT アクセスルーチン集
**
*/



#include "itron.h"
#include "version.h"
#include "errno.h"
#include "task.h"
#include "func.h"



/* set_gdt --- 指定した GDT のエントリに値をセットする。
 *
 * 引数：
 *	int	index	新しく値をセットするGDTのエントリを指すエントリ
 *	VP	desc	ディスクリプタへのポインタ
 *			(ディスクリプタ自体は、8バイトの大きさをもつ)
 *
 * 返り値：
 *	エラー番号
 *		E_OK	正常時	
 *		E_PAR	indexの値がおかしい(GDTの範囲をオーバーしている)
 *
 */
ER
set_gdt (int index, GEN_DESC *desc)
{
  GEN_DESC	*p;

  if ((index <= 0) || (index > MAX_GDT))
    {
      return (E_PAR);	/* パラメータがおかしい */
    }
  p = (GEN_DESC *)GDT_ADDR;
  bcopy (desc, &p[index], sizeof (GEN_DESC));
  return (E_OK);
}


/* get_gdt --- 指定した GDT のエントリのディスクリプタを取り出す。
 *
 * 引数：
 *	int	index	値を取り出するGDTのエントリを指すエントリ
 *
 * 返り値：
 *	引数で指定されたGDTのエントリへのポインタ。
 *
 */
GEN_DESC *
get_gdt (int index)
{
  GEN_DESC	*p;

#if 0
  printk ("get_gdt = %d\n", index);	/* */
#endif
  if ((index <= 0) || (index > MAX_GDT))
    {
      return (NULL);	/* パラメータがおかしい */
    }

  p = (GEN_DESC *)GDT_ADDR;
  return (&p[index + TSS_BASE]);
}





