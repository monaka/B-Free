#ifndef __BTRON_MEMMGR_H__
#define __BTRON_MEMMGR_H__

/* define for memory allocation. */
#define NOCLR 0x0008 /* Do not zero clear. */
#define CLR   0x0000 /* Do zero clear. */

/* define for memory attributes. */
/* �풓 */
#define MPL_LOCK 0x4000   /* �풓�Ƃ��� */
#define MPL_UNLOCK 0x0000 /* �풓�Ƃ��Ȃ� */

/* for Memory protection */
#define MPL_SELF   0x0000 /* The process have more than generation level if it write or read. */
#define MPL_ANY    0x1000 /* No protection */

#define DELEXIT    0x8000 /* delete when the process exit.

/* for attributes on shared memory block. */
/* �Ĕz�u���� */
#define M_LOCK    0x0001 /* �Ĕz�u����(�֎~) */
#define M_UNLOCK  0x0000 /* �Ĕz�u����(����) */
#define M_RELOC   0x0002 /* �Ĕz�u���(1:�Ĕz�u�ς�) */
#define M_NOPURGE 0x0000 /* �p�[�W����(�֎~) */
#define M_PURGE   0x0004 /* �p�[�W����(����) */
#define M_PURGED  0x0008 /* �p�[�W���(1:�p�[�W�ς�) */

/* local memory status. */
typedef struct {
        LONG lm_tsize; /* total size (byte) */
        LONG lm_fsize; /* unused size (byte) */
        LONG lm_msize; /* �擾�\�ő�T�C�Y (byte) */
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
        LONG mp_msize; /* �擾�\�ő�T�C�Y (byte) */
        LONG mp_nblk;  /* blocks already getting (block) */
        B *mp_ptr;   /* always -1. */
        W mp_misc[2]; /* always zero. */
}

#endif /* __BTRON_MEMMGR_H__ */

























           
