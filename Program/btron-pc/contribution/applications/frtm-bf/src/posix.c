/*
   FoRTh Modoki
   posix.c ver 1.3.6
   Copyright (C) 1999, 2000 Tomohide Naniwa. All rights reserved.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Last modified: May 29, 2000.
*/

#include "stdlib.h"
#include "frtm.h"

void f_cat ()
{
  int fd;
  static struct stat st;
  static char buf[512];
  int length, rlength;
  int i;
  int offset = 0;
  char file[100];

  pop_string(file); EC;
  fd = open(file, 0, 0);
  if (fd < 0)
    {
      printf ("cannot open file. [%d](%s)\n", ERRNO, file);
      return;
    }

  if (stat(fd, &st))
    {
      printf ("cannot stat for file. [%d]\n", ERRNO);
      close(fd);
      return;
    }

  if (st.st_size < 0)
    {
      printf ("file size is 0.\n");
      close(fd);
      return;
    }

  printf ("file size is = %d\n", st.st_size);
  for (rlength = st.st_size; rlength > 0; rlength -= length)
    {
      if ((length = read (fd, buf, sizeof (buf))) <= 0)
	{
	  break;
	}

      for (i = 0; i < length; i++)
	{
	  putc (buf[i], stdout);
	}
    }

  printf ("\nread done\n");
  close (fd);
}


void f_dir (void)
{
  int fd;
  static struct stat st;
  int length;
  int i;
  char dirname[256];
#ifdef notdef
#define SFS_MAXNAMELEN			14
  struct sfs_dir {
    unsigned int sfs_d_index;		/* inode 番号 */
    char sfs_d_name[SFS_MAXNAMELEN];
    char pad[2];			/* padding */
  } *sfsp, *buf;
#else
  char buf[512];
  struct dirent *dp;
#endif
  
  pop_string(dirname); EC;

  if (dirname[0] == 0) {
    printf("Directory name is empty\n");
    return;
  }
  fd = open (dirname, 0, 0);
  if (fd < 0)
    {
      printf ("cannot open file. [%d](%s)\n", ERRNO, dirname);
      close(fd);
      return;
    }
#ifdef notdef
  if (stat (fd, &st) < 0)
    {
      printf ("cannot stat for file. [%d]\n", ERRNO);
      close(fd);
      return;
    }

  if (st.st_size < 0)
    {
      printf ("file size is 0.\n");
      close(fd);
      return;
    }

  printf ("file size is = %d\n", st.st_size);
  buf = alloca (st.st_size);
  if (buf == NULL)
    {
      printf ("can not allocate memory.\n");
      close(fd);
      return;
    }

  if ((length = read (fd, buf, st.st_size)) <= 0)
    {
      printf ("cannot read directory. [%d]\n", ERRNO);
      close(fd);
      return;
    }

  printf ("read size = %d\n", length);
  for (i = 0; i < length / sizeof (struct sfs_dir); i++)
    {
      int j;

      sfsp = (struct sfs_dir *)&buf[i];
      for (j = 0; j < SFS_MAXNAMELEN; j++)
	{
	  if (sfsp->sfs_d_name[j] == 0)
	    break;
	  putchar (sfsp->sfs_d_name[j]);
	}
      printf ("\n");
    }
#else
  while((length = getdents(fd, buf, 512)) > 0) {
    for(i = 0; i < length; i += dp->d_reclen) {
      dp = (struct dirent *) (buf+i);
      printf("%s\n", dp->d_name);
    }
  }
#endif

  close (fd);
}

void f_open(void)
{
  int fd;
  char fname[256];
  int oflag;

  oflag = pop(); EC;
  pop_string(fname); EC;

  fd = open (fname, oflag, 0666);
  if (fd < 0) {
    printf("open failed. [%d](%s)\n", ERRNO, fname);
  }

  push(fd); EC;
}

void f_close(void)
{
  int fd;
  
  fd = pop(); EC;
  if (fd < 3) {
    printf("Illegal file descriptor %d\n", fd);
  }
  else {
    close(fd);
  }
}

void f_write(void)
{
  char str[256];
  int fd, res;

  fd = pop(); EC;
  pop_string(str); EC;

  res = write (fd, str, strlen(str));
  if (res < 0) {
    printf("write failed. [%d]\n", ERRNO);
  }
}

void f_unlink(void)
{
  int result;

  char str[256];
  pop_string(str); EC;
  result = unlink(str);
  if (result < 0) {
    printf("unlink failed. [%d](%s)\n", ERRNO, str);
  }
}

void f_mkdir(void)
{
  char fname[256];
  int result;

  pop_string(fname); EC;

  result = mkdir (fname, 0777);
  if (result < 0) {
    printf("mkdir failed. [%d](%s)\n", ERRNO, fname);
  }
}

void f_rmdir(void)
{
  int result;
  char str[256];

  pop_string(str); EC;
  result = rmdir(str);
  if (result < 0) {
    printf("rmdir failed. [%d](%s)\n", ERRNO, str);
  }
}

void f_chdir(void)
{
  int result;
  char str[256];

  pop_string(str); EC;
  result = chdir(str);
  if (result < 0) {
    printf("chdir failed. [%d](%s)\n", ERRNO, str);
  }
}

void f_exec(void)
{
  char str[256];
  int pid, res;

  pop_string(str); EC;
  if ((res = fork()) == 0) {
    execve(str, NULL, NULL);
    _exit(1);
  }
  else if (res == -1) {
    printf("fork error\n");
  }
  else {
    printf("waiting pid=%d ...\n", res);
    pid = waitpid(-1, &res, 0);
    printf("child process exited pid=%d stat=%d\n", pid, res);
  }
}

void f_multi(void)
{
  int fd, i, cnt = 0, res;
  f_gm();
  winmod(1, 1);
  if ((res = fork()) == 0) {
    close(0);
    close(1);
    close(2);
    fd = open("/dev/win1", 2, 0666);
    if (fd < 0) _exit(-1);
    dup(fd);
    dup(fd);
    execve("/frtm", NULL);
    _exit(1);
  }
  else if (res == -1) {
    printf("fork error\n");
  }
  else {
    ++cnt;
  }
  if ((res = fork()) == 0) {
    close(0);
    close(1);
    close(2);
    fd = open("/dev/win2", 2, 0666);
    if (fd < 0) _exit(-1);
    dup(fd);
    dup(fd);
    execve("/frtm", NULL);
    _exit(1);
  }
  else if (res == -1) {
    printf("fork error\n");
  }
  else {
    ++cnt;
  }
  for(i = 0; i < cnt; ++i) {
    waitpid(-1, &res, 0);
  }
  f_tm();
}


void f_single(void)
{
  int fd, i, cnt = 0, res;
  if ((res = fork()) == 0) {
    close(0);
    close(1);
    close(2);
    fd = open("/dev/console", 2, 0666);
    if (fd < 0) _exit(-1);
    dup(fd);
    dup(fd);
    execve("/frtm", NULL);
    _exit(1);
  }
  else if (res == -1) {
    printf("fork error\n");
  }
  else {
    ++cnt;
  }
  for(i = 0; i < cnt; ++i) {
    waitpid(-1, &res, 0);
  }
}

static int bfd = -1;
static int bsize = 0;
static int boffset = 0;
#define BUFFSIZE 512
#define C(x) (x & 0x1f)

void bclose(void)
{
  close(bfd);
  bfd = -1;
}

int bopen(char *name)
{
  if (bfd > 0) bclose();
  bfd = open (name, O_RDWR, 0666);
  bsize = 0;
  boffset = 0;
  return(bfd);
}

int bget()
{
  static char buf[BUFFSIZE];

  if ((bsize == 0 && boffset == 0) ||
      (boffset == BUFFSIZE)) {
    bsize = read(bfd, buf, BUFFSIZE);
    boffset = 0;
    if (bsize == 0) return(-1);
  }
  else if (boffset == bsize) return(-1);
  return(buf[boffset++]);
}

void f_load(void)
{
  char str[256];
  int c, i = 0, flag = 0;

  pop_string(str); EC;
  if (bopen(str) < 0) {
    printf("open failed. %s\n", str);
    return;
  }
  while (1) {
    c = bget();
    if (c == -1) {
      flag = 2;
    }
    else if (c == C('m') || c == C('j')) {
      flag = 1;
    }
    else {
      str[i++] = c;
      if (i == 255) flag = 1;
    }
    if (flag > 0) {
      str[i] = 0;
      if ((str[0] != '#') && (i != 0)) {
	eval(str); EC;
      }
      if (flag == 2) break;
      flag = 0;
      i = 0;
    }
  }
  bclose();
}
