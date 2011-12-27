/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* $Id: command.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "$Id: command.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

/*
 * $Log: command.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.3  1996-01-06 16:00:42  night
 * ヘルプ内容の一部変更。
 *
 * Revision 1.2  1995/12/05 14:32:19  night
 * help(), dos_attach() の関数を追加。
 * dos_ls(), dos_cat() も追加したが、中身はない。
 *
 * Revision 1.1  1995/10/10  16:21:47  night
 * 最初の登録.
 * システム起動後、プロンプトを出力してコマンド入力 - 実行ができるところ
 * まで出来た。ただし、コマンドは echo だけ。
 *
 *
 */

#include "init.h"


static W	echo (W ac, B **av);
static W	read (W ac, B **av);
static W	help (W ac, B **av);
static W	dos_attach (W ac, B **av);
static W	dos_ls (W ac, B **av);
static W	dos_cat (W ac, B **av);

struct command command_table[] =
{
  { "echo",	echo },
  { "help",	help },
  { "dattach",	dos_attach},
  { "dls",	dos_ls},
  { "dcat",	dos_cat},
  { NULL,	NULL }
};

static W
echo (W ac, B **av)
{
  W	i;

  for (i = 1; i < ac; i++)
    {
      printf ("%s\n", av[i]);
    }
  return (0);
}

static W
help (W ac, B **av)
{
  printf ("HELP: \n");
  printf ("echo arguments     print arguments.\n");
  printf ("dattach            attach DOS file system.\n");
  printf ("dls                list of DOS files.\n");
  printf ("dcat               print of DOS file.\n");
}


static W
dos_attach (W ac, B **av)
{
  W	id;
  W	target, lun;
  W	partition;

  id = init_dosfs ("driver.scsi");
  if (id < 0)
    {
      printf ("dosfs: cannot attach filesystem.\n");
      return (id);
    }

  printf ("DOS FS id: %d\n", id);
  return (id);
}

static W
dos_ls (W ac, B **av)
{
}

static W
dos_cat (W ac, B **av)
{
}

