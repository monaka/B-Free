/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* mktable --- GDT/PAGE TABLE の作成
 *
 *
 *	今現在、page table は作成していない！！！！
 */

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#ifdef __LINUX__
#include <linux/types.h>
#include <linux/fcntl.h>
#else /* not __LINUX__ */
#include <sys/types.h>
#include <fcntl.h>
#endif /* not __LINUX__ */


#define TRUE	1
#define FALSE	0

#define BLOCK_SIZE	1024
#define PAGE_SIZE	4096

#define BOOT2_GDT_SIZE	(2 * PAGE_SIZE)
#define PAGE_TABLE_SIZE	(5 * PAGE_SIZE)

#define BUF_SIZE        BOOT2_GDT_SIZE


typedef unsigned short int      word16;
typedef unsigned char           byte;


struct gdt_t
{
  unsigned int	limit0:16;
  unsigned int	offset0:24;
  unsigned int	access:8;
  unsigned int	limit1:4;
  unsigned int	reserve:2;
  unsigned int	d_bit:1;
  unsigned int	g_bit:1;
  unsigned int	offset1:8;
};

#define SET_OFFSET(desc,x)		\
((desc).offset0 = x & 0xffffff);		\
((desc).offset1 = x >> 24 & 0xff);

#define SET_LIMIT(desc,x)		\
((desc).limit0 = x & 0xffff);		\
((desc).limit1 = (x >> 16) & 0xf);

#define GET_OFFSET(desc)	((desc).offset1 << 24 | (desc).offset0)
#define GET_LIMIT(desc)		((desc).limit1 << 16 | (desc).limit0)

byte    buffer[BUF_SIZE];

char	*readline (FILE *, char *);
int	iscomment (char *line);

void
main (int ac, char **av)
{
  FILE	*fp;
  int   outfd;
  char	line[100];
  struct gdt_t	gdt;
  unsigned char	*p;
  int	i;
  
  if (ac < 3)
    {
      fprintf (stderr, "usage: %s in-filename out-filename\n", av[0]);
      exit (0);
    }

  fp = fopen (av[1], "r");
  if (fp == NULL)
    {
      fprintf (stderr, "can't file open (%s)\n", av[1]);
      exit (-1);
    }

  outfd = open (av[2], O_CREAT | O_WRONLY, 0666);
  if (outfd < 0)
    {
      fprintf (stderr, "can't file open (%s)\n", av[2]);
      exit (-1);
    }
      
  p = (unsigned char *)buffer;
  i = 0;
  while (readline (fp, line) != NULL)
    {
      int	base;
      int	limit;
      int	access;
      if (!iscomment (line))
	{
	  sscanf (line, "%x:%x:%x", &base, &limit, &access);
	  SET_OFFSET(gdt, base);
	  SET_LIMIT(gdt, limit);
	  gdt.g_bit = 1;
	  gdt.d_bit = 1;
	  gdt.access = (unsigned int)(access & 0xFF);
	  printf ("%d: offset = 0x%x, limit = 0x%x, access = 0x%x\n",
		  ++i, GET_OFFSET(gdt), 
		  GET_LIMIT(gdt),
		  gdt.access);
	  write_gdt (&gdt, p);
          p += 8;
	}
    }
  write (outfd, buffer, sizeof (buffer));

/********************** Page Table の作成 */
  close (outfd);
  fclose (fp);
}

/*
 */
char *
readline (FILE *fp, char *line)
{
  int	ch;

  if (feof (fp))	return NULL;

  ch = getc (fp);
  while ((ch != EOF) && (ch != '\n'))
    {
      *line++ = ch;
      ch = getc (fp);
    }
  *line = '\0';
  return (++line);
}

/*
 */
int
iscomment (char *line)
{
  while (*line != '\0')
    {
      if (*line == ';')
	return (TRUE);
      if (!isspace (*line))
	return (FALSE);
      line++;
    }
  return (TRUE);
}

write_gdt (struct gdt_t *gdt, unsigned char *buffer)
{
  buffer[0] = gdt->limit0;
  buffer[1] = gdt->limit0 >> 8;
  buffer[2] = gdt->offset0 & 0xff;
  buffer[3] = (gdt->offset0 >> 8) & 0xff;
  buffer[4] = (gdt->offset0 >> 16) & 0xff;
  buffer[5] = gdt->access;
  buffer[6] = ((gdt->d_bit ? 0x40 : 0x00) |
	       (gdt->g_bit ? 0x80 : 0x00) |
	       gdt->limit1);
  buffer[7] = gdt->offset1;
}
