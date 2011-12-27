#include "../init.h"
#include "lowlib.h"
#include "CellMemory.h"
#include "mitten-command.h"

/* posix_mountroot
 *
 *
 */
W
posix_mountroot (W ac, B **av)
{
  ID	posix;
  ID	device_port;
  ER	error;
  struct posix_request	req;
  struct posix_response res;
  ID	rport;
  W	rsize;
  B	*device_name;


  device_name = av[1];
  
#ifdef notdef
  if (find_port (device_name, &device_port) != E_PORT_OK)
    {
      printf ("init: Cannot open device.\n");
      return (E_SYS);
      /* DO NOT REACHED */
    }
#endif
  if (strcmp (device_name, "ide0") == 0)
    {
      device_port = 0x80010002;
    }
  else if (strcmp (device_name, "fd") == 0)
    {
      device_port = 0x80000000;
    }
  else
    {
      printf ("init: Cannot open device.\n");
      return (E_SYS);
      /* DO NOT REACHED */
    }

  printf ("init: device name is <%s>\n", device_name);

  error = find_port ("manager.posix", &posix);
  if (error != E_OK)
    {
      printf ("Cannot find port.\n");
      return (error);
    }

  rport = get_port (sizeof (res), sizeof (res));
  if (rport == 0)
    {
      printf ("Cannot allocate port\n");
      return (E_NOMEM);
    }

#ifdef notdef
  printf ("init: pmountroot, device = 0x%x, send port = %d, receive port = %d\n",
	  device_port, posix, rport);
#endif

  req.receive_port = rport;
  req.msg_length = sizeof (res);
  req.operation = PSC_MOUNTROOT;
  req.param.par_mountroot.device = device_port;
  req.param.par_mountroot.fstype = 1;
  req.param.par_mountroot.option = 0;
  snd_mbf (posix, sizeof (req), &req);
  rsize = sizeof (res);
  printf ("init: perform snd_mbf systemcall\n");
  rcv_mbf (&res, &rsize, rport);
  printf ("init: perform rcv_mbf systemcall\n");
  if (res.errno)
    {
      printf ("syscall error = %d\n", res.errno);
    }
  else
    {
      printf ("syscall success.\n");
    }

  del_mbf (rport);
  return (E_OK);
}


/*
 *
 */
ER
posix (W ac, B **av)
{
  W	device;
  B	*device_name;
  W	drive, partition;

  device_name = av[1];
  if (strncmp (device_name, "/dev/ide", strlen ("/dev/ide")) == 0)
    {
      device = 0x80010000;
      drive = device_name[strlen ("/dev/ide")] - '0';
      partition = device_name[strlen ("/dev/ide") + 1] - '0';
      if ((drive < 0) || (partition <= 0))
	{
	  printf ("Cannot mount root filesystem.\n");
	  return (E_SYS);
	}

      device = device | ((drive & 0xff) << 8)  | (partition & 0xff);
      printf ("mount device = ide/0x%x\n", device);		/* */
    }
  else if (strncmp (device_name, "/dev/fd", strlen ("/dev/fd")) == 0)
    {
      device = 0x80000000;
      drive = device_name[strlen ("/dev/fd")] - '0';
      partition = 0;
      if (drive < 0)
	{
	  printf ("Cannot mount root filesystem.\n");
	  return (E_SYS);
	}

      device = device | ((drive & 0xff) << 8);
      printf ("mount device = fd/0x%x\n", device);		/* */
    }
  else
    {
      printf ("init: Cannot open device.\n");
      return (E_SYS);
      /* DO NOT REACHED */
    }

  return (posix_init (device));
}

ER
posix_noaction (void)
{
  printf ("posix noaction: %d\n", noaction ());

  return E_OK;
}



ER
posix_mount (W ac, B **av)
{
  ID	posix;
  ER	error;
  struct posix_request	req;
  struct posix_response res;
  ID	rport;
  W	rsize;
  B	*device_name;


  error = find_port ("manager.posix", &posix);
  if (error != E_OK)
    {
      printf ("Cannot find port.\n");
      return (error);
    }
  rport = get_port (sizeof (res), sizeof (res));
  if (rport == 0)
    {
      printf ("Cannot allocate port\n");
      return (E_NOMEM);
    }

  device_name = av[1];

  dbg_printf ("init: pmount, send port = %d, receive port = %d\n",
	      posix, rport);
  req.receive_port = rport;
  req.msg_length = sizeof (res);
  req.operation = PSC_MOUNT;
  req.param.par_mount.devnamelen = strlen (av[1]);
  req.param.par_mount.devname = av[1];
  req.param.par_mount.dirnamelen = strlen (av[2]);
  req.param.par_mount.dirname = av[2];
  req.param.par_mount.fstype = 1;
  req.param.par_mount.option = 0;
  snd_mbf (posix, sizeof (req), &req);
  rsize = sizeof (res);
  rcv_mbf (&res, &rsize, rport);
  printf ("syscall error = %d\n", res.errno);
  del_mbf (rport);
  return (E_OK);
}



/* pdump
 *
 * POSIX マネージャに対して、プロセスの情報を出力させる。
 *
 */
ER
posix_dump (W ac, B **av)
{
  ID	posix;
  ER	error;
  struct posix_request	req;
  struct posix_response res;
  ID	rport;
  W	rsize;
  ID	myself;

  error = find_port ("manager.posix", &posix);
  if (error != E_OK)
    {
      printf ("Cannot find port.\n");
      return (error);
    }
  rport = get_port (sizeof (res), sizeof (res));
  if (rport == 0)
    {
      printf ("Cannot allocate port\n");
      return (E_NOMEM);
    }

  if (get_tid (&myself) != E_OK)
    {
      printf ("Can not get own taskid.\n");
      return (E_SYS);
    }

  printf ("init: pdump, send port = %d, receive port = %d\n",
	  posix, rport);
  req.param.par_misc.arg.set_procinfo.proc_pid = atoi (av[1]);
  req.receive_port = rport;
  req.msg_length = sizeof (res);
  req.operation = PSC_MISC;
  req.procid = 0;
  req.caller = myself;
  req.param.par_misc.cmd = M_PROC_DUMP;
  snd_mbf (posix, sizeof (req), &req);

  rsize = sizeof (res);
  rcv_mbf (&res, &rsize, rport);
  printf ("syscall error = %d\n", res.errno);
  del_mbf (rport);
  return (E_OK);
}


/* pvmdump
 *
 * POSIX マネージャに対して、プロセスの仮想メモリ情報を出力させる。
 *
 */
ER
posix_vm_dump (W ac, B **av)
{
  ID	posix;
  ER	error;
  struct posix_request	req;
  struct posix_response res;
  ID	rport;
  W	rsize;
  ID	myself;

  if (ac < 1)
    {
      printf ("usage: posix_vmdump procid\n");
      return (E_SYS);
    }


  error = find_port ("manager.posix", &posix);
  if (error != E_OK)
    {
      printf ("Cannot find port.\n");
      return (error);
    }
  rport = get_port (sizeof (res), sizeof (res));
  if (rport == 0)
    {
      printf ("Cannot allocate port\n");
      return (E_NOMEM);
    }

  if (get_tid (&myself) != E_OK)
    {
      printf ("Can not get own taskid.\n");
      return (E_SYS);
    }

  printf ("pvmdump : process ID = %d\n", atoi (av[1]));
  req.param.par_misc.arg.procid = atoi (av[1]);
  req.receive_port = rport;
  req.msg_length = sizeof (res);
  req.operation = PSC_MISC;
  req.procid = 0;
  req.caller = myself;
  req.param.par_misc.cmd = M_VM_DUMP;
  snd_mbf (posix, sizeof (req), &req);

  rsize = sizeof (res);
  rcv_mbf (&res, &rsize, rport);
  printf ("syscall error = %d\n", res.errno);
  del_mbf (rport);
  return (E_OK);
}


/* exec system call のテスト 
 */
W
posix_exec_child (B *path)
{
  W	err;

  printf ("posix exec: path = \"%s\"\n", path);
#ifdef notdef
  err = posix_exec (path);
#endif
  printf ("posix exec -- fail. (%d)\n", err);
  for (;;)
    {
      slp_tsk ();
    }

}


ER
posix_exec_test (W ac, B **av)
{
  W	err;

  err = posix_fork (posix_exec_child, av[1]);

  printf ("[posix_exec_tset] err = %d\n", err);

  return E_OK;
}


/* fork system call のテスト
 *
 */
W
posix_fork_test_child (void)
{
  printf ("I am a child!\n");
  _exit(1);
}

ER
posix_fork_test (void)
{
  W	err;

  printf("child start addr %x\n", posix_fork_test_child);
  err = posix_fork (posix_fork_test_child, NULL);
  printf ("err = %d\n", err);  

  return E_OK;
}

ER cexec(void)
{
  int pid, res;
  Cell *c;
  B fname[255];
  ER result;

  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_ToString(fname, c, 255);
  MITN_ERROR_CHECK(result);

  if ((res = fork()) == 0) {
    execve(fname, NULL, NULL);
    _exit(1);
  }
  else if (res == -1) {
    printf("fork error\n");
    return(res);
  }
  else {
    printf("waiting pid=%d ...\n", res);
    pid = waitpid(-1, &res, 0);
    printf("child process exited pid=%d stat=%d\n", pid, res);
    return(E_OK);
  }
}
