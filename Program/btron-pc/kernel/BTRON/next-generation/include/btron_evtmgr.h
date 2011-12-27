#ifndef __BTRON_EVTMGR_H__

/* Point structure */
typedef union {
        struct {
                WORD h;
                WORD v;
        } c;
        LONG l;
} POINT;

/* Event structure */
typedef struct {
        WORD type;      /* event type */
        ULONG time;     /* time on event occured */
        POINT pos;      /* PD position on event occured */
        EVDATA data;    /* event specific data */
        UWORD stat;     /* meta,PD button status */
} EVENT;

#define EV_NULL         (0)     /* null event */
#define EV_BUTDWN       (1)     /* button down */
#define EV_BUTUP        (2)     /* button up */
#define EV_KEYDWN       (3)     /* key down */
#define EV_KEYUP        (4)     /* key up */
#define EV_AUTKEY       (5)     /* auto repeat key */
#define EV_DEVICE       (6)     /* device event */
#define EV_RSV          (7)     /* reserved */
#define EV_APPL1        (8)     /* Application Event #1 */
#define EV_APPL2        (9)     /* Application Event #2 */
#define EV_APPL3        (10)     /* Application Event #3 */
#define EV_APPL4        (11)     /* Application Event #4 */
#define EV_APPL5        (12)     /* Application Event #5 */
#define EV_APPL6        (13)     /* Application Event #6 */
#define EV_APPL7        (14)     /* Application Event #7 */
#define EV_APPL8        (15)     /* Application Event #8 */

/* Event specific data structure */
typedef union {
        struct {
                UWORD keytop; /* keytop code */
                TCODE code;   /* keycode */
        } key;
        struct {
                WORD kind;      /* device event type */
                WORD devno;     /* device number */
        } dev; /* for device event */
        LONG info; /* for others */
} EVDATA;

/* define for meta key, PD button status */
#define ES_BUT   (0x0001) /* PD's main button */
#define ES_BUT2  (0x0002) /* PD's button2 */
#define ES_ALPH  (0x0004) /* Alphabet lock key */
#define ES_KANA  (0x0008) /* Katakana lock key */
#define ES_LSHFT (0x0010) /* Left shift key */
#define ES_RSHFT (0x0020) /* Right shift key */
#define ES_EXT   (0x0040) /* Extend key */
#define ES_CMD   (0x0080) /* Command key */

/* for keymap */
tpedef UBYTE KEYMAP[16];

/* defines for event mask. */
#define EM_NULL (0x0000)
#define EM_ALL  (0x7FFF)

#define EM_BUTDWN       (0x0001)
#define EM_BUTUP        (0x0002)
#define EM_KEYDOWN      (0x0004)
#define EM_KEYUP        (0x0008)
#define EM_AUTKEY       (0x0010)
#define EM_DEVICE       (0x0020)
#define EM_RSV          (0x0040)
#define EM_APPL1        (0x0080)
#define EM_APPL2        (0x0100)
#define EM_APPL3        (0x0200)
#define EM_APPL4        (0x0400)
#define EM_APPL5        (0x0800)
#define EM_APPL6        (0x1000)
#define EM_APPL7        (0x2000)
#define EM_APPL8        (0x4000)

/* keyboard ID structure */
typedef struct {
        UWORD kind;  /* keyboard type */
        UWORD maker; /* maker ID */
        BYTE id[4];  /* maker dependent keyboard ID */
} KBD_ID;

/* Character code conversion structure */
typedef struct {
        WORD key_max;
        WORD kct_max;
        WORD kct_sel[32];
        TCODE kct[1];
} KEYTAB;

/* Other defines */
#define CLR     (0x0000)
#define NOCLR   (0x0008)

#define EP_NONE (0x0000)
#define EP_POS  (0x0001)
#define EP_STAT (0x0002)
#define EP_TIME (0x0004)
#define EP_ALL  (EP_POS|EP_STAT|EP_TIME)

#define PD_ABS  (0x0100)        /* absolute */
#define PD_REL  (0x0000)        /* relative */
#define PD_SCMSK (0x00f0)       /* mask for scan speed */
#define PD_SNMSK (0x000f)       /* mask for scan sensivity */

#endif __BTRON_EVTMGR_H__

