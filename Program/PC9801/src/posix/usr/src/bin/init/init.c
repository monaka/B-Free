/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/bin/init/init.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/bin/init/init.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

/*

 POSIX 環境初期化プログラム


 POSIX 環境の立ち上げ手順
 ------------------------

 1) BOOTプログラムが Portmanager, POSIX server(s) をメモリ中にロード
 2) portmanager の実行
 3) POSIX server の起動
    src/kernel/make/config.tab に書いてある順番で起動
    (1) プロセスマネージャの起動
    (2) メモリマネージャの起動
    (3) ファイルマネージャの起動
 4) init (本プログラム)の起動
    (1) root ファイルシステムをマウントする
    (2) /etc/init.sh の内容を実行する

 */
#include "init.h"

/*
 * init プログラムの main 関数。
 * 
 *
 */
__main ()
{
  init_program ();
  banner ("POSIX Environment started.\n");
  mount_root (ROOTDEV);
  do_inittab ();
  for (;;)
    {
      slp_tsk ();
    }    
}

init_program ()
{
  dbg_printf ("init: start\n");
}


do_inittab (void)
{
#ifdef notdef
  interpret (INITTAB);
#endif /* notdef */
}
