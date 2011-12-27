/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
#include <stdio.h>
#ifdef __LINUX__
#include <linux/types.h>
#include <linux/fcntl.h>
#else /* not __LINUX__ */
#include <sys/types.h>
#include <fcntl.h>
#endif /* not __LINUX__ */


#define MINIX_HEADER		32
#define GCC_HEADER		32

#define ALIGN_SIZE		4096
#define BUF_SIZE		ALIGN_SIZE

#define ROUNDUP(x,align)	(((x + (align - 1)) / align) * align)

/****************************************************************************
 *	main ---
 *
 *	引数	ac
 *		av[0] .... コマンド名
 *		av[1] .... 16 ビット動作部
 *		av[2] .... 32 ビット動作部
 */
void
main (int ac, char **av)
{
  int	fd;
  char	buf[BUF_SIZE];
  int	count;
  int	total16;
  int	total32;
  
  if (ac < 3)
    {
      fprintf (stderr, "usage: %s 16bit-boot 32bit-boot\n", av[0]);
      exit (0);
    }

  fd = open (av[1], O_RDONLY);
  if (fd == -1)
    {
      fprintf (stderr, "can't open %s for 16bit-mode boot\n", av[1]);
      exit (-1);
    }

  read (fd, buf, MINIX_HEADER);
  total16 = 0;
  bzero (buf, sizeof (buf));
  while ((count = read (fd, buf, sizeof (buf))) > 0)
    {
      write (1, buf, ALIGN_SIZE);
      total16 += count;
      bzero (buf, sizeof (buf));
    }
  close (fd);
  
  fd = open (av[2], O_RDONLY);
  if (fd == -1)
    {
      fprintf (stderr, "can't open %s for 32bit-mode boot\n", av[2]);
      exit (-1);
    }

#ifdef GCC_USE
  read (fd, buf, GCC_HEADER);
#else
  read (fd, buf, MINIX_HEADER);
#endif /* GCC_USE */
  total32 = 0;
  bzero (buf, sizeof (buf));
  while ((count = read (fd, buf, sizeof (buf))) > 0)
    {
      write (1, buf, ALIGN_SIZE);
      total32 += count;
      bzero (buf, sizeof (buf));
    }
  close (fd);

  fprintf (stderr, "done\n");
  fprintf (stderr, "16bit mode boot size --------- %d\n",
	   ROUNDUP (total16, ALIGN_SIZE));
  fprintf (stderr, "32bit mode boot size --------- %d\n",
	   ROUNDUP (total32, ALIGN_SIZE));
  exit (0);
}
