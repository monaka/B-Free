/*

B-Free Project $B$N@8@.J*$O(B GNU Generic PUBLIC LICENSE $B$K=>$$$^$9!#(B

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /usr/local/src/master/B-Free/Program/btron-pc/kernel/BTRON/devic
e/fd765a/fd.c,v 1.5 1999/04/13 04:14:40 monaka Exp $ */
static char rcsid[] = "@(#)$Header: /usr/local/src/master/B-Free/Program/btron-p
c/kernel/BTRON/device/fd765a/fd.c,v 1.5 1999/04/13 04:14:40 monaka Exp $";

/*
 * $Log: aha152xlow.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  1999-12-18 15:24:03  kishida0
 * linux$B$N%=!<%9$rDI2C$7$?(B
 *
 * Revision 1.3  1999/12/09 17:13:12  kishida0
 * *** empty log message ***
 *
 * Revision 1.2  1999/12/02 16:02:34  kishida0
 * Here is start.
 *
 * Revision 1.1  1999/07/06 13:53:31  kishida0
 * add debug aha152x src
 *
 *
 *
 */
#define __AHA152XLOW_C__

#include "isa_scsi_internal.h"
#include "aha152x.h"
#include "aha152xlow.h"


//void outp(UW ind, UB data){
//  outb(ind,data);
//}




/*
 * $BIaDL$O$3$3$,(B aha152xlow.c
 *
 */

UW get_config_data(int iBusNum, int iDevNum, int iFuncNum, int iRegAddr)
{
int config_data = 0;

  return config_data;
}


/* aha152x $B%"%@%W%?$,B8:_$7$F$$$k$+$I$&$+$r%A%'%C%/$9$k(B
 */
ER
probe (struct device *dev)
{
  dbg_printf("ISA_SCSI:probe\n");
  dbg_printf("%s",rcsid);
  return (E_OK);
}

/*
 * $BIaDL$O$3$30J2<$,(B main.c
 *
 */

/*
 *	 $B6I=jJQ?t72$N@k8@(B
 *
 */
static W	mydevid;	/* $B<+J,<+?H$N(Bid */
static ID	recvport;	/* $BMW5a<u$1$D$1MQ%]!<%H(B */
static W	initialized;

struct device	dev_table[MAX_ISA_SCSI];


static void	main_loop (void);
static void	init_driver (void);


/* start --- sa_scsi $B%I%i%$%P$N%a%$%s4X?t(B
 */
void
start ()
{
  probe (&dev_table[0]); /* $B%G%P%$%9$,B8:_$7$F$$$k$+$r%A%'%C%/$9$k(B */
  init_driver ();

  printf ("registed isa_scsi driver.\n");
  main_loop ();
}

void
panic(char *s)
{
      dbg_printf (s);
      slp_tsk ();
}

/*
 * $B=i4|2=(B
 *
 * o $B%U%!%$%k%F!<%V%k(B (file_table) $B$N=i4|2=(B
 * o $BMW5a<u$1$D$1MQ$N%a%C%;!<%8%P%C%U%!(B ID $B$r%]!<%H%^%M!<%8%c$KEPO?(B
 */
static void
init_driver (void)
{
  int		i;
  ER		error;

  init_log ();

  /*
   * $BMW5a<u$1$D$1MQ$N%]!<%H$r=i4|2=$9$k!#(B
   */
  recvport = get_port (sizeof (DDEV_RES), sizeof (DDEV_RES));
  if (recvport <= 0)
    {
      panic ("isa_scsi: cannot make receive port.\n");
      /* $B%a%C%;!<%8%P%C%U%!@8@.$K<:GT(B */
    }

  error = regist_port (ISA_SCSI_DRIVER, recvport);
  if (error != E_OK)
    {
      /* error */
      panic("isa_scsi: cannot regist_port");
    }

}



/*
 *
 */
static void
main_loop (void)
{
  DDEV_REQ	req;
  extern ER	sys_errno;
  UW		rsize;

  /*
   * $BMW5a<u?.(B - $B=hM}$N%k!<%W(B
   */
  for (;;)
    {
      /* $BMW5a$N<u?.(B */
      get_req (recvport, &req, &rsize);
      switch (sys_errno)
	{
	case E_OK:
	  /* $B@5>o%1!<%9(B */
	  process_request (&req);
	  break;

	default:
	  panic ( " Unknown error ");/**/
	  break;
	}
    }

  /* $B$3$3$N9T$K$O!"Mh$J$$(B */
}


/*
 *
 */
ER
process_request (DDEV_REQ *req)
{
  switch (req->header.msgtyp)
    {
    case DEV_OPN:
      /* $B%G%P%$%9$N%*!<%W%s(B */
      open_isa_scsi (req->header.mbfid, &(req->body.opn_req));
      break;

    case DEV_CLS:
      /* $B%G%P%$%9$N%/%m!<%:(B */
      close_isa_scsi (req->header.mbfid, &(req->body.cls_req));
      break;

    case DEV_REA:
      read_isa_scsi (req->header.mbfid, &(req->body.rea_req));
      break;

    case DEV_WRI:
      write_isa_scsi (req->header.mbfid, &(req->body.wri_req));
      break;

    case DEV_CTL:
      control_isa_scsi (req->header.mbfid, &(req->body.ctl_req));
      break;
    }
}

/*
 * $B%G%P%$%9$N%*!<%W%s(B
 */
ER
open_isa_scsi (ID caller, DDEV_OPN_REQ *packet)
{
  /*** $B$3$3$K%3!<%I$r$D$$$+$7$F$/$@$5$$(B ***/

  return (E_NOSPT);
}

/*
 * $B%G%P%$%9$N%/%m!<%:(B
 */
ER
close_isa_scsi (ID caller, DDEV_CLS_REQ *packet)
{
  /*** $B$3$3$K%3!<%I$r$D$$$+$7$F$/$@$5$$(B ***/

  return (E_NOSPT);
}

/*
 *
 */
ER
read_isa_scsi (ID caller, DDEV_REA_REQ *packet)
{
  /*** $B$3$3$K%3!<%I$r$D$$$+$7$F$/$@$5$$(B ***/

  return (E_NOSPT);
}

/*
 *
 */
ER
write_isa_scsi (ID caller, DDEV_WRI_REQ *packet)
{
  /*** $B$3$3$K%3!<%I$r$D$$$+$7$F$/$@$5$$(B ***/

  return (E_NOSPT);
}

/*
 *
 */
ER
control_isa_scsi (ID caller, DDEV_CTL_REQ *packet)
{
  /*** $B$3$3$K%3!<%I$r$D$$$+$7$F$/$@$5$$(B ***/

  return (E_NOSPT);
}
