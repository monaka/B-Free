#include "../init.h"
#include "lowlib.h"
#include "CellMemory.h"
#include "mitten-command.h"

W
posix_ls (W ac, B **av)
{
  W			fd;
  static struct stat	st;


  fd = open (av[1], O_RDONLY, 0);
  if (fd < 0)
    {
      printf ("cannot file open.\n");
      return (fd);
    }

  if (stat (fd, &st))
    {
      printf ("cannot stat for file.\n");
      return (fd);
    }

  if (st.st_size < 0)
    {
      printf ("file size is 0.\n");
      return (0);
    }

  if (st.st_mode & FS_FMT_REG)
    {
      printf ("%s %d/%d %d bytes\n", av[1], st.st_uid, st.st_gid, st.st_size);
    }
  else if (st.st_mode & FS_FMT_DIR)
    {
      
    }

  close (fd);

  return E_OK;
}


ER
posix_cat (W ac, B **av)
{
  W			fd;
  static struct stat	st;
  static B		buf[1024];
  W			length, rlength;
  W			i;
  W			offset = 0;


  fd = open (av[1], O_RDONLY, 0);
  if (fd < 0)
    {
      printf ("cannot file open.\n");
      return (fd);
    }

  if (stat (fd, &st))
    {
      printf ("cannot stat for file.\n");
      return (fd);
    }

  if (st.st_size < 0)
    {
      printf ("file size is 0.\n");
      return (0);
    }

  if (ac > 2)
    {
      offset = atoi (av[2]);
      if (offset > st.st_size)
	{
	  printf ("offset overflow\n");
	  return (E_PAR);
	}

      printf ("lseek: offset = %d, mode = %d\n", offset, 0);
      if (lseek (fd, offset, 0))
	{
	  printf ("cannot lseek.\n");
	  return (E_SYS);
	}
      printf ("lseek success.\n");
    }

  printf ("file size is = %d\n", st.st_size);
  for (rlength = st.st_size - offset; rlength > 0; rlength -= length)
    {
      printf ("read: %d, 0x%x, %d\n", fd, buf,sizeof (buf));
      if ((length = read (fd, buf, sizeof (buf))) <= 0)
	{
	  break;
	}

      printf ("read size = %d\n", length);
      for (i = 0; i < length; i++)
	{
	  putc (buf[i], stdout);
	}
    }

  printf ("\nread done\n");
  close (fd);

  return E_OK;
}


ER
posix_dir (void)
{
  W			fd;
  static struct stat	st;
  struct sfs_dir	*buf;
  W			length;
  W			i;
  Cell			*c;
  B			dirname[255];
  ER			result;

#define SFS_MAXNAMELEN			14

  struct sfs_dir
    {
      UW	sfs_d_index;		/* inode ÈÖ¹æ */
      B	sfs_d_name[SFS_MAXNAMELEN];
      B	pad[2];			/* padding */
    } *sfsp;

  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_ToString(dirname, c, 255);
  MITN_ERROR_CHECK(result);
  
  fd = posix_opendir (dirname);
  if (fd < 0)
    {
      printf ("cannot file open.\n");
      return (fd);
    }

  if (stat (fd, &st) < 0)
    {
      printf ("cannot stat for file.\n");
      return (fd);
    }

  if (st.st_size < 0)
    {
      printf ("file size is 0.\n");
      return (0);
    }

  printf ("file size is = %d\n", st.st_size);
  buf = alloca (st.st_size);
  if (buf == NULL)
    {
      printf ("can not allocate memory.\n");
      return (1);
    }

  if ((length = posix_readdir (fd, buf, st.st_size)) <= 0)
    {
      printf ("cannot read directory.\n");
      return (1);
    }

  printf ("read size = %d\n", length);
  for (i = 0; i < length / sizeof (struct sfs_dir); i++)
    {
      W	j;

      sfsp = (struct sfs_dir *)&buf[i];
      for (j = 0; j < SFS_MAXNAMELEN; j++)
	{
	  if (sfsp->sfs_d_name[j] == 0)
	    break;
	  putchar (sfsp->sfs_d_name[j]);
	}
      printf ("\n");
    }

  posix_closedir (fd);

  return E_OK;
}


ER
p_open(void)
{
  W fd;
  Cell *c;
  B fname[255];
  ER result;
  W oflag;
  
  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&oflag, c);
  MITN_ERROR_CHECK(result);

  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_ToString(fname, c, 255);
  MITN_ERROR_CHECK(result);

  fd = open (fname, oflag, 0666);
  if (fd < 0)
    {
      printf("open failed. %s\n", fname);
      return E_SYS;
    }

  result = MitnCell_GetCellAsLong(&c, fd);
  MITN_ERROR_CHECK(result);
  result = MitnStack_Push(DATA_STACK, c);
  MITN_ERROR_CHECK(result);

  return E_OK;
}


ER
p_close(void)
{
  Cell *c;
  long fd;
  ER result;
  
  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&fd, c);
  MITN_ERROR_CHECK(result);

  close(fd);

  return E_OK;
}

ER
p_write(void)
{
  Cell *c;
  B str[255];
  long fd;
  ER result;
  
  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_GetLongFromCell(&fd, c);
  MITN_ERROR_CHECK(result);
  result = MitnStack_Pop(DATA_STACK, &c);
  MITN_ERROR_CHECK(result);
  result = MitnCell_ToString(str, c, 255);
  MITN_ERROR_CHECK(result);

  if (write (fd, str, strlen(str)) < 0)
    {
      return E_SYS;
    }

  return E_OK;
}


ER
posix_writefile (W ac, B **av)
{
  W	fd;

  fd = open (av[1], O_CREAT|O_RDWR, 0666);
  if (fd < 0)
    {
      printf ("cannot open file. %s\n", av[1]);
      return E_SYS;
    }

  if (write (fd, av[2], strlen(av[2])) < 0)
    {
      return E_SYS;
    }
  
  close(fd);
  return E_OK;
}


