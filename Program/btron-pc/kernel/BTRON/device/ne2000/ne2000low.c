/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/ne2000/ne2000low.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcs[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/ne2000/ne2000low.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/* 
 * $Log: ne2000low.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.10  1999-03-07 12:50:25  night
 * ne2000 アダプタへの書き込み処理の変更。
 *
 * Revision 1.9  1999/03/04 16:23:06  night
 * copy_to_ne2000() の作成。copy_to_ne2000() は、ne2000 内部のバッ
 * ファへ送信したいデータをコピーするための関数。
 *
 * Revision 1.8  1999/03/04 15:55:00  night
 * write_frame() の中身を作成(今までのは捨てた)。
 *
 * Revision 1.7  1999/03/02 15:21:21  night
 * 初期化処理の変更。
 *
 * Revision 1.6  1999/02/28 22:15:27  night
 * 関数 reset_adaptor() の変更。
 *
 * Revision 1.5  1997/11/10 14:44:58  night
 * 1 フレーム分のデータの書き込み処理の追加。
 *
 * Revision 1.3  1997/10/28 14:45:05  night
 * port_table[] の型を UW から W に変更。
 *
 * Revision 1.2  1997/10/28 12:48:00  night
 * 文字コードを SJIS から EUC に変更。
 * probe 処理の追加。
 *
 * Revision 1.1  1997/05/06 12:43:31  night
 * 最初の登録
 *
 *
 */

/*
 *
 *
 */

#include "ne2000.h"
#include "ne2000_internal.h"



W port_table[] =
{
  0x300,
  0x280,
  0x320,
  0x340,
  0x360,
  -1,
};

W intr_table[] =
{
  3,
  -1,
};


UW	initilized = 0;


extern ER	copy_to_ne2000 (struct ne2000_device *dev, B *buf, UH tx_buf, W buflen);



/* NE2000 Ethernet アダプタが存在しているかどうかをチェックする
 */
ER
probe (struct ne2000_device *dev)
{
  W	counter;

  /* PORT アドレスの検出 */
  for (counter = 0; port_table[counter] > 0; counter++)
    {
      if (port_table[counter] != 0)
	{
	  IO_BASE(dev) = port_table[counter];
	  if (probe_1 (dev) == E_OK)
	    break;
	}
    }
  if (port_table[counter] <= 0)
    {
      dbg_printf ("Can not probe: ne2000\n");
      return (E_DEV);
    }

  port_table[counter] = 0;	/* このアドレスは、すでに登録されたことをマーク */

  return (E_OK);
}


ER
probe_1 (struct ne2000_device *dev)
{  
  W	regd;
  W	reg0;
  W	reg;

#ifdef DEBUG
  printf ("probe_1: base address = 0x%x\n", IO_BASE (dev));
#endif

  reset_adaptor (dev);

  reg = inb (IO_BASE (dev));

  select_page1 (dev);

  regd = inb (IO_BASE (dev) + 0x0d);
  outb (IO_BASE (dev) + 0x0d, 0xff);
  outb (IO_CMD (dev), E8390_NODMA + E8390_PAGE0);
  inb (IO_BASE (dev) + EP0_COUNTER0); /* Clear the counter by reading. */
  if (inb (IO_BASE (dev) + EP0_COUNTER0) != 0) {
    outb (IO_BASE (dev), reg0);
    outb (IO_BASE (dev) + 0x0d, regd);	/* Restore the old values. */
    dbg_printf ("NE*000: ethernetcard probe at 0x%x\n", IO_BASE (dev));
    return (E_DEV);
  }

  return (E_OK);
}


ER
reset_adaptor (struct ne2000_device *dev)
{
  W	i;
  UB	data;

  printf ("NE2000: reset\n");

  data = inb (IO_RESET (dev));
  outb (IO_RESET (dev), data);

  for (i = 0; i < 30000; i++)
    {
      data = inb (IO_BASE (dev) + EP0_ISR);
      if (data & EPISR_RESET)
	{
	  break;
	}
    }

  return (E_OK);
}


/* NE2000 アダプタについての情報の初期化
 */
ER
init_device (struct ne2000_device *dev)
{
  dev->tx_busy = 0;
  dev->tx_use = 0;
  dev->tx_new = 0;
  dev->tx_next = 0;
  dev->rx_next = RX_PAGE_START;
  dev->intr_number  = NE2000_DEFAULT_INTR;
  dev->intr_handler = ne2000_interrupt_handler;

  /* 割り込みハンドラの設定
   */
  set_interrupt_handler (dev, dev->intr_number,dev->intr_handler); 


  /* 各種レジスタの設定 */
  select_page0 (dev);
  outb (IO_BASE (dev) + EP0_DCFG, 0x49);

  outb (IO_BASE (dev) + EP0_RCNTLO, 0x00);	/* リモートカウンタリセット */
  outb (IO_BASE (dev) + EP0_RCNTHI, 0x00);
  
  outb (IO_BASE (dev) + EP0_RXCR, E8390_RXOFF);

  outb (IO_BASE (dev) + EP0_TXCR, E8390_TXOFF);

  outb (IO_BASE (dev) + EP0_TPSR, TX_PAGE_START);
  outb (IO_BASE (dev) + EP0_STARTPG, RX_PAGE_START);

  outb (IO_BASE (dev) + EP0_STOPPG, RX_PAGE_END);
  outb (IO_BASE (dev) + EP0_BOUNDARY, RX_PAGE_START);

  outb (IO_BASE (dev) + EP0_ISR, 0xff); /* 割り込みステータスリセット */

  /* 割り込み許可 */
  outb (IO_BASE (dev) + EP0_IMR, EPISR_RX | EPISR_TX | EPISR_RX_ERR | EPISR_TX_ERR | EPISR_OVER );

  select_page1 (dev);

  outb (IO_BASE (dev) + EP1_CURPAG, dev->rx_next);

  select_page0 (dev);

  /* パケット受信許可 ＆ ブロードキャストパケット許可 */
  outb (IO_BASE (dev) + EP0_RXCR, 0x04);


  outb (IO_BASE (dev) + EP0_TXCR, 0x00);					/* 送信許可 */
  outb (IO_BASE (dev) + E8390_CMD, E8390_NODMA + E8390_PAGE0 + E8390_START);	/* 動作開始 */

  return (E_OK);
}



/* init_ne2000 --- NE2000 ボードの初期化
 * 
 */
W
init_adaptor (struct ne2000_device *dev)
{
  W saprom_index;
  W macaddr_index;

  dev->word_access = 2;

  reset_adaptor (dev);

  outb (IO_CMD (dev), E8390_PAGE0 + E8390_START + E8390_NODMA);	/* page0 */

  /* コマンドを設定 */
  outb (IO_BASE (dev) + EP0_DCFG, 0x48);	/* Byte 毎のアクセスを */
						/* 指定 */
  outb (IO_BASE (dev) + EP0_RCNTLO, 0x00);	/* カウントレジスタを */
						/* クリア */
  outb (IO_BASE (dev) + EP0_RCNTHI, 0x00);
  outb (IO_BASE (dev) + EP0_IMR, 0x00);		/* 割り込みをマスク */
  outb (IO_BASE (dev) + EP0_ISR, 0xff);
  outb (IO_BASE (dev) + EP0_RXCR, E8390_RXOFF);	/* 受信を off */
  outb (IO_BASE (dev) + EP0_TXCR, E8390_TXOFF);	/* 送信を off */


  /* SAPROM の内容を読み込む */
  outb (IO_BASE (dev) + EP0_RCNTLO, 32);	/* 転送バイト数 */
  outb (IO_BASE (dev) + EP0_RCNTHI, 0);
  outb (IO_BASE (dev) + EP0_RSARLO, 0);
  outb (IO_BASE (dev) + EP0_RSARHI, 0);
  outb (IO_CMD (dev), E8390_RREAD + E8390_START);

  for (saprom_index = 0; saprom_index < 32; saprom_index += 2)
    {
      dev->sa_prom[saprom_index] = (UW)inb (IO_DATA (dev))     & 0xff;
      dev->sa_prom[saprom_index + 1] = (UW)inb (IO_DATA (dev)) & 0xff;

      if (dev->sa_prom[saprom_index] != dev->sa_prom[saprom_index + 1])
	{
	  dev->word_access = 1;
	}
    }

#ifdef DEBUG
  printf ("word access: %d\n", dev->word_access);
#endif

  if (dev->word_access == 2)
    {
      for (saprom_index = 0; saprom_index < 16; saprom_index++)
	{
	  dev->sa_prom[saprom_index] = dev->sa_prom[saprom_index + saprom_index];
	}
    }
  else
    {
      return (E_NOSPT);
    }

#ifdef DEBUG
  printf ("NE*000: Mac address   ");
  for (saprom_index = 0; saprom_index < MACADDRLEN - 1 ; saprom_index++)
    {
      printf ("0x%x:", dev->sa_prom[saprom_index]);
    }
  printf ("0x%x\n", dev->sa_prom[saprom_index]);
#endif  

  select_page1 (dev);
  for (macaddr_index = 0; macaddr_index < MAC_ADDR_LENGTH; macaddr_index++)
    {
      outb (IO_BASE (dev) + EP1_PHYS + macaddr_index, dev->sa_prom[macaddr_index]);
    }

  return (E_OK);
}



/* read_frame - データを 1 フレーム分読み込む
 *
 * dev - データを読み込むアダプタ
 * buf - データを入れておくバッファ
 * buflen - 読み込んだデータの長さ
 *
 */
ER
read_frame (struct ne2000_device *dev, B *buf, W *buflen)
{
  
}


/* ne2000_transmit - NE2000 に対して送信要求を出す
 *
 *
 *
 *
 */
void
ne2000_transmit (struct ne2000_device *dev)
{
  UH	length;

  length = dev->tx_len[ dev->tx_next ];	/* 送信するデータの長さを代入 */

  printf ("ne2000_transmit: %d\n", __LINE__);
  /* select page 0 */
  select_page0 (dev);

  /* 送信バッファスタートページ設定 */
  outb (IO_CMD (dev) + EP0_TPSR, TX_PAGE_START + dev->tx_next * NE2000_PAGE_SIZE);

  /* 送信長 設定 */
  outb (IO_CMD (dev) + EP0_TCNTLO, length & 0xff);
  outb (IO_CMD (dev) + EP0_TCNTHI, length >> 8);

  /* 送信要求発行 */
  outb (IO_CMD (dev), E8390_NODMA | E8390_PAGE0 | E8390_TRANS | E8390_START);

  dev->tx_busy = 1;

  printf ("ne2000_transmit: %d\n", __LINE__);
  /* 未送信バッファブロックをつぎのブロックへ */
  dev->tx_next++;
  if (dev->tx_next == MAX_TX_BUF ) dev->tx_next = 0; /* 送信バッファを０へ */
}



/* write_frame - データを 1 フレーム分送信する。
 *
 *	送信処理は、次のように行う。
 *	1) 送信中かどうかのチェック(送信中ならば、エラーで抜ける)
 *	2) アダプタの送信バッファに未送信のバッファがあるかどうかをチェック
 *	   (あるならば、その内容を送信)
 *	3) アダプタの送信用のバッファに空きがあるかをチェック
 *	   (空きがなければ、エラーで抜ける)
 *	4) 送信する。
 *
 */
ER
write_frame (struct ne2000_device *dev, B *buf, W buflen)
{
  UH	tx_buf;	/* アダプタの送信バッファのインデックス */


  if (buflen > MAX_MTU_LENGTH)
    {
      /* パラメータが変 */
      return (E_PAR);
    }

  if (dev->tx_use > 0)
    {
      /* 未送信バッファがあったので、送信する */
      ne2000_transmit (dev);
    }

  if (dev->tx_busy == 1)
    {
      /* アダプタは、送信中 */
      return (E_DEV);
    }

  if (dev->tx_use >= MAX_TX_BUF)
    {
      /* 空きバッファがない */
      return (E_DEV);
    }

  tx_buf = LOC_MEM_START + (dev->tx_new * TX_BUF_SIZE * NE2000_PAGE_SIZE);

  copy_to_ne2000 (dev, buf, tx_buf, buflen);

  if ( buflen > MIN_ETHER_LENGTH) 
    {
      dev->tx_len[dev->tx_new] = buflen;
    }
  else
    {
      dev->tx_len[dev->tx_new] = MIN_ETHER_LENGTH;
    }
  dev->tx_use++;

  dev->tx_new++;
  if (dev->tx_new == MAX_TX_BUF)
    {
      dev->tx_new = 0;
    }

  if (dev->tx_busy == 0)
    {
printf ("%s: %d\n", __FILE__, __LINE__);
      ne2000_transmit (dev);
printf ("%s: %d\n", __FILE__, __LINE__);
    }

  return (E_OK);
}



/* copy_from_ne2000 --- NE2000 のバッファから受信したデータをコピーする
 *
 *
 */
ER
copy_from_ne2000 (struct ne2000_device *dev, B *buf, UH tx_buf, W buflen)
{
  return (E_NOSPT);
}



/* copy_to_ne2000 --- NE2000 のバッファヘ送信したいデータをコピーする
 *
 */
ER
copy_to_ne2000 (struct ne2000_device *dev, B *buf, UH tx_buf, W buflen)
{
  W 	maxwait = 100; /* about 120usらしい */
  UB	savebyte[2];

  /* select page 0 */
  select_page0 (dev);

  /* Reset remote DMA complete flag. */
  outb (IO_BASE (dev) + EP0_ISR, EPISR_RDC);

  /* 転送バイト数を設定 */
  outb (IO_BASE (dev) + EP0_RCNTLO, buflen & 0xff);
  outb (IO_BASE (dev) + EP0_RCNTHI, buflen >> 8);

  /* DP8390のローカルメモリ空間のアドレスを設定 */
  outb (IO_BASE (dev) + EP0_RSARLO, tx_buf & 0xff);
  outb (IO_BASE (dev) + EP0_RSARHI, tx_buf >> 8);

  /* DMA書き込み転送開始 */
  outb (IO_CMD (dev), (E8390_RWRITE + E8390_PAGE0 + E8390_START));

  /* データ転送 */
  if (buflen > 1)
    {
      W	i;

      for (i = 0; i < (buflen / 2); i++)
	{
	  UH	tmp;

	  tmp = *(UH *)buf;
	  outw (IO_DATA (dev), tmp);
	  buf += 2;
	}

      buflen &=1;
    }
      
  /* 残り1バイトの場合、0 を付加してコピー */
  if (buflen==1){
    savebyte[0] = *(buf - 1);
    savebyte[1] = 0;
    outw (IO_DATA (dev), *(UH *)savebyte);
  }

  /* 転送完了を待つ */
  while ((inb (IO_BASE (dev) + EP0_ISR) & EPISR_RDC) && (--maxwait))
    {
      ;
    }

  if (!maxwait)
    {
      select_page0 (dev);
      ne2000_transmit (dev);
    }

  return (E_OK);
}





/* page 0 の選択 
 */
void
select_page0 (struct ne2000_device *dev)
{
  outb (IO_CMD (dev), E8390_PAGE0 + E8390_START + E8390_NODMA);
}


/* page 1 の選択 
 */
void
select_page1 (struct ne2000_device *dev)
{
  outb (IO_CMD (dev), E8390_PAGE1 + E8390_START + E8390_NODMA);
}

