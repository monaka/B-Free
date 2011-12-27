#ifndef __BTRON_FILMGR_H__
#define __BTRON_FILMGR_H__

/* Special symbols for file path. */
#define TC_FDLM (0xff21) /* Path separator */
#define TC_FSEP (0xff22) /* 出現順の区切り */
#define TC_FOWN (0xff23) /* Myself */

/* for get_lnk() */
#define F_NORM          (0x0) /* normal */
#define F_BASED         (0x1) /* ベース指定 */
#define F_DIRECT        (0x2) /* 直接取り出し指定 */

/* for opn_fil() */
#define F_READ          (0x0004) /* 読み込み専用オープン */
#define F_WRITE         (0x0002) /* 書き込み専用オープン */
#define F_UPDATE        (F_READ|F_WRITE) /* 更新用(読込/書込)オープン */

/* for cre_fil() and cre_lnk() */
#define F_FLOAT         (0x0000) /* Don't treat as fixed link. */
#define F_FIX           (0x0001) /* Treat as fixed link. *
#define F_FILEID        (0x0002) /* File ID */

/* for fnd_rec() and fnd_lnk() */
#define F_FWD           (0x0000) /* Search foward. */
#define F_NFWD          (0x0001) /* Search foward on next. */
#define F_BWD           (0x0002) /* Search backward. */
#define F_NBWD          (0x0003) /* Search backward on previous */
#define F_TOPEND        (0x0004) /* Search from top to end. */
#define F_ENDTOP        (0x0005) /* Search from ent to top. */

#define F_SFILE         (0x4000) /* Same file. */
#define F_SNAME         (0x2000) /* Same filename. */
#define F_SATR1         (0x1000) /* Same attribute data 1. */
#define F_SATR2         (0x0800) /* Same attribute data 2. */
#define F_SATR3         (0x0400) /* Same attribute data 3. */
#define F_SATR4         (0x0200) /* Same attribute data 4. */
#define F_SATR5         (0x0100) /* Same attribute data 5. */

/* for loc_rec() */
#define F_UNLOCK        (0x0000)
#define F_LOCK          (0x0001)
#define F_TSLOCK        (0x0002) /* Test & lock */
#define F_CKLOCK        (0x0003) /* Check lock status on.*/

/* for chk_fil() */
#define F_READ          (0x0004) /* access check on R */
#define F_WRITE         (0x0002) /* access check on W */
#define F_EXECUTE       (0x0001) /* access check on E */
#define F_EXIST         (0x0000) /* file existence check */
#define F_PASWD         (0x0008) /* The password available or not.*/

/* for chg_fat() */
#define F_SETRONLY      (0x0001) /* Set readonly flag. */
#define F_RSTRONLY      (0x0002) /* Reset readonly flag. */
#define F_SETPERM       (0x0003) /* Set unremovable flag. */
#define F_RSTPERM       (0x0004) /* Reset unremovable flag. */
#define F_SETA1         (0x0005) /* Set Application attribute 1. */
#define F_RSTA1         (0x0006) /* Reset Application attribute 1. */
#define F_SETA2         (0x0007) /* Set Application attribute 2. */
#define F_RSTA2         (0x0008) /* Reset Application attribute 2. */

/* for lst_fls() */
#define F_GETDEV        (-1) /* Get device name. */
#define F_GETNAM        (-2) /* Get connection name. */

/* link structure.*/
typedef struct {
        TC fs_name[20]; /* filesystem name.*/
        UW f_id; /*file ID */
        UW atr1;
        UW atr2;
        UW atr3;
        UW atr4;
        UW atr5;
} LINK

typedef LINK *LINKPTR;

/* file access mode structure.*/
typedef struct {
        UW f_ownacc; /* Owner access mode */
        W f_grpacc;  /* Group access level */
        W f_pubacc;  /* Public access level */
        W f_grpno;   /* Group number */
} A_MODE;

/* default file access mode structure */
typedef struct {
        UW f_ownacc; /* Owner access mode */
        W f_grpacc;  /* Group access level */
        W f_pubacc;  /* Public access level */
        W f_grpno;   /* Group number */
        W f_gacc[4]; /* Group access level */
} DA_MODE;

/* file date structure */
typedef struct {
        LONG f_ltime;   /* Expire date */
        LONG f_atime;   /* Last access date */
        LONG f_mtime;   /* Last modified date */
} F_TIME;

/* file state information structure */
typedef struct {
        UW f_type;   /* file type/owner access mode */
        UW f_atype;  /* application type */
        TC f_owner[14]; /* file owner's name (12 char) +0 */
        TC f_group[14]; /* file group's name (12 char) +0 */
        W f_grpacc;        /* group access level */
        W f_pubacc;          /* public access level */
        W f_nlink;           /* link numbers file contains. */
        W f_index;           /* index level */
        LONG f_size;            /* total bytes in files.*/
        LONG f_nblk;            /* total logical block number. */
        LONG f_nrec;            /* total amounts of records */
        LONG f_ltime;           /* File expire date. */
        LONG f_atime;           /* last access date. */
        LONG f_mtime;           /* last modified date. */
        LONG f_ctime;           /* file created date. */
} F_STATE;

/* for filetype */
#define F_FILE          (0x1000) /* normal file */
#define F_APLATR1       (0x0040) /* Application attribute 1 */
#define F_APLATR2       (0x0080) /* Application attribute 2 */
#define F_RONLY         (0x0010) /* Attribute for not writable.*/
#define F_PERM          (0x0020) /* Attribute for not removable.*/

#define F_OWNACR        (0x0004) /* Owner has permision to read.*/
#define F_OWNACW        (0x0002) /* Owner has permision to write.*/
#define F_OWNACE        (0x0001) /* Owner has permision to execute.*/
#define F_OWNACC        (F_OWNACR|F_OWNACW|F_OWNACE)

#define F_NOCHG         0x8000 /* No change */

/* file location structure */
typedef struct {
        LONG fs_ctime;          /* filesystem creation date */
        TC fs_name[20];      /* name for filesystem */
        TC fs_locat[20];     /* device location name */
        TC fs_dev[8];        /* logical device name */
}

/* file link information structure */
typedef struct {
        LONG f_ctime;           /* file creation date */
        UW f_atype;          /* Application type on reffered file. */
        TC f_name[20];       /* filename on reffered file. */
        UW f_id;             /* file ID on reffered file. */
        LONG rf_ctime;          /* create date on reffered file.*/
        TC fs_name[20];      /* filesystem name on reffered file.*/
        TC fs_locat[20];
} F_LINK;

/* filesystem administration information structure */
typedef struct {
        W fs_bsize;          /* maximum logical block size */
        UW fs_nfile;         /* maximum numbers of file. */
        W fs_lang;           /* Used language on filesystem.(Character code) */
        W fs_level;          /* Access level */
        LONG fs_nblk;           /* Total amount on blocks */
        LONG fs_nfree;          /* Total amount on unused blocks */
        LONG fs_mtime;          /* Last modified time (System block) */
        LONG fs_ctime;          /* Creation time */
        TC fs_name[20];      /* Filesystem name */
        TC fs_locat[20];     /* Device location */
} FS_STATE;

/* for FS_STATE.fs_lang */
#define F_JPN   (0x0021) /* Japanese */
#define F_ENG   (0x0080) /* English (Latin-1) */

/* for syn_lnk() */
#define F_SYNC  (0) /* syncronized */
#define F_DNAME (1) /* Different name */
#define F_DDATE (2) /* Different creation date */
#define F_FBOTH (F_DNAME|F_DDATE)

/* Filesystem connection information structure */
typedef struct {
        TC a_name[8] /* Connection name */
        TC dev[8]; /* logical device name */
} F_ATTACH;

#endif /* __BTRON_FILMGR_H__ */

