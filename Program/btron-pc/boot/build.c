/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* Part of BTRON/386
 *
 */

#include <stdio.h>
#ifdef __LINUX__
#include <linux/types.h>
#include <linux/fcntl.h>
#else /* not __LINUX__ */
#include <sys/types.h>
#include <fcntl.h>
#endif /* not __LINUX__ */

/* #define BLOCK_SIZE	1024 */
#define BLOCK_SIZE	512
#define PAGE_SIZE	4096

#ifdef notdef
#define BOOT1_PATH	"./1st/1stboot_hd"
#define BOOT2_PATH	"./2nd/2ndboot"
#endif

#define TABLE_FILE_PATH	"./boottable"

#define BOOT2_GDT_SIZE	(2 * PAGE_SIZE)
#define PAGE_TABLE_SIZE	(5 * PAGE_SIZE)


struct boot2_information
{
  int	size;
  /* etc... */
} boot2_info; /* = { 44 }; */


char	block[BLOCK_SIZE];
char	*boot1_path;
char	*boot2_path;

int
main (int ac, char **av)
{
  int	fd;
  int	total;

  if (ac < 4)
    {
      fprintf (stderr, "usage: %s 1stboot 2ndboot image\n", av[0]);
      return 0;
    }

  boot1_path = av[1];
  boot2_path = av[2];

  fd = open (av[3], O_CREAT | O_WRONLY, 0666);
  if (fd < 0)
    {
      fprintf (stderr, "can't open image file.\n");
      return 0;
    }
  printf ("***********************> start build!\n");
  fflush (stdout);
  
  write_1stboot (fd);
  write_2ndboot_inf (fd, &boot2_info);
  total = write_2ndboot_gdt (fd);
  total += write_page_table (fd);
  total += write_2ndboot_body (fd);
  boot2_info.size = (total + (BLOCK_SIZE-1)) / BLOCK_SIZE;
/*  boot2_info.size *= 2; */
#ifdef notdef
  lseek (fd, 1024, 0);
#else
  lseek (fd, 512, 0);
#endif
  write_2ndboot_inf (fd, &boot2_info);
  close (fd);

  return 0;
}

/***************************************************************************
 *
 */
write_1stboot (int fd)
{
  int	boot1;

  boot1 = open (boot1_path, O_RDONLY);
  if (boot1 < 0)
    {
      fprintf (stderr, "can't open 1stboot image.\n");
      exit (-1);
    }

  fputs ("1stboot...", stdout);
  fputs ("reading...", stdout);	fflush (stdout);
  read (boot1, block, sizeof (block));
  close (boot1);
  fputs ("writing...", stdout);	fflush (stdout);
  write (fd, block, sizeof (block));
/*  bzero (block, sizeof (block));
  write (fd, block, sizeof (block));
*/
  puts ("wrote.");
}

/*****************************************************************************
 *
 */
write_2ndboot_inf (int fd, struct boot2_information *boot2_info)
{
  copy_block (block, boot2_info, sizeof (struct boot2_information));
  write (fd, block, sizeof (block));
/*  bzero (block, sizeof (block));
  write (fd, block, sizeof (block));
 */
  printf ("2ndboot information wrote. size is = %d\n", boot2_info->size);
}

/*****************************************************************************
 *
 */
write_2ndboot_gdt (int fd)
{
  int	count;
  int	infd;
  int	total_size;
  
  bzero (block, sizeof (block));
  infd = open (TABLE_FILE_PATH, O_RDONLY);
  if (infd < 0)
    {
      fprintf (stderr, "can't open table file.\n");
      exit (-1);
    }
  total_size = 0;
  while ((count = read (infd, block, sizeof (block))) > 0)
    {
      write (fd, &block, sizeof (block));
      total_size += count;
    }
  printf ("2nd boot GDT information wrote (%d bytes).\n", total_size);
  return (total_size);
}

/*****************************************************************************
 *
 */
write_page_table (int fd)
{
  int	count;

  bzero (block, sizeof (block));
  for (count = 0; count < PAGE_TABLE_SIZE / sizeof (block); count++)
    {
      write (fd, &block, sizeof (block));
    }
  printf ("page table information wrote (%d bytes).\n",
	  count * sizeof (block));
  return (count * sizeof (block));
}      

/***************************************************************************
 *
 */
write_2ndboot_body (int fd)
{
  int	boot2;
  int	count;
  int	total_size;
  
  boot2 = open (boot2_path, O_RDONLY);
  if (boot2 < 0)
    {
      fprintf (stderr, "can't open 2ndboot image.\n");
      exit (-1);
    }

  for (total_size = 0;
       (count = read (boot2, block, sizeof (block))) > 0;
       total_size += count)
    {
      write (fd, block, count);
    }
  close (boot2);
  printf ("boot2 wrote (%d bytes).\n", total_size);
  return (total_size);
}

/*************************************************************************
 *
 */
copy_block (char *dest, char *src, int size)
{
  while (size-- > 0)
    {
      *dest++ = *src++;
    }
}

/************************** E N D   O F   F I L E ************************/
