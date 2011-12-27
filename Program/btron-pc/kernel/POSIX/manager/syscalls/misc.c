/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: misc.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  2000-01-30 09:10:15  naniwa
 * add M_PRINT_FLIST, and to call vsys_msc
 *
 * Revision 1.1  1999/03/21 14:01:51  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

W
psc_misc_f (struct posix_request *req)
{
  ER	err = 0;

#ifdef notdef
  printf ("POSIX: misc systemcall.(cmd = %d)\n", req->param.par_misc.cmd);
#endif

  if (req->param.par_misc.cmd < 0) {
    /* req->param.par_misc.cmd が負の場合 ITRON の vsys_msc を呼び出す */
    err = vsys_msc(-req->param.par_misc.cmd, NULL);
  }
  else {
    switch (req->param.par_misc.cmd)
      {
      case M_SET_PROCINFO:
	err = proc_set_info (req);
	break;
	
      case M_PROC_DUMP:
	err = proc_dump (req);
	break;

      case M_VM_DUMP:
	err = proc_vm_dump (req);
	break;

      case M_PRINT_FLIST:
	/* malloc の空きリストの表示 */
	print_freelist();
	break;
      }
  }

  put_response (req, err, err ? -1 : 0, 0, 0);
  if (err)
    return (FAIL);
  else
    return (SUCCESS);
}
