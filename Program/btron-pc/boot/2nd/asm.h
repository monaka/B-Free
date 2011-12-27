#ifndef __IO_H__
#define __IO_H__

extern void enable_page (void);

extern void outb (short ioaddr, unsigned char byte);
extern void outw (short ioaddr, unsigned short data);
extern void outl (short ioaddr, unsigned long data);
extern unsigned char inb (short ioaddr);
extern unsigned short inw (short ioaddr);
extern unsigned long inl (short ioaddr);

extern void lock(void);
extern void unlock(void);

#endif
