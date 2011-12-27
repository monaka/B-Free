/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* io.h --- デバイスドライバのための定義
 *
 *
 */

#ifndef __IO_IO_H__
#define __IO_IO_H__	1

/* デバイステーブルに入れる値 */
#define UNDEF_DEVICE	-1	/* このエントリは、使われていない */
#define ANY_DEVICE	-2	/* 任意のデバイス */

/* デバイスドライバの種類
 *
 * B-Free OS でのデバイスには、ブロック型のデバイスとキャラクタ型の
 * デバイスの2種類がある。
 *
 */
typedef enum io_type_t
{
  BLOCK,			/* ブロック型デバイス */
  CHAR,				/* キャラクタ型デバイス */
} IO_TYPE;


/* デバイスドライバのエラー番号の定義 
 */
typedef enum
{
  IO_OK = 	0,		/* 正常終了		*/
  IO_TIMEOUT =  1,		/* タイムアウト		*/
  IO_PERM =     2		/* パーミッションエラー */
} IO_ERR;


/* I/O コマンド --- ドライバへ送るためのコマンド
 */
typedef enum io_command_t
{
  IO_NULL =	0x0000,		/* 何もしないコマンド */
  IO_OPEN =	0x0001,		/* ドライバのオープン  */
				/* 一度にひとつのタスクだけが使用できるドライバの */
				/* 場合に使用する */
  IO_CLOSE =	0x0002,		/* ドライバのクローズ: 解放宣言 */
  IO_READ =	0x0003,		/* ドライバからのデータの読み取り */
  IO_WRITE =	0x0004,		/* ドライバへのデータの書き込み */
  IO_STAT = 	0x0005,		/* デバイス状態の読み取り */
  IO_CONTROL =  0x0006		/* デバイスのコントロール */
} T_IO_COMMAND;


/* IO_CONTROLコマンドの中で使用するサブコマンド(共通)
 *
 * これらの他にデバイス固有のサブコマンドを定義することもできる。
 * 固有のサブコマンドについては、これら共通サブコマンドと重ならないよう 
 * 先頭のビットを1にすること(0x8000 のように)。
 */
#define IO_SYNC			0x0001
#define IO_ASYNC		0x0002


/* 各I/Oコマンドの使用時にドライバへ渡すパケット */

 /* オープン時に使用するパケット */
struct io_open_packet
{
  UINT			device;	/* デバイス番号 */
  W			mode;	/* オープンモード */
};

struct io_close_packet
{
  UINT			device;	/* デバイス番号 */
};

struct io_read_packet
{
  W			offset;	/* read開始位置  */
				/* デバイスによっては使用しない */
  W			size;	/* 読み込むサイズ */
  VP			bufp;	/* データを読み込むバッファへのポインタ */
};

struct io_write_packet
{
  W			offset;	/* write開始位置 */
  W			size;	/* 書き込むサイズ */
  VP			bufp;	/* データを書き込むバッファへのポインタ */
};

struct io_stat_packet
{
  W			statid;
};

struct io_control_packet
{
  W			control;
  VP			argp;
};

typedef struct io_request_t
{
  T_IO_COMMAND	command;
  ID		taskid;		/* 要求元のタスクＩＤ */
  ID		resport;	/* レスポンス用のポート */

  union
    {
      struct io_open_packet	open_pack;
      struct io_close_packet	close_pack;
      struct io_read_packet	read_pack;
      struct io_write_packet	write_pack;
      struct io_stat_packet	stat_pack;
      struct io_control_packet	control_pack;
    } s;
} T_IO_REQUEST;


/* リクエストに対する答え。
 *
 * このパケットは、単にコマンドの実行に成功したかを返すだけ。
 *
 */
typedef struct io_response_t
{
  IO_ERR		stat;
  
} T_IO_RESPONSE;


#endif	/* __IO_IO_H__ */
