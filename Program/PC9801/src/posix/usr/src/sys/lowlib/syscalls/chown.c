/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/syscalls/chown.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $  */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/syscalls/chown.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 * $Log: chown.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:53:08  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/02/28  14:10:28  night
 * コンパイルできるように、構造体の名前などを定義されたものに直した。
 *
 * Revision 1.1  1995/02/27  14:23:30  night
 * 最初の登録
 *
 *
 */

/*
 *
 *
 */

#include <sys/types.h>
#include <errno.h>
#include "../funcs.h"
#include "../global.h"

/*
 * 指定したファイルの所有者とグループを変更する。
 * 所有者とグループは、それぞれ uid_t と gid_t で指定する。
 *
 * Syntax:
 *	int chown (const char *FILENAME, uid_t OWNER, gid_t GROUP)
 *
 * return valud:
 *	0	正常終了
 *	-1	異常終了
 *
 * このシステムコールの結果、errno には次の値が入る。
 *
 *	EROFS	指定したファイルのあるファイルシステムは、書き込みがで
 *		きない。
 *	EPERM	指定したファイルの変更権がない。
 *	
 */
int
psys_chown (void *argp)
{
  int	retval;
  struct a
    {
      const char	*filename;	/* 所有者を変更したいファイル */
      uid_t		owner;		/* 変更したい所有者 ID */
      gid_t		group;		/* 変更したいグループ ID */
    } *args = (struct a *)argp;

  retval = fm_chown (my_pid, args->filename, args->owner, args->group, &errno);
  return (retval);
}
