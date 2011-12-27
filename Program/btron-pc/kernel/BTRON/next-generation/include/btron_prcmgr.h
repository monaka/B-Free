#ifndef __BTRON_PRCMGR_H__
#define __BTRON_PRCMGR_H__

/* process state */
typedef struct {
        UW state;    /* プロセスの状態 */
        UW priority; /* 現在のプロセスの優先度 */
        W parpid;    /* 親プロセスのプロセスID */
} P_STATE;

/* process user information */
typedef struct {
        TCODE usr_name[14];     /* ユーザ名 (+ 隠し名2文字) */
        TCODE grp_name1[14];    /* グループ名1 (+ 隠し名2文字) */
        TCODE grp_name2[14];    /* グループ名2 (+ 隠し名2文字) */
        TCODE grp_name3[14];    /* グループ名3 (+ 隠し名2文字) */
        TCODE grp_name4[14];    /* グループ名4 (+ 隠し名2文字) */
        W level;             /* ユーザレベル (0-15) */
        W net_level;         /* ネットワーク・ユーザレベル (1-15) */
} P_USER;

/* process information */
typedef struct {
        ULONG etime;            /* 生成されてからの累計経過時間 (sec) */
        ULONG utime;            /* プロセスで費やした累計CPU時間 (msec) */
        ULONG stime;            /* システムでそのプロセスのために費やした累計CPU時間 (msec) */
        ULONG tmem;             /* 実行に必要とするプロセス全体のメモリサイズ (byte) */
        ULONG wmem;             /* 現在プロセスに割りつけられている実メモリサイズ (byte) */
        LONG resv[11];          /* 予約 */
} P_INFO;

/* 強制終了に関する定数 */
#define TERM_NRM 0x0000 /* 指定したプロセスのみ強制終了 */
#define TERM_ALL 0x0001 /* 子プロセスまで含めた強制終了 */

/* 優先度に関する定数 */
#define P_ABS    0x0000 /* 優先度の絶対指定 */
#define P_REL    0x0001 /* 優先度の相対指定 */

/* 実行状態に関する定数 */
#define P_LOCK   0x0001 /* ロック状態 */
#define P_WAIT   0x2000 /* 待ち状態 */
#define P_READY  0x4000 /* 実行可能状態 */
#define P_RUN    0x8000 /* 実行状態 */


/* message structure */
typedef union {
        struct {
                W pid;
                W code;
        } ABORT; /* 子プロセスの異常終了 */
        struct {
                W pid;
                W code;
        } EXIT; /* 子プロセスの正常終了 */
        struct {
                W pid;
                W code;
        } TERM; /* 子プロセスの強制終了 */
        struct {
                W code;
        } TMOUT; /* 自プロセスのタイムアウト */
        struct {
                LONG iopac;
        } IOREQ; /* ドライバプロセスへの要求 */
        struct {
                W code;
        } DOWN; /* システム停止要求 */
        struct {
                B msg_str[32];
        } ANYMSG; /* 汎用 */
} MSGBODY;

typedef struct {
        W msg_type;          /* メッセージタイプ */
        W msg_size;          /* メッセージサイズ (バイト数) */
        B msg_body[0];       /* メッセージ本体 (msg_sizeバイト) */
} MESSAGE;

/* メッセージ操作の定義 */
#define WAIT    0x0000 /* キューに入るまで待つ */
#define NOWAIT  0x0001 /* キューに入るまで待たない */
#define CONFM   0x0002 /* Wait until the message is received. */
#define CLR     0x0000 /* Remove from the queue. */
#define NOCLR   0x0008 /* Leave it on the queue. */
#define CHECK   0x0002 /* Checking whether messages are available or not. */

/* System pre-defined message handler. */
#define MH_NONE  ((FUNCP)1)
#define MH_BREAK ((FUNCP)2)
#define MH_TERM  ((FUNCP)3)

/* defines used for message ID */
#define MS_ABORT (1) /* 子プロセスの異常終了 */
#define MS_EXIT  (2) /* 子プロセスの正常終了 */
#define MS_TERM  (3) /* 子プロセスの強制終了 */
#define MS_TMOUT (4) /* タイムアウト */
#define MS_IOREQ (5) /* デバドラプロセス要求 */
#define MS_SYS   (6) /* reserved */
#define MS_DOWN  (7) /* システム停止要求 */
#define MS_SRSV0 (8) /* reserved */
#define MS_SRSV1 (9) /* reserved */
#define MS_SRSV2 (10) /* reserved */
#define MS_MNG0  (11) /* reserved */
#define MS_MNG1  (12) /* ウインドウマネージャ用リザーブ */
#define MS_MNG2  (13) /* ウインドウマネージャ用リザーブ */
#define MS_MNG3  (14) /* 実身/仮身マネージャ用リザーブ */
#define MS_MNG4  (15) /* スプーラ用 reserved */
#define MS_MNG5  (16) /* スプーラ用 reserved */
#define MS_MNG6  (17) /* LAN用 reserved */
#define MS_MNG7  (18) /* LAN用 reserved */
#define MS_MNG8  (19) /* reserved */
#define MS_MNG9  (20) /* reserved */
#define MS_MNG10 (21) /* reserved */
#define MS_MNG11 (22) /* reserved */
#define MS_MNG12 (23) /* reserved */
#define MS_TYPE0 (24) /* アプリケーションメッセージ #0 */
#define MS_TYPE1 (25) /* アプリケーションメッセージ #1 */
#define MS_TYPE2 (26) /* アプリケーションメッセージ #2 */
#define MS_TYPE3 (27) /* アプリケーションメッセージ #3 */
#define MS_TYPE4 (28) /* アプリケーションメッセージ #4 */
#define MS_TYPE5 (29) /* アプリケーションメッセージ #5 */
#define MS_TYPE6 (30) /* アプリケーションメッセージ #6 */
#define MS_TYPE7 (31) /* アプリケーションメッセージ #7 */

#define MM_ABORT (0x00000001) /* 子プロセスの異常終了 */
#define MM_EXIT  (0x00000002) /* 子プロセスの正常終了 */
#define MM_TERM  (0x00000004) /* 子プロセスの強制終了 */
#define MM_TMOUT (0x00000008) /* タイムアウト */
#define MM_IOREQ (0x00000010) /* デバドラプロセス要求 */
#define MM_SYS   (0x00000020) /* reserved */
#define MM_DOWN  (0x00000040) /* システム停止要求 */
#define MM_SRSV0 (0x00000080) /* reserved */
#define MM_SRSV1 (0x00000100) /* reserved */
#define MM_SRSV2 (0x00000200) /* reserved */
#define MM_MNG0  (0x00000400) /* reserved */
#define MM_MNG1  (0x00000800) /* ウインドウマネージャ用リザーブ */
#define MM_MNG2  (0x00001000) /* ウインドウマネージャ用リザーブ */
#define MM_MNG3  (0x00002000) /* 実身/仮身マネージャ用リザーブ */
#define MM_MNG4  (0x00004000) /* スプーラ用 reserved */
#define MM_MNG5  (0x00008000) /* スプーラ用 reserved */
#define MM_MNG6  (0x00010000) /* LAN用 reserved */
#define MM_MNG7  (0x00020000) /* LAN用 reserved */
#define MM_MNG8  (0x00040000) /* reserved */
#define MM_MNG9  (0x00080000) /* reserved */
#define MM_MNG10 (0x00100000) /* reserved */
#define MM_MNG11 (0x00200000) /* reserved */
#define MM_MNG12 (0x00400000) /* reserved */
#define MM_TYPE0 (0x00800000) /* アプリケーションメッセージ #0 */
#define MM_TYPE1 (0x01000000) /* アプリケーションメッセージ #1 */
#define MM_TYPE2 (0x02000000) /* アプリケーションメッセージ #2 */
#define MM_TYPE3 (0x04000000) /* アプリケーションメッセージ #3 */
#define MM_TYPE4 (0x08000000) /* アプリケーションメッセージ #4 */
#define MM_TYPE5 (0x10000000) /* アプリケーションメッセージ #5 */
#define MM_TYPE6 (0x20000000) /* アプリケーションメッセージ #6 */
#define MM_TYPE7 (0x40000000) /* アプリケーションメッセージ #7 */
#define MM_ALL   (0x7FFFFFFF) /* 全タイプ指定 */
#define MM_NULL  (0x00000000) /* 全タイプ指定外 */

/* for semaphoe. */
#define SEM_SYNC (0x0000) /* Syncronize semaphoe */
#define SEM_EXCL (0x4000) /* 排他制御用セマフォ */

#define DELEXIT  (0x8000) /* Delete when process is killed. */

/* for Global Name Data */
#define N_CREATE (0x0000) /* Create a new Global Name Data. */
#define N_MODIFY (0x0001) /* Modify exsisting one. */
#define N_FORCE  (0x0002) /* Force creation. */

#endif /* __BTRON_PRCMGR_H__ */



