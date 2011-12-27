#ifndef __BTRON_TIMMGR_H__
#define __BTRON_TIMMGR_H__

/* Time zone structure.*/
typedef struct {
        LONG adjust;
        W dst_flg;
        W dst_adj;
} TIMEZONE;



/* Calender data structure */

typedef struct {
        W d_year;
        W d_month;
        W d_day;
        W d_hour;
        W d_min;
        W d_sec;
        W d_week;
        W d_wday;
        W d_days;
} DATE_TIM;

#endif /* __BTRON_TIMMGR_H__ */

