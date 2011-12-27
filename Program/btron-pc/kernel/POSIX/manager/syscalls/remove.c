/*

B-Free Project $B$N@8@.J*$O(B GNU Generic PUBLIC LICENSE $B$K=>$$$^$9!#(B

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: remove.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  2000-05-06 03:54:31  naniwa
 * implement mkdir/rmdir, etc.
 *
 * Revision 1.1  1999/03/21 14:01:52  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

/* psc_rmeove_f - $B%U%!%$%k$N:o=|(B
 */
W
psc_remove_f (struct posix_request *req)
{
#ifdef USE_ALLCOA
  B			*pathname;
#else
  B			pathname[MAX_NAMELEN];
#endif
  W			errno;
  struct inode		*startip;
  struct access_info	acc;


#ifdef USE_ALLOCA
  pathname = alloca (req->param.par_remove.pathlen);
  if (pathname == NULL)
    {
      /* $B%a%b%j<hF@%(%i!<(B */
      put_response (req, EP_NOMEM, 0, 0, 0);
      return (FAIL);
    }
#endif

  errno = vget_reg (req->caller, req->param.par_open.path, req->param.par_open.pathlen, pathname);
  if (errno)
    {
      /* $B%Q%9L>$N%3%T!<%(%i!<(B */
      if (errno == E_PAR)
	put_response (req, EP_INVAL, 0, 0, 0);
      else
	put_response (req, EP_FAULT, 0, 0, 0);
	
      return (FAIL);
    }


  if (*pathname != '/')
    {
      errno = proc_get_cwd (req->procid, &startip);
      if (errno)
	{
	  put_response (req, errno, 0, 0, 0);
	  return (FAIL);
	}
    }
  else
    {
      startip = rootfile;
    }
  errno = proc_get_euid (req->procid, &(acc.uid));
  if (errno)
    {
      put_response (req, errno, 0, 0, 0);
      return (FAIL);
    }

  errno = proc_get_egid (req->procid, &(acc.gid));
  if (errno)
    {
      put_response (req, errno, 0, 0, 0);
      return (FAIL);
    }

  errno = fs_remove_file (startip,
			  pathname,
			  &acc);
  if (errno)
    {
      /* $B%U%!%$%k$,%*!<%W%s$G$-$J$$(B */
      put_response (req, errno, 0, 0, 0);
      return (FAIL);
    }
  
  put_response (req, EP_OK, 0, 0, 0);
  return (SUCCESS);
}  
