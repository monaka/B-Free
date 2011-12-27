#ifndef __BTRON_SYSMGR_H__
#define __BTRON_SYSMGR_H__

typedef struct {
        UW maker;
        UW id;
        UW spver;
        UW prver;
        UW prno[4];
        UW cpu;
        UW var;
} T_VER;


/* debug process control information structure */
typedef struct {
        W request;
        W pid;
        B *addr;
        W regnum;
        B *data;
        UW size;
} TRACE;

#endif /* __BTRON_SYSMGR_H__ */

