/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/init/ether.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static char rcsid[] = "Id: command.c,v 1.21 1997/10/18 12:40:13 night Exp $";


#include "init.h"
#include "../kernlib/device.h"
#include "../device/ne2000/ne2000.h"
#include "../kernlib/libkernel.h"

ID	ether_port;

UB	my_macaddr[6];


W
ether (W ac, B **av)
{
  DDEV_REQ		req;		/* 要求パケット */
  DDEV_RES		res;		/* 返答パケット */
  W			rsize;
  ER			error;
  ID			recv;
  ID			console;


  if (ac < 1)
    {
      printf ("usage: ether command\n");
      printf ("             reset\n");
      printf ("             probe\n");
      printf ("             init\n");
      printf ("             macaddr\n");
      printf ("             arp\n");
      printf ("             \n");

      return (0);
    }

  recv = get_port (sizeof (DDEV_RES), sizeof (DDEV_RES));
  if (recv <= 0)
    {
      return (E_SYS);
    }

  if (find_port (NE2000_DRIVER, &console) != E_PORT_OK)
    {
      return (E_SYS);
    }

  if (strcmp (av[1], "reset") == 0)
    {
      /* Ether adaptor reset */

      req.header.mbfid = recv;
      req.header.msgtyp = DEV_CTL;
      req.body.ctl_req.dd = 0;
      req.body.ctl_req.cmd = NE2000_RESET;
      req.body.ctl_req.len = 0;

      error = snd_mbf (console, sizeof (req), &req);
      if (error != E_OK)
	{
	  return (error);
	}
      rsize = sizeof (res);
      error = rcv_mbf (&res, (INT *)&rsize, recv);
      if (res.body.ctl_res.errcd != E_OK)
	{
	  del_mbf (recv);
	  return (res.body.ctl_res.errcd);
	}      
    }
  else if (strcmp (av[1], "probe") == 0)
    {
      /* Ether adaptor probe */

      req.header.mbfid = recv;
      req.header.msgtyp = DEV_CTL;
      req.body.ctl_req.dd = 0;
      req.body.ctl_req.cmd = NE2000_PROBE;
      req.body.ctl_req.len = 0;

      error = snd_mbf (console, sizeof (req), &req);
      if (error != E_OK)
	{
	  return (error);
	}
      rsize = sizeof (res);
      error = rcv_mbf (&res, (INT *)&rsize, recv);
      if (res.body.ctl_res.errcd != E_OK)
	{
	  del_mbf (recv);
	  return (res.body.ctl_res.errcd);
	}      
    }
  else if (strcmp (av[1], "init") == 0)
    {
      /* Ether adaptor initialize */
    }
  else if (strcmp (av[1], "macaddr") == 0)
    {
      W		i;

      req.header.mbfid = recv;
      req.header.msgtyp = DEV_CTL;
      req.body.ctl_req.dd = 0;
      req.body.ctl_req.cmd = NE2000_GETMAC;
      req.body.ctl_req.len = 0;

      error = snd_mbf (console, sizeof (req), &req);
      if (error != E_OK)
	{
	  del_mbf (recv);
	  return (error);
	}
      rsize = sizeof (res);
      error = rcv_mbf (&res, (INT *)&rsize, recv);
      if (res.body.ctl_res.errcd != E_OK)
	{
	  del_mbf (recv);
	  return (res.body.ctl_res.errcd);
	}

      printf ("mac address: ");
      for (i = 0; i < MACADDRLEN; i++)
	{
	  printf ("%x ", res.body.ctl_res.res[i]);
	  my_macaddr[i] = res.body.ctl_res.res[i];
	}
      printf ("\n");
    }
  else if (strcmp (av[1], "arp") == 0)
    {
#define MYADDR	0xC0A80003
#define htons(s)	(UH)(((UH)s << 8) | ((UH)s >> 8))

      struct arp_buf
      {
	UB	dest_ether[6];
	UB	src_ether[6];
	UH	ether_type;

	UH	hardware;
	UH	protocol;
	UB	hardware_length;
	UB	protocol_length;
	UH	opcode;
	UB	sender_macaddr[6];
	UB	sender_ip[4];
	UB	target_macaddr[6];
	UB	target_ip[4];
      } arp;
      W		i;

      for (i = 0; i < 6; i++)
	{
	  arp.src_ether[i] = my_macaddr[i];
	  arp.dest_ether[i] = 0xff;

	  arp.sender_macaddr[i] = my_macaddr[i];
	  arp.target_macaddr[i] = 0xff;
	}
      arp.ether_type = 0x0608;

      arp.sender_ip[0] = 192;
      arp.sender_ip[1] = 168;
      arp.sender_ip[2] = 0;
      arp.sender_ip[3] = 3;
      for (i = 0; i < 4; i++)
	{
	  arp.target_ip[i] = 0xff;
	}
      arp.hardware = htons (1);
      arp.protocol = htons (0x0800);
      arp.hardware_length = 0x06;
      arp.protocol_length = 0x04;
      arp.opcode = htons (1);

      req.header.mbfid = recv;
      req.header.msgtyp = DEV_WRI;
      req.body.wri_req.dd = 0;
      req.body.wri_req.start = &arp;
      req.body.wri_req.size = sizeof (arp);
      get_tid (&req.header.tskid);

      printf ("send ether\n");
      error = snd_mbf (console, sizeof (req), &req);
      if (error != E_OK)
	{
	  del_mbf (recv);
	  return (error);
	}
      rsize = sizeof (res);
      error = rcv_mbf (&res, (INT *)&rsize, recv);
      if (res.body.ctl_res.errcd != E_OK)
	{
	  del_mbf (recv);
	  return (res.body.ctl_res.errcd);
	}
    }
  else
    {
      printf ("Unknown command: %s\n", av[1]);
      del_mbf (recv);
      return (1);
    }

  del_mbf (recv);
  return (0);
}


