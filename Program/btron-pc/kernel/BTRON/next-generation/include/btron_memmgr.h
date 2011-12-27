#ifndef __BTRON_MEMMGR_H__
#define __BTRON_MEMMGR_H__

/* define for memory allocation. */
#define NOCLR 0x0008 /* Do not zero clear. */
#define CLR   0x0000 /* Do zero clear. */

/* define for memory attributes. */
/* 常駐 */
#define MPL_LOCK 0x4000   /* 常駐とする */
#define MPL_UNLOCK 0x0000 /* 常駐としない */

/* for Memory protection */
#define MPL_SELF   0x0000 /* The process have more than generation level if it write or read. */
#define MPL_ANY    0x1000 /* No protection */

#define DELEXIT    0x8000 /* delete when the process exit.

/* for attributes on shared memory block. */
/* 再配置属性 */
#define M_LOCK    0x0001 /* 再配置属性(禁止) */
#define M_UNLOCK  0x0000 /* 再配置属性(許可) */
#define M_RELOC   0x0002 /* 再配置状態(1:再配置済み) */
#define M_NOPURGE 0x0000 /* パージ属性(禁止) */
#define M_PURGE   0x0004 /* パージ属性(許可) */
#define M_PURGED  0x0008 /* パージ状態(1:パージ済み) */

/* local memory status. */
typedef struct {
        LONG lm_tsize; /* total size (byte) */
        LONG lm_fsize; /* unused size (byte) */
        LONG lm_msize; /* 取得可能最大サイズ (byte) */
        W ln_nblk;  /* Memory block size process has. */
        W ln_misc[2]; /* No use always zero value.*/
} LM_STATE;

/* shared memory status. */
typedef struct {
        UW sm_attr; /* memory block attributes */
        W sm_mplid; /* memory pool ID */
        LONG sm_size;  /* memory block size (byte) */
        LONG sm_info;  /* user information */
        B *sm_ptr;   /* memory block first address */
        W sm_misc[2]; /* protect level, number of segment */
} SM_STATE;

/* shared memory pool status. */
typedef struct {
        UW mp_attr; /* memory pool attributes */
        LONG mp_tsize; /* memory pool size (byte) */
        LONG mp_fsize; /* unused memory pool size (byte) */
        LONG mp_msize; /* 取得可能最大サイズ (byte) */
        LONG mp_nblk;  /* blocks already getting (block) */
        B *mp_ptr;   /* always -1. */
        W mp_misc[2]; /* always zero. */
}

#endif /* __BTRON_MEMMGR_H__ */

























           
