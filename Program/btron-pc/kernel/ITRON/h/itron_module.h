#ifndef __ITRON_MODULE_H__
#define __ITRON_MODULE_H__

#define MAX_MODULE_NAME	(40)
#define MAX_MODULE	(20)

enum module_type 
{
  kernel	= 0,
  user		= 1,
  lowlib	= 2, 
  server      	= 3,
  driver	= 4
};


/* $BJ#?t$N%b%8%e!<%k$rFI$_9~$`$?$a$N%X%C%@(B
 *
 *	$B%X%C%@$O0J2<$NMWAG$+$i$G$-$F$$$k(B
 *
 *	1) cookie:	$B%^%8%C%/HV9f(B($B%X%C%@<1JLMQ(B)
 *	2) count:	$B%b%8%e!<%k?t(B
 *	3) module_info:	$B3F%b%8%e!<%k$N>pJs(B
 *	   length
 *	   vaddr
 *	   entry
 *	   mode
 */
struct module_info
{
  int		length;		/* $B%G%#%9%/>e$G$NBg$-$5(B */
  int		mem_length;	/* $B%a%b%jCf$G$NBg$-$5(B; 
				   $B%X%C%@$r4^$^$:!"(BBSS $BNN0h$r4^$`(B */
  unsigned int		vaddr;	/* $B2>A[%"%I%l%9(B	*/
  unsigned int		paddr;	/* $B<B%"%I%l%9(B	*/
  unsigned int		entry;
  enum module_type	type;
  char		name[MAX_MODULE_NAME];
};

struct machine_info
{
  unsigned int	ext_mem;
  unsigned int	real_mem;
  unsigned int	base_mem;
  unsigned int	rootfs;
};

struct boot_header
{
  unsigned char		cookie;		/* 0001 $B$K8GDj(B */
  int			count;
  struct machine_info	machine;
  struct module_info	modules[0];
};


#endif /*__ITRON_MODULE_H__*/
