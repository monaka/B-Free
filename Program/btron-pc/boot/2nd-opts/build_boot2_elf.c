/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
#define __BUILD_BOOT2_ELF_C__

#include <stdio.h>
#ifdef __LINUX__
#include <linux/types.h>
#include <fcntl.h>
#else /* not __LINUX__ */
#include <sys/types.h>
#include <fcntl.h>
#endif /* not __LINUX__ */

#include <sys/stat.h>
#include <unistd.h>
#include <malloc.h>

//#include "protdef.h"


#define MINIX_HEADER		32
#define GCC_HEADER		32

#define ALIGN_SIZE		4096
#define BUF_SIZE		ALIGN_SIZE

#define ROUNDUP(x,align)	(((x + (align - 1)) / align) * align)


#define EI_NIDENT	16


typedef	unsigned long	ELF32_Addr;
typedef unsigned short	ELF32_Half;
typedef unsigned long	ELF32_Off;
typedef unsigned long	ELF32_Sword;
typedef unsigned long	ELF32_Word;


#define PT_NULL		0
#define PT_LOAD		1
#define PT_DYNAMIC	2
#define PT_INTERP	3
#define PT_NONE		4
#define PT_SHLIB	5
#define PT_PHDR		6
#define PT_LOPROC	0x70000000
#define PT_HIPROC	0x7fffffff

#define PF_R		0x4
#define PF_W		0x2
#define PF_X		0x1


struct ELFheader
{
  unsigned char		e_ident[EI_NIDENT];
  ELF32_Half		e_type;
  ELF32_Half		e_machine;
  ELF32_Word		e_version;
  ELF32_Addr		e_entry;
  ELF32_Off		e_phoff;
  ELF32_Off		e_shoff;
  ELF32_Word		e_flags;
  ELF32_Half		e_ehsize;
  ELF32_Half		e_phentsize;
  ELF32_Half		e_phnum;
  ELF32_Half		e_shentsize;
  ELF32_Half		e_shnum;
  ELF32_Half		e_shstrndx;
};



struct Pheader
{
  ELF32_Word		p_type;
  ELF32_Off		p_offset;
  ELF32_Addr		p_vaddr;
  ELF32_Addr		p_paddr;
  ELF32_Word		p_filesz;
  ELF32_Word		p_memsz;
  ELF32_Word		p_flags;
  ELF32_Word		p_align;
};


extern void		*read_minix_efile (char *fname, int *size);
extern void		*read_elf_efile (char *fname, int *size, ELF32_Word type, ELF32_Word flag, ELF32_Addr *addr);
extern struct Pheader	*read_elf_pheader (int fd, struct ELFheader *elfh, int *ent);
extern struct ELFheader	*read_elf_header (int fd);

extern void		pad_write (int outfd, int size);



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
  int		fd;
  void		*buf;
  ELF32_Addr	text_vaddr, data_vaddr;
  int		text_size, data_size;
  int		size;


  if (ac < 3)
    {
      fprintf (stderr, "usage: %s 16bit-boot 32bit-boot\n", av[0]);
      exit (0);
    }

  buf = read_minix_efile (av[1], &size);
  if (buf == NULL)
    {
      fprintf (stderr, "can't open %s for 16bit-mode boot\n", av[1]);
      exit (1);
    }

  if (write (1, buf, size) < 0)
    {
      perror ("write");
      exit (0);
    }

  free (buf);

  if (size % ALIGN_SIZE)
    {
      pad_write (1, ALIGN_SIZE - (size % ALIGN_SIZE));
    }

  /* Text 領域の読み取り */
  buf = read_elf_efile (av[2], &text_size, PT_LOAD, PF_R | PF_X, &text_vaddr);
  if (buf == NULL)
    {
      fprintf (stderr, "can't open %s for 32bit-mode boot\n", av[2]);
      exit (1);
    }

  if (write (1, buf, text_size) < 0)
    {
      perror ("write");
      exit (0);
    }
  fprintf (stderr, "text section write vaddr = 0x%x, size = %d bytes\n",
	   text_vaddr, text_size);
  free (buf);

	     
  /* Data 領域の読み取り */
  buf = read_elf_efile (av[2], &data_size, PT_LOAD, PF_R | PF_W, &data_vaddr);
  if (buf == NULL)
    {
      fprintf (stderr, "can't open %s for 32bit-mode boot\n", av[2]);
      exit (1);
    }
  pad_write (1, (data_vaddr - text_vaddr) - text_size);
  fprintf (stderr, "pad write %d\n", (data_vaddr - text_vaddr) - text_size);

  if (write (1, buf, data_size) < 0)
    {
      perror ("write");
      exit (0);
    }
  fprintf (stderr, "data section write vaddr = 0x%x, size = %d bytes\n",
	   data_vaddr, data_size);

  free (buf);
  exit (0);
}

/*
 */
void *
read_minix_efile (char *fname, int *size)
{
  int	fd;
  void	*buf;
  struct stat	st;

  fd = open (fname, O_RDONLY);
  if (fd < 0)
    {
      perror ("open");
      exit (1);
    }

  if (fstat (fd, &st) < 0)
    {
      perror ("fstat");
      exit (1);
    }

  buf = malloc (st.st_size);
  if (buf == NULL)
    {
      perror ("malloc");
      exit (1);
    }

  if (lseek (fd, MINIX_HEADER, SEEK_SET) < 0)
    {
      perror ("lseek");
      exit (1);
    }

  if (read (fd, buf, st.st_size - MINIX_HEADER) < 0)
    {
      perror ("read");
      exit (1);
    }
  
  *size = st.st_size - MINIX_HEADER;
  return (buf);
}

/*
 * ELF ファイルを読み込む。
 */
void *
read_elf_efile (char *fname, int *size, ELF32_Word type, ELF32_Word flag, ELF32_Addr *vaddr)
{
  struct ELFheader	*elfhp;
  struct Pheader	*ph;
  int			ent;
  int			i;
  int			section_size;
  void			*buf;
  int			fd;

  fd = open (fname, O_RDONLY);
  if (fd < 0)
    {
      perror ("open");
      exit (1);
    }

  elfhp = read_elf_header (fd);
  if (elfhp == NULL)
    {
      close (fd);
      fprintf (stderr, "Cannot read elf file.\n");
      exit (1);
    }

  ph = read_elf_pheader (fd, elfhp, &ent);
  if (ph == NULL)
    {
      free (elfhp);
      close (fd);
      fprintf (stderr, "Cannot read exec header.\n");
      exit (1);
    }

  for (i = 0; i < ent; i++)
    {
#ifdef nodef
      fprintf (stderr, "ph[%d].p_type = 0x%x, ph[%d].p_flags = 0x%x\n",
	       i, ph[i].p_type,
	       i, ph[i].p_flags);
#endif
      if ((ph[i].p_type == type) && (ph[i].p_flags == flag))
	{
	  break;
	}
    }

  if (i >= ent)
    {
      /* 条件に合致した program header が見つからなかった */
      free (ph);
      free (elfhp);
      fprintf (stderr, "Not found program header\n");
      close (fd);
      exit (1);
    }

  fprintf (stderr, "ELF: type = 0x%x, flag = 0x%x, vaddr = 0x%x, mem size = %d, file size = %d\n",
	   ph[i].p_type,
	   ph[i].p_flags,
	   ph[i].p_vaddr,
	   ph[i].p_memsz,
	   ph[i].p_filesz);

  *size = ph[i].p_memsz;
  *vaddr = ph[i].p_vaddr;
  buf = calloc (ph[i].p_memsz, 1);
  if (buf == NULL)
    {
      free (ph);
      free (elfhp);
      close (fd);
      perror ("malloc");
      exit (1);
    }

  if (lseek (fd, ph[i].p_offset, SEEK_SET) < 0)
    {
      free (ph);
      free (elfhp);
      close (fd);
      perror ("lseek");
      exit (1);
    }

  if (read (fd, buf, ph[i].p_filesz) < 0)
    {
      free (ph);
      free (elfhp);
      close (fd);
      perror ("read");
      exit (1);
    }

  free (ph);
  free (elfhp);
  close (fd);

  return (buf);
}

/*
 */
struct ELFheader *
read_elf_header (int fd)
{
  struct ELFheader *elfp;

  elfp = (struct ELFheader *)malloc (sizeof (struct ELFheader));
  if (elfp == NULL)
    {
      perror ("malloc");
      exit (1);
    }

  if (lseek (fd, 0, 0) < 0)
    {
      free (elfp);
      perror ("lseek");
      exit (1);
    }

  if (read (fd, elfp, sizeof (struct ELFheader)) < 0)
    {
      free (elfp);
      perror ("read");
      exit (1);
    }

  return (elfp);
}


/*
 */
struct Pheader *
read_elf_pheader (int fd, struct ELFheader *elfh, int *ent)
{
  struct Pheader	*phbuf;

  phbuf = (struct Pheader *)malloc (elfh->e_phentsize * elfh->e_phnum);
  lseek (fd, elfh->e_phoff, SEEK_SET);
  if (read (fd, phbuf, elfh->e_phentsize * elfh->e_phnum) < 0)
    {
      perror ("read");
      free (phbuf);
      exit (0);
    }

  *ent = elfh->e_phnum;
  return (phbuf);
}


/* ========================================================================= */

/*
 */
void
pad_write (int outfd, int size)
{
  void	*buf;

  fprintf (stderr, "pad_write: size = %d\n", size);
  buf = calloc (size, 1);
  if (buf == NULL)
    {
      perror ("malloc");
      exit (1);
    }

  if (write (outfd, buf, size) < 0)
    {
      perror ("write");
      exit (1);
    }

  free (buf);
}

