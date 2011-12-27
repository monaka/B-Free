#include <stdio.h>
#include <linux/hdreg.h>


main (int ac, char **av)
{
  struct hd_geometry geo;
  int	fd;

  fd = open (av[1], 0);
  if (ioctl (fd, HDIO_GETGEO, &geo) < 0)
    {
      perror ("ioctl");
    }
  close (fd);

  printf ("%s: C/H/S %d/%d/%d, start = %d\n",
	av[1],
	geo.cylinders, geo.heads, geo.sectors,
	geo.start);
   
}

