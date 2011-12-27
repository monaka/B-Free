#ifndef __BTRON_PRCMGR_H__
#define __BTRON_PRCMGR_H__

/* process state */
typedef struct {
        UW state;    /* �v���Z�X�̏�� */
        UW priority; /* ���݂̃v���Z�X�̗D��x */
        W parpid;    /* �e�v���Z�X�̃v���Z�XID */
} P_STATE;

/* process user information */
typedef struct {
        TCODE usr_name[14];     /* ���[�U�� (+ �B����2����) */
        TCODE grp_name1[14];    /* �O���[�v��1 (+ �B����2����) */
        TCODE grp_name2[14];    /* �O���[�v��2 (+ �B����2����) */
        TCODE grp_name3[14];    /* �O���[�v��3 (+ �B����2����) */
        TCODE grp_name4[14];    /* �O���[�v��4 (+ �B����2����) */
        W level;             /* ���[�U���x�� (0-15) */
        W net_level;         /* �l�b�g���[�N�E���[�U���x�� (1-15) */
} P_USER;

/* process information */
typedef struct {
        ULONG etime;            /* ��������Ă���̗݌v�o�ߎ��� (sec) */
        ULONG utime;            /* �v���Z�X�Ŕ�₵���݌vCPU���� (msec) */
        ULONG stime;            /* �V�X�e���ł��̃v���Z�X�̂��߂ɔ�₵���݌vCPU���� (msec) */
        ULONG tmem;             /* ���s�ɕK�v�Ƃ���v���Z�X�S�̂̃������T�C�Y (byte) */
        ULONG wmem;             /* ���݃v���Z�X�Ɋ�������Ă�����������T�C�Y (byte) */
        LONG resv[11];          /* �\�� */
} P_INFO;

/* �����I���Ɋւ���萔 */
#define TERM_NRM 0x0000 /* �w�肵���v���Z�X�̂݋����I�� */
#define TERM_ALL 0x0001 /* �q�v���Z�X�܂Ŋ܂߂������I�� */

/* �D��x�Ɋւ���萔 */
#define P_ABS    0x0000 /* �D��x�̐�Ύw�� */
#define P_REL    0x0001 /* �D��x�̑��Ύw�� */

/* ���s��ԂɊւ���萔 */
#define P_LOCK   0x0001 /* ���b�N��� */
#define P_WAIT   0x2000 /* �҂���� */
#define P_READY  0x4000 /* ���s�\��� */
#define P_RUN    0x8000 /* ���s��� */


/* message structure */
typedef union {
        struct {
                W pid;
                W code;
        } ABORT; /* �q�v���Z�X�ُ̈�I�� */
        struct {
                W pid;
                W code;
        } EXIT; /* �q�v���Z�X�̐���I�� */
        struct {
                W pid;
                W code;
        } TERM; /* �q�v���Z�X�̋����I�� */
        struct {
                W code;
        } TMOUT; /* ���v���Z�X�̃^�C���A�E�g */
        struct {
                LONG iopac;
        } IOREQ; /* �h���C�o�v���Z�X�ւ̗v�� */
        struct {
                W code;
        } DOWN; /* �V�X�e����~�v�� */
        struct {
                B msg_str[32];
        } ANYMSG; /* �ėp */
} MSGBODY;

typedef struct {
        W msg_type;          /* ���b�Z�[�W�^�C�v */
        W msg_size;          /* ���b�Z�[�W�T�C�Y (�o�C�g��) */
        B msg_body[0];       /* ���b�Z�[�W�{�� (msg_size�o�C�g) */
} MESSAGE;

/* ���b�Z�[�W����̒�` */
#define WAIT    0x0000 /* �L���[�ɓ���܂ő҂� */
#define NOWAIT  0x0001 /* �L���[�ɓ���܂ő҂��Ȃ� */
#define CONFM   0x0002 /* Wait until the message is received. */
#define CLR     0x0000 /* Remove from the queue. */
#define NOCLR   0x0008 /* Leave it on the queue. */
#define CHECK   0x0002 /* Checking whether messages are available or not. */

/* System pre-defined message handler. */
#define MH_NONE  ((FUNCP)1)
#define MH_BREAK ((FUNCP)2)
#define MH_TERM  ((FUNCP)3)

/* defines used for message ID */
#define MS_ABORT (1) /* �q�v���Z�X�ُ̈�I�� */
#define MS_EXIT  (2) /* �q�v���Z�X�̐���I�� */
#define MS_TERM  (3) /* �q�v���Z�X�̋����I�� */
#define MS_TMOUT (4) /* �^�C���A�E�g */
#define MS_IOREQ (5) /* �f�o�h���v���Z�X�v�� */
#define MS_SYS   (6) /* reserved */
#define MS_DOWN  (7) /* �V�X�e����~�v�� */
#define MS_SRSV0 (8) /* reserved */
#define MS_SRSV1 (9) /* reserved */
#define MS_SRSV2 (10) /* reserved */
#define MS_MNG0  (11) /* reserved */
#define MS_MNG1  (12) /* �E�C���h�E�}�l�[�W���p���U�[�u */
#define MS_MNG2  (13) /* �E�C���h�E�}�l�[�W���p���U�[�u */
#define MS_MNG3  (14) /* ���g/���g�}�l�[�W���p���U�[�u */
#define MS_MNG4  (15) /* �X�v�[���p reserved */
#define MS_MNG5  (16) /* �X�v�[���p reserved */
#define MS_MNG6  (17) /* LAN�p reserved */
#define MS_MNG7  (18) /* LAN�p reserved */
#define MS_MNG8  (19) /* reserved */
#define MS_MNG9  (20) /* reserved */
#define MS_MNG10 (21) /* reserved */
#define MS_MNG11 (22) /* reserved */
#define MS_MNG12 (23) /* reserved */
#define MS_TYPE0 (24) /* �A�v���P�[�V�������b�Z�[�W #0 */
#define MS_TYPE1 (25) /* �A�v���P�[�V�������b�Z�[�W #1 */
#define MS_TYPE2 (26) /* �A�v���P�[�V�������b�Z�[�W #2 */
#define MS_TYPE3 (27) /* �A�v���P�[�V�������b�Z�[�W #3 */
#define MS_TYPE4 (28) /* �A�v���P�[�V�������b�Z�[�W #4 */
#define MS_TYPE5 (29) /* �A�v���P�[�V�������b�Z�[�W #5 */
#define MS_TYPE6 (30) /* �A�v���P�[�V�������b�Z�[�W #6 */
#define MS_TYPE7 (31) /* �A�v���P�[�V�������b�Z�[�W #7 */

#define MM_ABORT (0x00000001) /* �q�v���Z�X�ُ̈�I�� */
#define MM_EXIT  (0x00000002) /* �q�v���Z�X�̐���I�� */
#define MM_TERM  (0x00000004) /* �q�v���Z�X�̋����I�� */
#define MM_TMOUT (0x00000008) /* �^�C���A�E�g */
#define MM_IOREQ (0x00000010) /* �f�o�h���v���Z�X�v�� */
#define MM_SYS   (0x00000020) /* reserved */
#define MM_DOWN  (0x00000040) /* �V�X�e����~�v�� */
#define MM_SRSV0 (0x00000080) /* reserved */
#define MM_SRSV1 (0x00000100) /* reserved */
#define MM_SRSV2 (0x00000200) /* reserved */
#define MM_MNG0  (0x00000400) /* reserved */
#define MM_MNG1  (0x00000800) /* �E�C���h�E�}�l�[�W���p���U�[�u */
#define MM_MNG2  (0x00001000) /* �E�C���h�E�}�l�[�W���p���U�[�u */
#define MM_MNG3  (0x00002000) /* ���g/���g�}�l�[�W���p���U�[�u */
#define MM_MNG4  (0x00004000) /* �X�v�[���p reserved */
#define MM_MNG5  (0x00008000) /* �X�v�[���p reserved */
#define MM_MNG6  (0x00010000) /* LAN�p reserved */
#define MM_MNG7  (0x00020000) /* LAN�p reserved */
#define MM_MNG8  (0x00040000) /* reserved */
#define MM_MNG9  (0x00080000) /* reserved */
#define MM_MNG10 (0x00100000) /* reserved */
#define MM_MNG11 (0x00200000) /* reserved */
#define MM_MNG12 (0x00400000) /* reserved */
#define MM_TYPE0 (0x00800000) /* �A�v���P�[�V�������b�Z�[�W #0 */
#define MM_TYPE1 (0x01000000) /* �A�v���P�[�V�������b�Z�[�W #1 */
#define MM_TYPE2 (0x02000000) /* �A�v���P�[�V�������b�Z�[�W #2 */
#define MM_TYPE3 (0x04000000) /* �A�v���P�[�V�������b�Z�[�W #3 */
#define MM_TYPE4 (0x08000000) /* �A�v���P�[�V�������b�Z�[�W #4 */
#define MM_TYPE5 (0x10000000) /* �A�v���P�[�V�������b�Z�[�W #5 */
#define MM_TYPE6 (0x20000000) /* �A�v���P�[�V�������b�Z�[�W #6 */
#define MM_TYPE7 (0x40000000) /* �A�v���P�[�V�������b�Z�[�W #7 */
#define MM_ALL   (0x7FFFFFFF) /* �S�^�C�v�w�� */
#define MM_NULL  (0x00000000) /* �S�^�C�v�w��O */

/* for semaphoe. */
#define SEM_SYNC (0x0000) /* Syncronize semaphoe */
#define SEM_EXCL (0x4000) /* �r������p�Z�}�t�H */

#define DELEXIT  (0x8000) /* Delete when process is killed. */

/* for Global Name Data */
#define N_CREATE (0x0000) /* Create a new Global Name Data. */
#define N_MODIFY (0x0001) /* Modify exsisting one. */
#define N_FORCE  (0x0002) /* Force creation. */

#endif /* __BTRON_PRCMGR_H__ */



