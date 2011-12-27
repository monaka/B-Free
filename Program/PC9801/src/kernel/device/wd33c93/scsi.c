/*

Copyright 1994,1995 (C) B-Free Project.


B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

*/
/* $Id: scsi.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/wd33c93/scsi.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

/*
 * $Log: scsi.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.1  1995-12-05 15:18:03  night
 * 最初の登録。
 * このバージョンでは、デバイスから来たエラーのチェックを行っていないので、
 * 注意すること。
 *
 *
 */

#include "scsi.h"

#define SET_REG(regno, val) { \
		outb(SCSI_ADR_REG, (regno)); \
		outb(SCSI_CTL_REG, (val)); \
	}

#define SET_XFER_LEN(val) { \
		SET_REG(REG_TFR_COUNT_H, ((val) & 0xff0000) >> 16); \
		SET_REG(REG_TFR_COUNT_M, ((val) & 0x00ff00) >> 8); \
		SET_REG(REG_TFR_COUNT_L, (val) & 0x0000ff); \
	}

#define DMA_ENABLE() outb(SCSI_CMD_WRT, 1)


static int	interrupt_flag;


/*
 * wd33c93 のレジスタにデータを書きこむ
 */
W
write_wd33c93 (W reg, UB data)
{
  outb (SCSI_ADR_REG, reg);
  outb (SCSI_CTL_REG, data);
}

/*
 * wd33c93 のレジスタから情報を読み込む
 */
W
read_wd33c93 (W reg)
{
  outb (SCSI_ADR_REG, reg);
  return (inb (SCSI_CTL_REG));
}

/*
 * アダプタの存在の有無を調べる
 *
 */
probe ()
{
  UB	*findid;

  findid = (UB*)0x80000482;
  dbg_printf ("findid = %d\n", *findid);
#ifdef notdef
  if ((*findid) == 0)
    {
      return (FALSE);
    }
#endif /* notdef */
  return (TRUE);
}

/*
 * SCSI コントローラおよび SCSI 機器をリセットする
 * 
 */
W
reset_scsi ()
{
  UW	as_dma_chan;
  UW	as_irq_number;

  /* read SCSI dma channel */
  as_dma_chan = (inb(SCSI_STAT_RD) & 3);
/*  dbg_printf ("SCSI: dma channel: %d\n", as_dma_chan);	/* */
	
  /* setup WD33C93 */
  inb(SCSI_AUX_REG);			/* dummy read */
  read_wd33c93(REG_SCSI_STATUS);	/* dummy read */
	
  /* disable interrupt */
  write_wd33c93(REG_MEM_BANK, (read_wd33c93(REG_MEM_BANK) & 0xfb));

  /* read SCSI interrupt number */
  as_irq_number = ((read_wd33c93(0x33) >> 3) & 7);

/*  dbg_printf ("SCSI: IRQ is %d\n", as_irq_number); */
  switch (as_irq_number)
    {
    case 0:
      adaptor.intrn = 35;
      break;
    case 1:
      adaptor.intrn = 37;
      break;
    case 2:
      adaptor.intrn = 38;
      break;
    case 3:
      adaptor.intrn = 41;
      break;
    case 4:
      adaptor.intrn = 12;
      break;
    case 5:
      adaptor.intrn = 44;
      break;
    }
  adaptor.dma_chan = (inb(SCSI_STAT_RD) & 0x03);

  /* set Host ID */
  write_wd33c93(REG_OWN_ID, (read_wd33c93(0x33) & 7));
  do {
    /* reset command */
    write_wd33c93(REG_COMMAND, CMD_RESET);
    /* wait aux. interrupt */
    while ((inb(SCSI_AUX_REG) & 0x80) == 0) {}
  } while (read_wd33c93(REG_SCSI_STATUS) != 0);

  /* selection disable */
  write_wd33c93(REG_SRC_ID, 0);
  /*write_wd33c93(REG_DST_ID, 0);*/
  /* Set Timeout Period */
  write_wd33c93(REG_TIMEOUT_PERIOD, 0xa0);
	
  /* End Disconnect Interrupt */
  write_wd33c93(REG_CONTROL, 0x08);
  /* Synchronous Transfer = 0 */
  write_wd33c93(REG_SYNC_TFR, 0);
  
  /*inb(SCSI_AUX_REG);*/	/* dummy read */
  /* get scsi bus status */
  scsi_bus_status = read_wd33c93(REG_SCSI_STATUS);
  dbg_printf ("SCSI: scsi_bus_status = %d\n", scsi_bus_status); 
  /* enable interrupt */
  write_wd33c93(REG_MEM_BANK, (read_wd33c93(REG_MEM_BANK) | 0x4));
/*  dbg_printf ("scsi: end initialize.\n"); */
  return (TRUE);
}

/*
 * 割り込みハンドラルーチン
 */
void
intr_scsi ()
{
  interrupt_flag = 1;
/*  dbg_printf ("scsi: intr.\n"); */
}

/*
 * SCSI ステータス情報を取得する
 */
get_scsi_status (void)
{
}


/*
 * SCSI デバイスにコマンドを送信する。
 *
 * 引数:
 *	target	送信先の SCSI ID
 *	lun	送信先の SCSI LUN
 *	cmd	コマンド列 (バイト単位)
 *	cmdlen	コマンド列の長さ
 *
 */
W
send_command (W target, W lun, UB *cmd, W cmdlen)
{
  int i;

/*  dbg_printf ("SCSI: send_command\n"); */
#if 1
  SET_REG(SCSI_cdbsize, (cmdlen));
  for (i = 0; i < (cmdlen); i++) {
/*    dbg_printf ("[%d] 0x%x    ", i, cmd[i]); */
    SET_REG(SCSI_cdb1 + i, cmd[i]);
  }
#else
  dbg_printf("[cdb");
  outb(SCSI_ADR_REG, 0x3);
  for (i = 0; i < cmdlen; i++) {
    outb(SCSI_CTL_REG, cmd[i]);
    dbg_printf("(%x)", cmd[i]);
  }
#endif
}

scsi_exec (UW target, UW lun, UW cdblen, UB *cdb, UW buflen, UB *buf, UW read_flag)
{
  W	i, j;
  UW	stat;

/*  dbg_printf ("read data start\n"); */
  /* disable interrupt */
  write_wd33c93(REG_MEM_BANK, (read_wd33c93(REG_MEM_BANK) & 0xfb));

  /* SCSI コマンドの送信 */
  send_command (target, lun, cdb, cdblen);

  SET_XFER_LEN (buflen);
  write_wd33c93 (REG_DST_ID, target);
  write_wd33c93 (REG_TARGET_LUN, lun);
  write_wd33c93 (REG_CONTROL, 0x08);
  /* DMA_ENABLE(); */

  /* enable interrupt */
  interrupt_flag = 0;
  write_wd33c93(REG_MEM_BANK, (read_wd33c93(REG_MEM_BANK) | 0x4));
  write_wd33c93(REG_COMMAND, CMD_SELECT_NO_ATN_TFR);


  /* データの送受信 */
  for (i = 0; i < buflen; i++)
    {
      while (!(inb (SCSI_AUX_REG) & 0x01))
	;

      if (read_flag)
	buf[i] = read_wd33c93 (REG_DATA);
      else
	write_wd33c93 (REG_DATA, buf[i]);
    }

  while (!interrupt_flag)
    ;
      
  stat = inb (SCSI_AUX_REG);
  stat = read_wd33c93 (REG_SCSI_STATUS);

  /* もしエラーが発生していたらリカバーする */
  return (stat);
}


W
read_capacity (UW target, UW lun)
{
  UB	cdb[10];
  UB	buf[8];

  bzero (cdb, sizeof (cdb));
  cdb[0] = 0x25;
  cdb[1] = lun << 5;
  cdb[9] = 0;

  scsi_exec (target, lun, 10, cdb, 8, buf, 1);
  return ((buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | (buf[3]) + 1);
}

/*
 *
 */
inquery_scsi (W target, W lun)
{
  UB	buf[PAGE_SIZE];
  UB	cdb[6];
  W	i;
  UW	stat;

  bzero (buf, sizeof (buf));
/*  dbg_printf ("Inquery start\n"); */
  /* disable interrupt */
  write_wd33c93(REG_MEM_BANK, (read_wd33c93(REG_MEM_BANK) & 0xfb));

  /* SCSI ボードに与えるコマンド列の生成 */
  bzero (cdb, 6); 
  cdb[0] = 0x12; /* INQUIRY */
  cdb[4] = 36; /* allocation length */

  /* SCSI コマンドの送信 */
  scsi_exec (target, lun, 6, cdb, 36, buf, 1);

  dbg_printf ("Inquery: %d\n", target);
  dbg_printf ("data: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n",
	      buf[0],
	      buf[1],
	      buf[2],
	      buf[3],
	      buf[4],
	      buf[5]);
  dbg_printf ("HD name: %s\n", &buf[8]);
}

/*
 *
 */
W
read_block (W target, W lun, UW blockno, UW length, UB *buf)
{
  UB	cdb[10];
  W	i, j;
  UW	stat;
  UW	byte_length;

  byte_length = length * BLOCK_SIZE(target);
/*  dbg_printf ("read data start\n"); */
  /* disable interrupt */
  write_wd33c93(REG_MEM_BANK, (read_wd33c93(REG_MEM_BANK) & 0xfb));

  /* SCSI ボードに与えるコマンド列の生成 */
  bzero (cdb, 10);
  cdb[0] = 0x28;
  cdb[1] = 0;
  cdb[2] = (blockno >> 24) & 0xff;
  cdb[3] = (blockno >> 16) & 0xff;
  cdb[4] = (blockno >> 8)  & 0xff;
  cdb[5] = blockno & 0xff;
  cdb[6] = 0;
  cdb[7] = length >> 8;
  cdb[8] = length & 0xff;
  cdb[9] = 0;

  return (scsi_exec (target, lun, 10, cdb, byte_length, buf, 1));
}

/*
 *
 */
W
write_block (W target, W lun, UW blockno, UW length, UB *buf)
{
  UB	cdb[10];
  W	i, j;
  UW	stat;
  UW	byte_length;

  byte_length = length * BLOCK_SIZE(target);
/*  dbg_printf ("read data start\n"); */
  /* disable interrupt */
  write_wd33c93(REG_MEM_BANK, (read_wd33c93(REG_MEM_BANK) & 0xfb));

  /* SCSI ボードに与えるコマンド列の生成 */
  bzero (cdb, 10);
  cdb[0] = 0x2A;
  cdb[1] = 0;
  cdb[2] = (blockno >> 24) & 0xff;
  cdb[3] = (blockno >> 16) & 0xff;
  cdb[4] = (blockno >> 8)  & 0xff;
  cdb[5] = blockno & 0xff;
  cdb[6] = 0;
  cdb[7] = length >> 8;
  cdb[8] = length & 0xff;
  cdb[9] = 0;

  return (scsi_exec (target, lun, 10, cdb, byte_length, buf, 0));
}


/* 
 * モードセンス
 */
mode_sense (W target, W lun, W page, W buflen, UB *buf)
{
  UB	cdb[6];
  W	i, j;
  UW	stat;

/*  dbg_printf ("read data start\n"); */
  /* disable interrupt */
  write_wd33c93(REG_MEM_BANK, (read_wd33c93(REG_MEM_BANK) & 0xfb));

  /* SCSI ボードに与えるコマンド列の生成 */
  bzero (cdb, 6);
  cdb[0] = 0x1a;
  cdb[1] = (lun << 5);
  cdb[2] = page;
  cdb[3] = 0;
  cdb[4] = buflen;
  cdb[5] = 0;

  /* SCSI コマンドの送信 */
  return scsi_exec (target, lun, 6, cdb, buflen, buf, 1);
}


/*
 * BUSY Loop 
 */
void
busywait(W x)
{
  int i,j;
  int	tmp;

  for (i = 0; i < x; i++)
     for (j = 0; j < 100; j++)
       tmp = j;
}

/*************************************************************************
 * reset_intr_mask 
 *
 * 引数：	intn	割り込み番号
 *
 * 返値：	なし
 *
 * 処理：	割り込みマスクをリセットする。
 *
 */
void
reset_intr_mask (W intn)
{
  dis_int ();
  if (intn < 8)
    outb (MASTER_8259A_DATA, inb (MASTER_8259A_DATA) & ~(1 << intn));
  else
    outb (SLAVE_8259A_DATA, inb (SLAVE_8259A_DATA) & ~(1 << (intn - 8)));
  ena_int ();
}    
