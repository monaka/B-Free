/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* func.h - 大域関数の定義
 *
 */

#ifndef __ITRON_FUNC_H__
#define __ITRON_FUNC_H__	1


#include "types.h"
#include "itron.h"
#include "task.h"

#include "itron_syscall_func.h"

/* main.c */
extern ER	main (void);

/* printk.c */
extern W	printk (B *, ...);
extern W	vprintk (B *, VP);

/* error.c */
extern void     falldown(B *, ...);

/* memory.c */
extern void	init_memory (void);
extern W	alloc_memory (W size);
extern ER	free_memory (W index, W size);

/* pmemory.c */
extern void	pmem_init (void);
extern VP	palloc (W size);
extern ER	pfree (VP p, W size);
extern void     pmemstat(void);
extern UW	pmemfree(void);
extern void     pmemdump(VP *argp);

/* console.c */
extern void	simple_init_console (void);
extern BOOL	init_crt (W);		/* 初期化			*/
extern W	intr_crt (void);		/* 割り込みハンドラ		*/

extern void	putchar (TC ch);	/* １文字出力			*/
extern void	write_cr (void);
extern void	write_tab (void);
extern void	console_clear (void);
extern ER	get_cursor_position (W *x, W *y);

/* timer.c */
extern void	start_interval (void);
extern void	intr_interval (void);
extern void	init_timer (void);
extern void	set_timer (W time, void (* func)(VP), VP argp);
extern ER	unset_timer (void (* func)(VP), VP arg);
extern void	check_timer(void);
extern W	left_time (void (* func)(VP), VP arg);

/* misc.c */
extern UH	halfword_swap (UH w);
extern VP	bcopy (VP src, VP dest, W count);
extern void	bzero (VP src, W count);
extern INT	strlen (B *buf);
extern W	strncmp (B *s1, B *s2, W size);

/* virtual_memory.c */
extern ADDR_MAP	dup_vmap_table (ADDR_MAP dest);
extern ER	release_vmap (ADDR_MAP dest);
extern BOOL	vmap (T_TCB *task, UW vpage, UW ppage, W accmode);
extern UW       vtor (ID tskid, UW addr);

/* task.c */
extern void	init_task (void);
extern void	init_task1 (void);
extern void	init_task_state_segment (W index, T_I386_CONTEXT *tss);
extern T_TCB	*add_tcb_list (T_TCB *list, T_TCB *new);
extern T_TCB	*ins_tcb_list (T_TCB *list, T_TCB *new);
extern T_TCB	*del_tcb_list (T_TCB *list, T_TCB *del);
extern ER	task_switch (BOOL save_nowtask);
extern void	print_task_list (void);
extern T_TCB	*get_tskp (ID tskid);
extern void	salvage_task(void);

/* tss.c */
extern void	print_task_desc (TASK_DESC *desc);

/* keyboard.c */
extern W intr_kbd(void);

/* kalloc.c */
extern void	init_kalloc (void);
extern VP	kalloc (W size);
extern void	kfree (VP area, W size);

/* version.c */
extern ER	get_ver (T_VER *ver);

/* message.c */
extern ER	init_msgbuf (void);
extern void	del_task_mbf(ID tid);

/* semaphore.c */
extern ER	init_semaphore(void);
extern void	del_task_sem(ID tid);

/* eventflag.c */
extern ER	init_eventflag (void);
extern void	del_task_evt(ID tid);

/* time.c */
extern void	init_time(void);

/* lowlib.c */
extern ER	init_lowlib (struct module_info *modp);
extern ER	load_lowlib (VP *argp);
extern ER	unload_lowlib (VP *argp);
extern ER	stat_lowlib (VP *argp);

/* itron_memory.c */
extern ER	init_mpl (void);

/* kernlib/message_port.c */
extern ID get_port (W minsize, W maxsize);

/* --------------------------------------------------------- */
/* テスト用タスク                                            */
/* --------------------------------------------------------- */
extern void	tsk2_func (void);
extern void	tsk3_func (void);

/* --------------------------------------------------------- */
/* システム管理用タスク                                      */
/* --------------------------------------------------------- */
extern void	sys_init_task (void);

/* --------------------------------------------------------- */
/* デバッグ用タスク                                          */
/* --------------------------------------------------------- */
extern void	debugger (void);

/* --------------------------------------------------------- */
/* RS232C デバイス管理タスク				     */
/* --------------------------------------------------------- */
extern void	rs232c_task (void);
extern void	keyboard_task (void);


/* --------------------------------------------------------- */
/* port manager                                              */
/* --------------------------------------------------------- */
extern void     regist_port (struct port_manager_msg_t *msgp);
extern void     unregist_port (struct port_manager_msg_t *msgp);
extern ER       find_port (B *name, ID *rport);

#ifdef I386
/* tss.c */
extern void create_context (T_TCB *task);
extern void init_task_state_segment (W index, T_I386_CONTEXT *tss);

/* gdt.c */
extern ER set_gdt (int index, GEN_DESC *desc);
extern GEN_DESC *get_gdt (int index);

/* locore.S */
extern void resume(UW);
extern void load_task_register(UW);

/* is.S */
extern void outb(UH, UB);
extern void outw(UH, UH);
extern void outl(UH, UW);
extern B inb(UH);
extern H inw(UH);
extern W inl(UH);
#endif

#endif /* __ITRON_FUNC_H__ */
