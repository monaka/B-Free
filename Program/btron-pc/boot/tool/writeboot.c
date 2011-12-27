#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <limits.h>
#include <linux/hdreg.h>
#include <errno.h>


/* 1stboot のイメージ中で HD のジオメトリ情報が入る場所 */
#define HDGEO_OFFSET	506

/* 1stboot のイメージ中で HD のパーティション情報が入る場所 */
#define HDPART_OFFSET	502

#define PARTITION_INFO_OFFSET 0x1be


/* 1st boot プログラムが使う HD のジオメトリ情報の形式
 */
struct boot_geo
{
  unsigned short	cylinder;
  unsigned short	head;
  unsigned char		sector;
};

struct hd_partition
{
  unsigned char		boot_flag;
  unsigned char		begin[3];
  unsigned char		type;
  unsigned char		end[3];	
  unsigned long		start;
  unsigned long		length;
};

struct write_information
{
  unsigned short	geo_cylinder;
  unsigned char		geo_head;
  unsigned char		geo_sector;

  unsigned short	boot_cylinder;
  unsigned char		boot_head;
  unsigned char		boot_sector;
};




int	verbose_flag;
int	write_flag;
char	*device;
char	*partition;
char	*bootimage;

extern void	read_partition_table (char *device, struct hd_partition *hd_part);



int
main (int ac, char **av)
{
  int	i;
  struct hd_geometry	geo;
  struct hd_partition	hd_part[4];
  struct write_information boot_info;


  if (ac < 3)
    {
      fprintf (stderr, "usage: %s [option] device partition\n", av[0]);
      fprintf (stderr, "       %s -w device partition bootimage\n", av[0]);
      exit (0);
    }

  for (i = 1; i < ac; i++)
    {
      if (strcmp (av[i], "-w") == 0)
	{
	  write_flag = 1;
	}
      else if  (strcmp (av[i], "-v") == 0)
	{
	  verbose_flag = 1;
	}
      else if (av[i][0] == '-')
	{
	  fprintf (stderr, "illegal option: %s\n", av[i]);
	  exit (1);
	}
      else
	{
	  break;
	}
    }

  if (i > ac - 1)
    {
      fprintf (stderr, "usage: %s [option] device partition\n", av[0]);
      fprintf (stderr, "       %s -w device partition bootimage\n", av[0]);
      exit (0);
    }

  if (geteuid () != 0)
    {
      fprintf (stderr, "Illegal user. Root user use only.\n");
      exit (0);
    }

  device = av[i];
  partition = av[i + 1];
  bootimage = av[i + 2];

  if (write_flag && (bootimage == NULL))
    {
      fprintf (stderr, "usage: %s -w device partition bootimage\n", av[0]);
      exit (0);
    }

  printf ("device: %s, partition: %s, bootimage: %s\n",
	  device, partition, bootimage ? bootimage : "null");

  get_geometry (device, partition, &geo);
  read_partition_table (device, hd_part);

  boot_info.geo_cylinder = (unsigned short)geo.cylinders;
  boot_info.geo_head     = (unsigned char)geo.heads;
  boot_info.geo_sector   = (unsigned char)geo.sectors;

  boot_info.boot_cylinder = (unsigned short)(hd_part[atoi(partition) - 1].begin[2] | ((hd_part[atoi(partition) - 1].begin[1] & 0xC0) << 2));
  boot_info.boot_head     = (unsigned char)(hd_part[atoi(partition) - 1].begin[0]);
  boot_info.boot_sector   = (unsigned char)(hd_part[atoi(partition) - 1].begin[1] & 0x3F);

  if (verbose_flag)
    {
      printf ("geometry:\n");
      printf ("\theads:     %d\n", boot_info.geo_head);
      printf ("\tcylinders: %d\n", boot_info.geo_cylinder);
      printf ("\tsectors:   %d\n", boot_info.geo_sector);

      printf ("start:\n");
      printf ("\theads:     %d\n", boot_info.boot_head);
      printf ("\tcylinders: %d\n", boot_info.boot_cylinder);
      printf ("\tsectors:   %d\n", boot_info.boot_sector);
    }

  if (write_flag)
    {
      write_boot_information (bootimage, &boot_info);
    }

  exit (0);
}


get_geometry (char *device, char *partition, struct hd_geometry *geo)
{
  int	partition_number;
  char	special_file_name[PATH_MAX];
  int	fd;

  partition_number = atoi (partition);
  if (partition_number <= 0)
    {
      fprintf (stderr, "Illegal partition number : %s\n", partition);
      exit (1);
    }

  sprintf (special_file_name, "%s%d", device, partition_number);
  fd = open (special_file_name, O_RDONLY);
  if (fd < 0)
    {
      fprintf (stderr, "Cannot open special file(%s).\n", special_file_name);
      exit (1);
    }

  printf ("%s\n", special_file_name);

  if (ioctl (fd, HDIO_GETGEO, geo) < 0)
    {
      fprintf (stderr, "Cannot get hd geometry. (%s)\n", strerror (errno));
      close (fd);
      exit (1);
    }

  close (fd);
}


void
read_partition_table (char *device, struct hd_partition *hd_part)
{

  int	fd;

  fd = open (device, O_RDONLY);
  if (fd < 0)
    {
      fprintf (stderr, "Cannot open special file(%s).\n", device);
      exit (1);
    }

  if (lseek (fd, PARTITION_INFO_OFFSET, SEEK_SET) < 0)
    {
      fprintf (stderr, "Cannot seek. (%s)\n", strerror (errno));
      close (fd);
      exit (1);
    }

  if (read (fd, hd_part, sizeof (struct hd_partition) * 4) < 0)
    {
      fprintf (stderr, "Cannot read partition table. (%s)\n", strerror (errno));
      close (fd);
      exit (1);
    }

  close (fd);  

#ifdef DEBUG
  {
    struct hd_partition *par;
    int	i;
    int	c, h, s;

    par = hd_part;
    for (i = 0; i < 4; i++)
      {
	c = par->begin[2] | ((par->begin[1] & 0xC0) << 2);
	h = par->begin[0];
	s = par->begin[1] & 0x3F;
	printf ("[%d] start = %7d, c/h/s = %4d/%4d/%4d  ", i, par->start, c, h, s);

	c = par->end[2] | ((par->end[1] & 0xC0) << 2);
	h = par->end[0];
	s = par->end[1] & 0x3F;
	printf ("-  c/h/s = %4d/%4d/%4d\n", c, h, s);
	par++;
      }
  }
#endif
}


write_boot_information (char *bootfile, struct write_information *info)
{
  int	fd;
  unsigned char	buf[512];
  unsigned short	*cyl;
  unsigned char		*sec;
  unsigned char		*head;


  fd = open (bootfile, O_RDWR);
  if (fd < 0)
    {
      fprintf (stderr, "Cannot open boot image. (%s)\n", strerror (errno));
      exit (1);
    }

  if (read (fd, buf, sizeof (buf)) != sizeof (buf))
    {
      fprintf (stderr, "Cannot open boot image file. (%s)\n", strerror (errno));
      close (fd);
      exit (1);
    }

  (unsigned char *)cyl = &buf[HDGEO_OFFSET];
  head = &buf[HDGEO_OFFSET + 2];
  sec = &buf[HDGEO_OFFSET + 3];

  if (verbose_flag)
    {
      printf ("old geometry information:\n");
      printf ("cylinder %d\n", *cyl);
      printf ("head %d\n", *head);
      printf ("sector %d\n", *sec);
    }

  *cyl = info->geo_cylinder;
  *head = info->geo_head;
  *sec = info->geo_sector + 1;


  (unsigned char *)cyl = &buf[HDPART_OFFSET];
  head = &buf[HDPART_OFFSET + 2];
  sec = &buf[HDPART_OFFSET + 3];

  if (verbose_flag)
    {
      printf ("old start CHS information:\n");
      printf ("cylinder %d\n", *cyl);
      printf ("head %d\n", *head);
      printf ("sector %d\n", *sec);
    }

  *cyl = info->boot_cylinder;
  *head = info->boot_head;
  *sec = info->boot_sector;

  if (lseek (fd, 0, SEEK_SET) < 0)
    {
      fprintf (stderr, "Cannot lseek in boot image. (%s)\n", strerror (errno));
      close (fd);
      exit (1);
    }

  if (write (fd, buf, sizeof (buf)) < 0)
    {
      fprintf (stderr, "Cannot write to boot image. (%s)\n", strerror (errno));
      close (fd);
      exit (1);
    }

  close (fd);
}

