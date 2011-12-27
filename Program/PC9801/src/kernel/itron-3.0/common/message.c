/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* message.c --- メッセージバッファ生成
 *
 *
 */


#include "itron.h"
#include "config.h"
#include "memory.h"
#include "errno.h"
#include "func.h"
#include "pc98.h"
#include "message.h"


static T_MSGHEAD		message_table[NMSGBUF];
static T_MSG			msgbuf[MAX_MSGENTRY];
static T_MSG			*free_msg;



/****************************************************************************
 * alloc_msg
 */
static T_MSG *
alloc_msg (void)
{
  T_MSG	*p;

  if (free_msg == NULL)
    {
      return (NULL);
    }
  
  p = free_msg;
  free_msg = free_msg->next;
  return (p);
}

/****************************************************************************
 * dealloc_msg
 */
static void
dealloc_msg (T_MSG *p)
{
  p->next = free_msg;
  free_msg = p;
}

/***************************************************************************
 * add_msg_list --- メッセージリストにメッセージを追加する。
 *
 */
static void
add_msg_list (T_MSGHEAD *list, T_MSG *newmsg)
{
  dis_int ();
  if (list->last_message_ptr == NULL)
    {
      list->message_ptr = list->last_message_ptr = newmsg;
    }
  else
    {
      list->message_ptr->next = newmsg;
      list->message_ptr = newmsg;
    }
  ena_int ();
}

/***************************************************************************
 * get_msg --- メッセージリストからメッセージを取り出す
 *
 */
static T_MSG *
get_msg (T_MSGHEAD *list)
{
  T_MSG *p;

  if (list->message_ptr == NULL)
    return (NULL);

  dis_int ();
  p = list->message_ptr;
  if (p == list->last_message_ptr)
    {
      list->message_ptr = list->last_message_ptr = NULL;
    }
  else
    {
      list->message_ptr = p->next;
    }
  ena_int ();
  return (p);
}

/****************************************************************************
 * init_msgbuf --- メッセージバッファ機能の初期化
 *
 * 引数：
 *	なし
 *
 * 返り値：
 *	エラー番号
 */
ER
init_msgbuf (void)
{
  T_MSG	*p;
  INT	i;

  free_msg = &msgbuf[0];
  for (i = 0; i < MAX_MSGENTRY; i++)
    {
      msgbuf[i].next = &msgbuf[i + 1];
    }
  msgbuf[MAX_MSGENTRY].next = NULL;

  for (i = 0; i < NMSGBUF; i++)
    {
      message_table[i].mbfatr = TA_UNDEF;
    }
  return (E_OK);
}


/****************************************************************************
 * cre_mbf -- メッセージバッファ生成
 *
 * 引数：
 *	id	メッセージバッファ ID
 *	pk_cmbf	メッセージバッファ生成情報
 *		mbfatr	メッセージ属性 (TA_FIFO 固定)
 *		bufsz	バッファサイズ
 *		maxmsz	メッセージの最大長
 *
 * 返り値：
 *	ER	エラー番号
 *		E_OK	正常終了
 *		E_ID	メッセージ ID 不正
 *		E_OBJ	同一ID のメッセージがすでに存在する
 *		E_PAR	パラメータ不正
 *
 * 機能：
 *	メッセージバッファを生成する。
 */
ER
cre_mbf (ID id, T_CMBF *pk_cmbf)
{
  if ((id < MIN_MSGBUF) || (id > MAX_MSGBUF))
    {
      return (E_ID);
    }
  if ((pk_cmbf->mbfatr != TA_TFIFO) || (pk_cmbf->bufsz <= 0) || (pk_cmbf->maxmsz <= 0))
    {
      return (E_PAR);
    }
  if (message_table[id].mbfatr != TA_UNDEF)
    {
/*      printf ("cre_msg: message_table[id].mbfatr = %d\n", message_table[id].mbfatr);	/* */
      return (E_OBJ);
    }

  message_table[id].total_size = pk_cmbf->bufsz;
  message_table[id].message_ptr = NULL;
  message_table[id].msgsz = pk_cmbf->maxmsz;
  message_table[id].bufsz = pk_cmbf->bufsz;
  message_table[id].mbfatr = pk_cmbf->mbfatr;
  return (E_OK);
}

/**************************************************************************************
 * del_mbf --- メッセージバッファを削除する
 *
 * 引数：
 *	id	メッセージバッファ ID
 *
 * 返り値：
 *	ER	エラー番号
 *		E_OK	正常終了
 *		E_ID	メッセージ ID 不正
 *		E_NOEXS メッセージバッファが存在しない
 *
 * 機能：
 *	引数 ID で指定したメッセージバッファを削除する。
 *
 */
ER
del_mbf (ID id)
{
  T_TCB	*p;
  T_MSG	*msgp, *tmpp;

  if ((id < MIN_MSGBUF) || (id > MAX_MSGBUF))
    {
      return (E_ID);
    }
  if (message_table[id].mbfatr == TA_UNDEF)
    {
      return (E_NOEXS);
    }

  dis_int ();
  for (msgp = message_table[id].message_ptr; msgp != NULL; msgp = tmpp)
    {
      kfree (msgp->buf, msgp->size);
      tmpp = msgp->next;
      msgp->next = free_msg;
      free_msg = msgp;
    }

  /* 受信待ちタスクに対して E_DLT を返す */
  for (p = message_table[id].wait_recvtask; p != NULL; p = p->msg_next)
    {
      p->slp_err = E_DLT;
      p->tskwait.msg_wait = 0;
      wup_tsk (p->tskid);
    }
  message_table[id].wait_recvtask = NULL;

  /* 送信待ちタスクに対して E_DLT を返す */
  for (p = message_table[id].wait_sendtask; p != NULL; p = p->msg_next)
    {
      p->slp_err = E_DLT;
      p->tskwait.msg_wait = 0;
      wup_tsk (p->tskid);
    }
  message_table[id].wait_sendtask = NULL;
  message_table[id].mbfatr = TA_UNDEF;
  ena_int (); 
  return (E_OK);
}

/*********************************************************************************
 * snd_mbf --- メッセージバッファへ送信
 *
 * 引数：
 *	id	メッセージバッファ ID
 *	size	送信メッセージのサイズ
 *	msg	送信メッセージの先頭アドレス
 *
 * 返り値：
 *	ER	エラー番号
 *
 * 機能：
 *	
 */
ER
snd_mbf (ID id, INT size, VP msg)
{
  T_MSG	*newmsg;
  VP	buf;

  if ((id < MIN_MSGBUF) || (id > MAX_MSGBUF))
    {
      return (E_ID);
    }
  if (message_table[id].mbfatr == TA_UNDEF)
    {
      return (E_NOEXS);
    }

retry:
  newmsg = alloc_msg ();
  if ((newmsg == NULL) || (message_table[id].total_size < size))
    {
      /* メッセージバッファがアロケートできなかった。 */
      /* 送信待ちリストに入れ、sleep する。	      */
      message_table[id].wait_sendtask = add_tcb_list (message_table[id].wait_sendtask,
						      run_task);
      run_task->tskwait.msg_wait = 1;
      slp_tsk ();
      goto retry;
    }

  buf = kalloc (size);
  if (buf == NULL)
    {
      dealloc_msg (newmsg);
      return (E_OBJ);
    }

  newmsg->buf = buf;
  newmsg->size = size;
  add_msg_list (&message_table[id], newmsg);
  bcopy (msg, buf, size);
  if (message_table[id].wait_recvtask)
    {
      T_TCB *p;

      /* もし、受信待ちタスクがあれば、それを wakeup する */
      p = message_table[id].wait_recvtask;
      message_table[id].wait_recvtask = del_tcb_list (message_table[id].wait_recvtask, p);
      p->tskwait.msg_wait = 0;
      wup_tsk (p->tskid);
    }

  message_table[id].total_size -= size;
  return (E_OK);
}

/*********************************************************************************
 * snd_mbf --- メッセージバッファへ送信
 *
 * 引数：
 *	id	メッセージバッファ ID
 *	size	送信メッセージのサイズ
 *	msg	送信メッセージの先頭アドレス
 *
 * 返り値：
 *	ER	エラー番号
 *
 * 機能：
 *	
 */
ER
snd_mbf2 (ID id, INT size, VP msg)
{
  T_MSG	*newmsg;
  VP	buf;

  if ((id < MIN_MSGBUF) || (id > MAX_MSGBUF))
    {
      return (E_ID);
    }
  if (message_table[id].mbfatr == TA_UNDEF)
    {
      return (E_NOEXS);
    }

retry:
  newmsg = alloc_msg ();
  if ((newmsg == NULL) || (message_table[id].total_size < size))
    {
      /* メッセージバッファがアロケートできなかった。 */
      /* 送信待ちリストに入れ、sleep する。	      */
      message_table[id].wait_sendtask = add_tcb_list (message_table[id].wait_sendtask,
						      run_task);
      run_task->tskwait.msg_wait = 1;
      slp_tsk ();
      goto retry;
    }

  buf = kalloc (size);
  if (buf == NULL)
    {
      printf ("snd_mbf2: can not allocate memory.\n");	/* */
      dealloc_msg (newmsg);
      return (E_OBJ);
    }

  newmsg->buf = buf;
  newmsg->size = size;
  add_msg_list (&message_table[id], newmsg);
  bcopy (msg, buf, size);

  if (message_table[id].wait_recvtask)
    {
      T_TCB *p;

      /* もし、受信待ちタスクがあれば、それを wakeup する */
      p = message_table[id].wait_recvtask;
      message_table[id].wait_recvtask = del_tcb_list (message_table[id].wait_recvtask, p);
      p->tskwait.msg_wait = 0;
      printf ("snd_mbf2: wait task.(ID = %d\n", p->tskid);	/** */
      wup_tsk (p->tskid);
    }
  else
    {
      printf ("snd_mbf2: not wait task.\n");	/** */
    }

  message_table[id].total_size -= size;
  return (E_OK);
}


/*********************************************************************************
 * psnd_mbf --- メッセージバッファへ送信(ポーリング)
 *
 * 引数：
 *	id	メッセージバッファ ID
 *	size	送信メッセージのサイズ
 *	msg	送信メッセージの先頭アドレス
 *
 * 返り値：
 *	ER	エラー番号
 *
 * 機能：
 *	
 */
ER
psnd_mbf (ID id, INT size, VP msg)
{
  T_MSG	*newmsg;
  VP	buf;

  if ((id < MIN_MSGBUF) || (id > MAX_MSGBUF))
    {
      return (E_ID);
    }
  if (message_table[id].mbfatr == TA_UNDEF)
    {
      return (E_NOEXS);
    }

  newmsg = alloc_msg ();
  if ((newmsg == NULL) || (message_table[id].total_size < size))
    {
      /* メッセージバッファがアロケートできなかった。 */
      /* E_TMOUT のエラーを返す			      */
      return (E_TMOUT);
    }

  buf = kalloc (size);
  if (buf == NULL)
    {
      dealloc_msg (newmsg);
      return (E_TMOUT);
    }

  newmsg->buf = buf;
  newmsg->size = size;
  add_msg_list (&message_table[id], newmsg);
  bcopy (msg, buf, size);
  if (message_table[id].wait_recvtask)
    {
      T_TCB *p;

      /* もし、受信待ちタスクがあれば、それを wakeup する */
      p = message_table[id].wait_recvtask;
      message_table[id].wait_recvtask = del_tcb_list (message_table[id].wait_recvtask, p);
      p->tskwait.msg_wait = 0;
      wup_tsk (p->tskid);
    }

  message_table[id].total_size -= size;
  return (E_OK);
}

/*********************************************************************************
 * tsnd_func --- タイムアウト時間が過ぎた場合に実行する関数
 *
 * 引数：
 *	タイムアウトしたタスク
 *
 * 返り値：
 *	なし
 *
 * 機能：
 *	本関数は、tsnd_mbf によってメッセージバッファ待ちしていたタスクが
 *	タイムアウトした場合に呼びだされる。
 */
static void
tsnd_func (VP p)
{
  T_TCB *taskp;

  dis_int ();
  taskp = (T_TCB *)p;
  taskp->slp_err = E_TMOUT;
  taskp->tskwait.msg_wait = 0;
  wup_tsk (taskp->tskid);
  ena_int ();
}
/*********************************************************************************
 * tsnd_mbf --- メッセージバッファへ送信(タイムアウト機能つき)
 *
 * 引数：
 *	id	メッセージバッファ ID
 *	size	送信メッセージのサイズ
 *	msg	送信メッセージの先頭アドレス
 *	tmout	タイムアウト値
 *
 * 返り値：
 *	ER	エラー番号
 *
 * 機能：
 *	
 */
ER
tsnd_mbf (ID id, INT size, VP msg, TMO tmout)
{
  T_MSG	*newmsg;
  VP	buf;

  if ((id < MIN_MSGBUF) || (id > MAX_MSGBUF))
    {
      return (E_ID);
    }
  if (message_table[id].mbfatr == TA_UNDEF)
    {
      return (E_NOEXS);
    }

retry:
  newmsg = alloc_msg ();
  if ((newmsg == NULL) || (message_table[id].total_size < size))
    {
      /* メッセージバッファがアロケートできなかった。 */
      /* 送信待ちリストに入れ、sleep する。	      */
      set_timer (tmout, tsnd_func, run_task);
      message_table[id].wait_sendtask = add_tcb_list (message_table[id].wait_sendtask,
						      run_task);
      run_task->tskwait.msg_wait = 1;
      slp_tsk ();
      unset_timer (tsnd_func, run_task);
      if (run_task->slp_err)
	return (run_task->slp_err);
      goto retry;
    }

  buf = kalloc (size);
  if (buf == NULL)
    {
      dealloc_msg (newmsg);
      return (E_OBJ);
    }

  newmsg->buf = buf;
  newmsg->size = size;
  add_msg_list (&message_table[id], newmsg);
  bcopy (msg, buf, size);
  if (message_table[id].wait_recvtask)
    {
      T_TCB *p;

      /* もし、受信待ちタスクがあれば、それを wakeup する */
      p = message_table[id].wait_recvtask;
      message_table[id].wait_recvtask = del_tcb_list (message_table[id].wait_recvtask, p);
      p->tskwait.msg_wait = 0;
      wup_tsk (p->tskid);
    }

  message_table[id].total_size -= size;
  return (E_OK);
}

/*********************************************************************************
 * rcv_mbf --- メッセージバッファから受信する
 *
 * 引数：
 *	msg	受信したメッセージを入れる領域
 *	id	メッセージバッファ ID
 *	size	受信したメッセージのサイズ(返り値)
 *
 * 返り値：
 *	ER	エラー番号
 *
 * 機能：
 *
 */
ER
rcv_mbf (VP msg, INT *size, ID id)
{
  T_MSG	*newmsgp;

  if ((id < MIN_MSGBUF) || (id > MAX_MSGBUF))
    {
      return (E_ID);
    }
  if (message_table[id].mbfatr == TA_UNDEF)
    {
      return (E_NOEXS);
    }

/*  printf ("(1)\n");	/* */

  /* メッセージが存在しない --> sleep する。 */
  if (message_table[id].message_ptr == NULL)
    {
      message_table[id].wait_recvtask = add_tcb_list (message_table[id].wait_recvtask,
						      run_task);
      run_task->tskwait.msg_wait = 1;
      slp_tsk ();
    }


  /* メッセージリストから1つエントリを取り出す */
  newmsgp = get_msg (&message_table[id]);
  /* メッセージの内容をコピーする */
  bcopy ((VP)newmsgp->buf, msg, newmsgp->size);
  *size = (INT)newmsgp->size;
  kfree (newmsgp->buf, *size);
  dealloc_msg (newmsgp);
  message_table[id].total_size += *size;
  return (E_OK);
}

/*********************************************************************************
 * prcv_mbf --- メッセージバッファから受信する (ポーリング)
 *
 * 引数：
 *	msg	受信したメッセージを入れる領域
 *	id	メッセージバッファ ID
 *	size	受信したメッセージのサイズ(返り値)
 *
 * 返り値：
 *	ER	エラー番号
 *
 * 機能：
 *
 */
ER
prcv_mbf (VP msg, INT *size, ID id)
{
  T_MSG	*newmsgp;

  if ((id < MIN_MSGBUF) || (id > MAX_MSGBUF))
    {
      return (E_ID);
    }
  if (message_table[id].mbfatr == TA_UNDEF)
    {
      return (E_NOEXS);
    }

  /* メッセージが存在しない --> エラーリターンする */
  if (message_table[id].message_ptr == NULL)
    {
      return (E_TMOUT);
    }

  /* メッセージリストから1つエントリを取り出す */
  newmsgp = get_msg (&message_table[id]);

  /* メッセージの内容をコピーする */
  bcopy (newmsgp->buf, msg, newmsgp->size);
  *size = newmsgp->size;
 
  kfree (newmsgp->buf, *size);
  dealloc_msg (newmsgp);
  message_table[id].total_size += *size;
  return (E_OK);
}

/*********************************************************************************
 * trcv_func --- 受信待ちの場合のタイムアウト関数
 *
 */
static void
trcv_func (VP p)
{
  T_TCB *taskp;

  dis_int ();
  taskp = (T_TCB *)p;
  taskp->slp_err = E_TMOUT;
  taskp->tskwait.msg_wait = 0;
  wup_tsk (taskp->tskid);
  ena_int ();
}


/*********************************************************************************
 * trcv_mbf --- メッセージバッファから受信する(タイムアウト有)
 *
 * 引数：
 *	msg	受信したメッセージを入れる領域
 *	id	メッセージバッファ ID
 *	size	受信したメッセージのサイズ(返り値)
 *
 * 返り値：
 *	ER	エラー番号
 *
 * 機能：
 *
 */
ER
trcv_mbf (VP msg, INT *size, ID id, TMO tmout)
{
  T_MSG	*newmsgp;

  if ((id < MIN_MSGBUF) || (id > MAX_MSGBUF))
    {
      return (E_ID);
    }
  if (message_table[id].mbfatr == TA_UNDEF)
    {
      return (E_NOEXS);
    }

retry:
  /* メッセージが存在しない --> sleep する。 */
  if (message_table[id].message_ptr == NULL)
    {
      dis_int ();
      set_timer (tmout, trcv_func, run_task);
      message_table[id].wait_recvtask = add_tcb_list (message_table[id].wait_recvtask,
						      run_task);
      ena_int ();
      run_task->tskwait.msg_wait = 1;
      slp_tsk ();
      unset_timer (tsnd_func, run_task);
      if (run_task->slp_err)
	return (run_task->slp_err);
      goto retry;
    }

  /* メッセージリストから1つエントリを取り出す */
  newmsgp = get_msg (&message_table[id]);

  /* メッセージの内容をコピーする */
  bcopy (newmsgp->buf, msg, newmsgp->size);
  *size = newmsgp->size;
 
  kfree (newmsgp->buf, *size);
  dealloc_msg (newmsgp);
  message_table[id].total_size += *size;
  return (E_OK);
}

/****************************************************************************************
 * ref_mbf --- メッセージバッファ状態参照
 *
 * 引数：
 *	pk_rmbf	メッセージバッファ状態 (返り値)
 *	id	メッセージ ID
 */
ER
ref_mbf (T_RMBF *pk_rmbf, ID id)
{
  if ((id < MIN_MSGBUF) || (id > MAX_MSGBUF))
    {
      return (E_ID);
    }
  if (message_table[id].mbfatr == TA_UNDEF)
    {
      return (E_NOEXS);
    }

  if (message_table[id].wait_recvtask != NULL) 
    pk_rmbf->wtsk = TRUE;
  else
    pk_rmbf->wtsk = FALSE;

  if (message_table[id].wait_sendtask != NULL) 
    pk_rmbf->stsk = TRUE;
  else
    pk_rmbf->stsk = FALSE;
  pk_rmbf->frbufsz = message_table[id].total_size;
  return (E_OK);
}
