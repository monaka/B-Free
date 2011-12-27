/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

#include <stdio.h>
#include <string.h>
#include <a.out.h>
#include <sys/file.h>

#include "../../ITRON/h/itron_module.h"	/* module_info ��¤�Τλ��ȤΤ��� */



#define	MAX_LINE	1000
#define ALIGN(x,al)	((((int)x) + (al) - 1) & ~(al - 1))
#define BLOCKSIZE	1024
#define PAGESIZE	4096


#ifdef DEBUG

#define DPRINTF(x)	printf x;

#else

#define DPRINTF(x)

#endif



struct config_entry
{
  char			*fname;
  int			offset;
  struct module_info	mod_info;
};


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


  
char			*cmd;
int			nmodule = 0;
struct config_entry	module_table[MAX_MODULE];


void
usage (void)
{
  fprintf (stderr, "Usage: %s configuration-file output-file\n");
}


int
main (int ac, char **av)
{
  int		i;


  cmd = av[0];

  if (ac < 2)
    {
      usage ();
      exit (0);
    }


  /* pass 0 -	configuration file ���ɤ߹��� 
   * 		module_table[] ������
   */
  pass0 (av[1]);
  if (nmodule <= 0)
    {
      fprintf (stderr, "No module.\n");
      exit (1);
    }


  /* pass 1 -	module_table[] ���Ȥˡ��ƥ⥸�塼���
   *		������ɤ߼�롣�ɤ߼�ä������Ȥä�
   *		module_table �� mod_info ���Ǥ����롣
   */
  pass1 (module_table);

  fprintf (stderr, "modinfo: \n");
  for (i = 0; i < nmodule; i++)
    {
      fprintf (stderr, "%02d: %-20.20s  vaddr = 0x%08x, paddr = 0x%08x, entry = 0x%08x, memsize = %6d, file size = %6d\n",
	       i,
	       module_table[i].mod_info.name,
	       module_table[i].mod_info.vaddr,
	       module_table[i].mod_info.paddr,
	       module_table[i].mod_info.entry,
	       module_table[i].mod_info.mem_length,
	       module_table[i].mod_info.length);
    }

  /* pass 2 -	�ǽ�Ū�� OS �ե��������Ϥ��롣
   */
  pass2 (module_table, av[2]);


  exit (0);
}

/* pass0 - configuration file ���ɤ߼��
 *
 *	   configuration file �Υե����ޥåȤϼ��Τ褦�ˤʤäƤ��롣
 *	   (�ƥ���ȥ�϶���ޤ��� tab �Ƕ��ڤäƤ���)
 *
 *		<�⥸�塼��ե�����>	<���ۥ��ɥ쥹>	<ʪ�����ɥ쥹>	<����>	<�⥸�塼��̾>
 *
 */
pass0 (char *config)
{
  FILE	*fp;
  char	line[MAX_LINE], *p;
  int	i, mod_index;
  char	*path, *vaddr, *paddr, *type, *mod_name;

  fp = fopen (config, "r");
  if (fp == NULL)
    {
      perror ("fopen");
      exit (1);
    }

  for (i = 0, mod_index = 0; fgets (line, MAX_LINE, fp); i++)
    {
      if ((line[0] == '#') || (line[0] == '\n'))
	{
	  continue;
	}

      for (p = line; *p != '\0'; p++)
	{
	  if (*p != ' ') 
	    {
	      break;
	    }
	}
      if ((*p == '\0') || (*p == '\n'))
	{
	  /* ����Ԥ��ä� */
	  continue;
	}

      /* �⥸�塼��ե�����̾���ɤ߼�� */
      path = strtok (line, " \t\n");
      vaddr = strtok (NULL,  " \t\n");
      paddr = strtok (NULL,  " \t\n");
      type = strtok (NULL,  " \t\n");
      mod_name = strtok (NULL,  " \t\n");

      if (strlen (mod_name) > MAX_MODULE_NAME)
	{
	  fprintf (stderr, "module name too long in %d\n", i);
	  exit (1);
	}

      DPRINTF (("mod[%d]: %-30.30s, vaddr = %10.10s, paddr = %10.10s, type = %-8.8s, module = %s\n", 
		mod_index, path, vaddr, paddr, type, mod_name));

      if ((path == NULL) || (vaddr == NULL) || (paddr == NULL) || (type == NULL) || (mod_name == NULL))
	{	
	  fprintf (stderr, "syntax error in %d\n", i);
	  exit (1);
	}

      /* module_table ������ */
      module_table[mod_index].fname = strdup (path);
      sscanf (vaddr, "0x%x", &(module_table[mod_index].mod_info.vaddr));
      if (vaddr <= 0)
	{
	  fprintf (stderr, "syntax error in %d\n", i);
	  exit (1);
	}

      sscanf (paddr, "0x%x", &(module_table[mod_index].mod_info.paddr));
      if (paddr <= 0)
	{
	  fprintf (stderr, "syntax error in %d\n", i);
	  exit (1);
	}

      if (strcmp (type, "kernel") == 0)
	{
	  module_table[mod_index].mod_info.type = kernel;
	}
      else if (strcmp (type, "user") == 0)
	{
	  module_table[mod_index].mod_info.type = user;
	}
      else if (strcmp (type, "lowlib") == 0)
	{
	  module_table[mod_index].mod_info.type = lowlib;
	}
      else if (strcmp (type, "server") == 0)
	{
	  module_table[mod_index].mod_info.type = server;
	}
      else if (strcmp (type, "driver") == 0)
	{
	  module_table[mod_index].mod_info.type = driver;
	}
      else
	{
	  fprintf (stderr, "Unknown module type: %s in %d\n", type, i);
	  exit (1);
	}
      strncpy (module_table[mod_index].mod_info.name, mod_name, MAX_MODULE_NAME);

      DPRINTF (("mod[%d]: %-30.30s, vaddr = 0x%08.08x, paddr = 0x%08.08x, type = %d, module = %s\n", 
		mod_index,
		module_table[mod_index].fname,
		module_table[mod_index].mod_info.vaddr,
		module_table[mod_index].mod_info.paddr,
		module_table[mod_index].mod_info.type,
		module_table[mod_index].mod_info.name));

      mod_index++;
    }

  nmodule = mod_index;
}


/* pass1 - �ƥ⥸�塼��μ¹ԥե�����ξ�����ɤ߼��
 */
int
pass1 (struct config_entry *info_table)
{
  int			i, j, phindex;
  FILE			*fp;
  struct ELFheader	elf;
  struct Pheader	*phead;


  for (i = 0; i < nmodule; i++)
    {
      /* �¹ԥե�����Υ����ץ�
       */
      fp = fopen (info_table[i].fname, "r");
      if (fp == NULL)
	{
	  fprintf (stderr, "-- %s --\n", info_table[i].fname);
	  perror ("fopen");
	  exit (1);
	}

      /* ELF �إå����ɤ߼�� */
      if (fread (&elf, sizeof (elf), 1, fp) <= 0)
	{
	  perror ("fread");
	  exit (1);
	}

      /* �ץ����إå����ɤ߼�� */
      phead = (struct Pheader *)malloc (elf.e_phentsize * elf.e_phnum);
      if (phead == NULL)
	{
	  perror ("malloc");
	  exit (1);
	}
      if (fseek (fp, elf.e_phoff, SEEK_SET) == -1)
	{
	  perror ("fseek");
	  exit (1);
	}
      if (fread (phead, elf.e_phentsize, elf.e_phnum, fp) < elf.e_phnum)
	{
	  perror ("fread");
	  exit (1);
	}

      /* info_table �Υ���ȥ������ */
      info_table[i].mod_info.entry = elf.e_entry;

      /* �ե������Υ����� */
#ifdef notdef
      info_table[i].mod_info.length = phead[elf.e_phnum - 1].p_offset 
                                    + ALIGN (phead[elf.e_phnum - 1].p_filesz, phead[elf.e_phnum - 1].p_align)
				    - phead[0].p_offset;
#else
      if (elf.e_phnum == 1)
	{
	  info_table[i].mod_info.length = ALIGN (phead[0].p_filesz, phead[0].p_align);
	  info_table[i].mod_info.length = ALIGN (info_table[i].mod_info.length, PAGESIZE);
	}
      else
	{
	  DPRINTF(("vaddr0 %x, offset %d filesz0 %d, vaddr1 %x offset %d filesz1 %d\n", phead[0].p_vaddr, phead[0].p_offset, phead[0].p_filesz, phead[1].p_vaddr, phead[1].p_offset, phead[1].p_filesz));
	  info_table[i].mod_info.length = phead[elf.e_phnum - 1].p_vaddr
   	                                - phead[0].p_vaddr 
		    	    	        + phead[elf.e_phnum - 1].p_filesz;
	  info_table[i].mod_info.length = ALIGN (info_table[i].mod_info.length, PAGESIZE);
	}
#endif

#ifdef notdef
      info_table[i].mod_info.mem_length = 0;
      info_table[i].mod_info.mem_length += ALIGN (phead[0].p_memsz, phead[0].p_align);
      for (j = 1; j < elf.e_phnum; j++)
	{
	  info_table[i].mod_info.mem_length += (phead[j].p_vaddr - (phead[j - 1].p_vaddr + ALIGN (phead[j - 1].p_memsz, phead[j - 1].p_align)));
	  DPRINTF (("gap: %d\n", (phead[j].p_vaddr - (phead[j - 1].p_vaddr + ALIGN (phead[j - 1].p_memsz, phead[j - 1].p_align)))));
	  info_table[i].mod_info.mem_length += ALIGN (phead[j].p_memsz, phead[0].p_align);
	}
      info_table[i].mod_info.mem_length = ALIGN (info_table[i].mod_info.mem_length, PAGESIZE);
#else
      if (elf.e_phnum == 1)
	{
	  info_table[i].mod_info.mem_length
	    = ALIGN (phead[0].p_memsz, phead[0].p_align);
	  info_table[i].mod_info.mem_length
	    = ALIGN (info_table[i].mod_info.mem_length, PAGESIZE);
	}
      else
	{
	  info_table[i].mod_info.mem_length = phead[elf.e_phnum - 1].p_vaddr
   	                                - phead[0].p_vaddr 
		    	    	        + phead[elf.e_phnum - 1].p_memsz;
	  info_table[i].mod_info.mem_length
	    = ALIGN (info_table[i].mod_info.mem_length, PAGESIZE);
	}
#endif

      info_table[i].offset = phead[0].p_offset;

      free (phead);
      fclose (fp);
    }

}



/* pass2 - OS �ե�����ν���
 */
int
pass2 (struct config_entry *info_table, char *outputfile)
{
  int			i;
  FILE			*fp, *outfp;
  struct exec		einfo;		/* a.out �����μ¹ԥե�����Υإå� */
  char			*buf;
  struct ELFheader	elf;
  struct Pheader	*phead;
  struct boot_header	boot;


  outfp = fopen (outputfile, "w");
  if (outfp == NULL)
    {
      perror ("fopen");
      exit (1);
    }
  
  bzero (&boot, sizeof (boot));
  boot.cookie = 1;
  boot.count = nmodule;
  boot.machine.rootfs = 0xffffffff;

  /* �إå��ν��� */
  if (fwrite (&boot, sizeof (boot), 1, outfp) < 1)
    {
      perror ("fwrite");
      fclose (outfp);
    }

  for (i = 0; i < nmodule; i++)
    {
      if (fwrite (&(info_table[i].mod_info), sizeof (struct module_info), 1, outfp) < 1)
	{
	  perror ("fwrite");
	  fclose (outfp);
	  exit (1);
	}
    }
  fseek (outfp, 1024, SEEK_SET);

  einfo.a_info = ZMAGIC;
  /* �⥸�塼��ҤȤĤҤȤĤν��� */
  for (i = 0; i < nmodule; i++)
    {
      buf = (char *)malloc (info_table[i].mod_info.length);
      if (buf == NULL)
	{
	  perror ("malloc");
	  fclose (outfp);
	  exit (1);
	}
      
      /* �¹ԥե�����Υإå������ (a.out ����)
       */
      einfo.a_text = info_table[i].mod_info.length;
      einfo.a_data = 0;
      einfo.a_bss = info_table[i].mod_info.mem_length - info_table[i].mod_info.length;
      einfo.a_entry = info_table[i].mod_info.entry;
      if (fwrite (&einfo, sizeof (einfo), 1, outfp) < 1)
	{
	  perror ("write");
	  fclose (outfp);
	  exit (1);
	}
      output_padding (outfp, BLOCKSIZE - sizeof (einfo));
      
      fp = fopen (info_table[i].fname, "r");
      if(fp == NULL)
	{
	  perror ("fopen");
	  fclose (outfp);
	  exit (1);
	}

      DPRINTF (("output[%d]: ", i));

      output_module (&info_table[i], fp, outfp);

      fclose (fp);
    }


  fclose (outfp);
}


output_module (struct config_entry *info, FILE *in, FILE *out)
{
  struct ELFheader	elf;
  struct Pheader	*phead;
  int			i;
  char			*buf;
  int			output_length;
  int			padding;


  if (fseek (in, 0, SEEK_SET) < 0)
    {
      perror ("fseek");
      exit (1);
    }

  /* ELF �إå����ɤ߼�� */
  if (fread (&elf, sizeof (elf), 1, in) <= 0)
    {
      perror ("fread");
      exit (1);
    }

  /* �ץ����إå����ɤ߼�� */
  phead = (struct Pheader *)malloc (elf.e_phentsize * elf.e_phnum);
  if (phead == NULL)
    {
      perror ("malloc");
      exit (1);
    }
  if (fseek (in, elf.e_phoff, SEEK_SET) == -1)
    {
      perror ("fseek");
      exit (1);
    }
  if (fread (phead, elf.e_phentsize, elf.e_phnum, in) < elf.e_phnum)
    {
      perror ("fread");
      exit (1);
    }

  /* Text ���ꥢ���ɤ߼��
   */
  if (fseek (in, phead[0].p_offset, SEEK_SET) < 0)
    {
      perror ("fseek");
      exit (1);
    }
  buf = (char *)malloc (phead[0].p_filesz);
  if (buf == NULL)
    {
      perror ("malloc");
      fclose (in);
      fclose (out);
      exit (1);
    }
  if (fread (buf, phead[0].p_filesz, 1, in) < 1)
    {
      perror ("fread");
      fclose (in);
      fclose (out);
      exit (1);
    }
  if (fwrite (buf, phead[0].p_filesz, 1, out) < 1)
    {
      perror ("fwrite");
      fclose (in);
      fclose (out);
      exit (1);
    }
  DPRINTF (("filesize: %d\n", info[0].mod_info.length));
  DPRINTF (("fwrite(TEXT): %d\n", phead[0].p_filesz));
  free (buf);
  output_length = phead[0].p_filesz;


  /* Data ���ꥢ���ɤ߼��
   */
  if (elf.e_phnum == 2)
    {
      /* padding �ν��� */
      padding = phead[1].p_vaddr - (phead[0].p_vaddr + phead[0].p_filesz);
      output_padding (out, padding);
      output_length += padding;

      DPRINTF (("Data area: 0x%x - 0x%x (size 0x%x (%d))\n",
		output_length, 
		output_length + phead[1].p_filesz,
		phead[1].p_filesz, phead[1].p_filesz));

      if (fseek (in, phead[1].p_offset, SEEK_SET) < 0)
	{
	  perror ("fseek");
	  exit (1);
	}
      buf = (char *)malloc (phead[1].p_filesz);
      if (buf == NULL)
	{
	  perror ("malloc");
	  fclose (in);
	  fclose (out);
	  exit (1);
	}
      if (fread (buf, phead[1].p_filesz, 1, in) < 1)
	{
	  perror ("fread: (2)");
	  fclose (in);
	  fclose (out);
	  exit (1);
	}
      DPRINTF (("fwrite(DATA): %d\n", phead[1].p_filesz));
      if (fwrite (buf, phead[1].p_filesz, 1, out) < 1)
	{
	  perror ("fwrite");
	  fclose (in);
	  fclose (out);
	  exit (1);
	}
      free (buf);
      output_length += phead[1].p_filesz;
      output_padding (out, ALIGN (output_length, PAGESIZE) - output_length);
    }
  else if (elf.e_phnum == 1)
    {
      DPRINTF(("only TEXT\n"));
      padding = ALIGN (phead[0].p_filesz, phead[0].p_align) - phead[0].p_filesz;
      output_padding (out, padding);
    }
  else
    {
      fprintf (stderr, "Too many program header.\n");
      exit (1);
    }
}


output_padding (FILE *outfp, int size)
{
  int	i;

  DPRINTF (("padding write: %d bytes\n", size));
  for (i = 0; i < size; i++)
    {
      fputc (0, outfp);
    }
}
