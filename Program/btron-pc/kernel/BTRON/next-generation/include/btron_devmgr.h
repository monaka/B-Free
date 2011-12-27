#ifndef __BTRON_DEVMGR_H__
#define __BTRON_DEVMGR_H__

/* for opn_dev */
#define D_READ          (0x0001)                /* Open readonly */
#define D_WRITE         (0x0002)                /* Open writeonly */
#define D_UPDATE        (D_READ|D_WRITE)        /* Open read and write */
#define D_EXCL          (0x0100)                /* 排他モード */
#define D_WEXCL         (0x0200)                /* 排他書き込みモード */
#define D_NOWAIT        (0x8000)                /* Nowait mode.*/

/* special device descriptor (for ctl_dev) */
#define DD_KB           (0x4000)        /* Keyboard */
#define DD_PD           (0x4001)        /* Pointing device */

/* subcomand under ctl_dev() */
#define I_FORMAT        (0x0003)

/* device administration information structure */
typedef struct {
        UWORD attr;     /* device attributes */
        UWORD mode;     /* Current access mode */
        WORD blksz;     /* Physical block size (byte) (-1 == unknown) */
        WORD wprt;      /* 0: Writeable, 1: Readonly (-1 == unknown) */
} DEVSTATE;

typedef struct {
        UWORD attr;     /* device attribute */
        WORD nsub;      /* Sub unit number */
        TCODE name[8];  /* unit name */
} DEV_INFO;

#endif /*__BTRON_DEVMGR_H__*/

