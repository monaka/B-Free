#ifndef __aha152xlow_h__
#define __aha152xlow_h__


#define printk     printf
#define dbg_printf printf



#define ISA_SCSI_DRIVER  "driver.isa_scsi"
#define MAX_ISA_SCSI 1

//void outp(UW adr, UB data);
#define __inb(a)     inb(a)
#define __inbc(a)    inb(a)
#define __inbc_p(a)  inb(a)
#define __inb_p(a)   inb(a)
#define insw(a)      inw(a)
 
#define __outb(a,b)     outb(a,b)
#define __outbc(a,b)    outb(a,b)
#define __outbc_p(a,b)  outb(a,b)
#define __outb_p(a,b)   outb(a,b)
#define outsw(a,b)      outw(a,b)

//#define kdevname(a) (a)
#endif
