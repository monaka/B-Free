/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: fcntl.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  2000-02-20 09:40:04  naniwa
 * minor fix
 *
 * Revision 1.3  2000/01/30 03:04:48  naniwa
 * minor fix
 *
 * Revision 1.2  2000/01/29 16:26:53  naniwa
 * partially implemented
 *
 * Revision 1.1  1999/03/21 14:01:51  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

/* ctl_device - デバイスにコントロールメッセージを送る
 *
 */
static W control_device (ID device, struct posix_request *preq)
{
  DDEV_REQ	req;		/* 要求パケット */
  DDEV_RES	res;		/* 返答パケット */
  ID		res_port;
  W		errno;
  ID		send_port;
  UW		dd;
  W		rlength;

  errno = get_device_info (device, &send_port, &dd);
  if (errno)
    {
      return (errno);
    }

  res_port = get_port (sizeof (res), sizeof (res));
  if (res_port <= 0)
    {
      return (EP_NOMEM);
    }

  req.header.mbfid = res_port;
  req.header.msgtyp = DEV_CTL;
  req.body.ctl_req.dd = dd;
  req.body.ctl_req.cmd = (preq->param.par_fcntl.cmd >> 16) & 0x0FFFF;
  req.body.ctl_req.len = preq->param.par_fcntl.cmd & 0x0FFFF;
  if (req.body.ctl_req.len == 0) {
    /* W に cast しているが、UB のまま代入したほうが良いかも */
    ((W *) req.body.ctl_req.param)[0] = (W) preq->param.par_fcntl.arg;
    req.body.ctl_req.len = sizeof(W);
  }
  else {
    errno = vget_reg (preq->caller, preq->param.par_fcntl.arg,
		      req.body.ctl_req.len, req.body.ctl_req.param);
    if (errno) {
      dbg_printf("fctl: vget_reg error\n");
      del_mbf (res_port);
      return (errno);
    }
  }

  errno = snd_mbf (send_port, sizeof (req), &req);
  if (errno != E_OK)
    {
      dbg_printf ("cannot send packet. %d\n", errno); 	/* */
      del_mbf (res_port);
      return (EP_NODEV);
    }
  
  rlength = sizeof (res);
  errno = rcv_mbf ((UB *)&res, (INT *) &rlength, res_port);
  if (errno != E_OK)
    {
      dbg_printf ("cannot receive packet. %d\n", errno); 	/* */
      del_mbf (res_port);
      return (EP_NODEV);
    }
  
  del_mbf (res_port);
  return (res.body.ctl_res.errinfo);
}

/* psc_fcntl_f - ファイルに対して特殊な操作を行う。
 */
W
psc_fcntl_f (struct posix_request *req)
{
  W		errno;
  struct file	*fp;
  ID		device;

  errno = proc_get_file (req->procid, req->param.par_fcntl.fileid, &fp);
  if (errno)
    {
      put_response (req, errno, -1, 0, 0);
      return (FAIL);
    }
  else if (fp == 0)
    {
      put_response (req, EP_INVAL, -1, 0, 0);
      return (FAIL);
    }
  else if (fp->f_inode == 0)
    {
      put_response (req, EP_INVAL, -1, 0, 0);
      return (FAIL);
    }

  if (fp->f_inode->i_mode & FS_FMT_DEV) {
      /* スペシャルファイルだった
       */
      device = fp->f_inode->i_dev;

      /* send message to the device.
       */
      errno = control_device(device, req);
      if (errno) {
	put_response (req, errno, errno, 0, 0);
	return (FAIL);
      }
      else {
	put_response (req, EP_OK, errno, 0, 0);
	return(SUCCESS);
      }
    }
  else {
    /* とりあえず、サポートしていないというエラーで返す
     */
    put_response (req, EP_NOSUP, 0, 0, 0);
  }

  return (FAIL);
}  

