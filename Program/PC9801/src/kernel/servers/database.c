/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#) $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/servers/database.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

static char rcs[] = "@(#) $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/servers/database.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";


/*
 * $Log: database.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.4  1995-12-05 15:16:28  night
 * 検索する時、ポートテーブルを 0 から 1 までしか調べていなかった。
 * これを MAX_PORT_ENTRY(= 100)まで検索するようにした。
 *
 * Revision 1.3  1995/09/21  15:51:48  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/06/26  15:18:56  night
 * いくつかの printf を DEBUG マクロで囲んだ。
 *
 * Revision 1.1  1995/03/18  14:12:47  night
 * 最初の登録
 *
 *
 */

/*
 * メッセージバッファをデータベース管理するためのモジュール。
 * 名前とメッセージバッファ ID とを結びつける。
 *
 * このモジュールは、外部に対して次の関数を公開する。
 *
 *   init_regist_table (void)
 *   regist_database (PORT_NAME *name, ID port, ID task)
 *   unregist_database (PORT_NAME *name, ID port, ID task)
 *   find_database (PORT_NAME *name, ID *port)
 *
 *
 * データの管理方法としては、単純なテーブル方式で管理する。
 * すなわち、空いている最初のエントリに登録し、探すときには、
 * 最初のエントリから順番に探していく。
 *
 */


/*
 * 必要なインクルードファイルの読み込み。
 */
#include <itron.h>
#include <errno.h>
#include <types.h>
#include "port-manager.h"


/*
 * データを登録するエントリ。
 * 名前とメッセージバッファの ID そして登録したタスクの ID の 
 * 3 つの情報が入る。
 * この構造体は、このファイルの中しか使わない。
 *
 */
struct data_entry_t
{
  PORT_NAME	name;		/* メッセージバッファに結びついた名前。   */
				/* 登録するときに指定する。               */

  ID		port;		/* メッセージバッファ ID                  */
				/* この要素の値が 0 のときには、このエ    */
  				/* ントリは、使っていないことを表す。     */

  ID		task;		/* メッセージバッファを登録したタスク */
};


/*
 * 情報テーブル
 */
static struct data_entry_t	table[MAX_PORT_ENTRY];


/*
 * テーブルから名前によってエントリを検索する関数。
 * unregist_database() と find_database() で使用する。
 */
static W	find_entry (PORT_NAME name);


/*
 * データベースの初期化
 * 情報テーブル table 変数の要素を初期設定する。
 * (0 で埋める)
 */
void
init_regist_table (void)
{
  bzero (table, sizeof (table));
}


/*
 * データの登録処理
 * table 変数の空いているエントリを見つけ、メッセージバッファを
 * 登録する。
 *
 */
PORT_MANAGER_ERROR
regist_database (PORT_NAME name, ID port, ID task)
{
  W	counter;		/* 空いているエントリをテーブルから */
				/* 見つけるときに使用するカウンタ   */

  /*
   * テーブル同じ名前のエントリがないかを検索する。
   * もし、同じ名前をもつエントリがあった場合には、
   * E_PORT_DUPLICATION のエラーを返す。
   */
  if (find_entry (name) >= 0)
    {
      return (E_PORT_DUPLICATION);	/* 同じ名前をもつエントリがす */
					/* でにあったエラーを返す。   */
    }

  /*
   * テーブルの先頭から空いているエントリを探す。
   */
  for (counter = 0; counter < MAX_PORT_ENTRY; counter++)
    {
      /*
       * もし、エントリの ID 要素が 0 ならば、このエントリは
       * 使われていない。
       */
      if (table[counter].port == 0)
	{
	  /*
	   * 空いているエントリを見つけた。
	   * メッセージバッファを登録する。
	   */
	  strncpy (table[counter].name, name, (PORT_NAME_LEN + 1));
	  table[counter].port = port;
	  table[counter].task = task;
#ifdef DEBUG
	  dbg_printf ("regist: %s, %s\n", &(table[counter].name), name);
#endif /* DEBUG */
	  return (E_PORT_OK);
	}
    }

  /*
   * 空いているエントリが発見できなかった。
   * E_PORT_FULL のエラーを返す。
   */
  return (E_PORT_FULL);
}


/*
 * データベースから指定した名前をもつエントリを削除する。
 * もし、対応するエントリが見つけられなかった場合には、E_PORT_NOTFOUND 
 * のエラーを返す。
 *
 * 引数 name で指定したエントリを見つけ、エントリの内容を 0 で埋める。
 * 
 * この関数は、削除するメッセージバッファの ID を引数 port に返す。
 *
 */
PORT_MANAGER_ERROR
unregist_database (PORT_NAME name, ID *port, ID task)
{
  W	index;		/* データベースから見つけたエントリ */

  index = find_entry (name);
  if (index < 0)
    {
      return (E_PORT_NOTFOUND);	/* エントリ番号が -1 だった;       */
				/* 引数 name に該当するエントリが  */
				/* なかったので、E_PORT_NOTFOUND の */
      				/* エラーを返す。                  */
    }

  /*
   * メッセージバッファ ID を引数 port に代入する。
   */
  *port = table[index].port;

  /*
   * メッセージバッファを登録したタスクと抹消要求を行ったタスクが
   * 異なっていた場合には、E_PORT_INVALID のエラーとする。
   */
  if (table[index].task	 != task)
    {
      return (E_PORT_INVALID);
    }

  /*
   * find_entry() によって、発見したエントリを 0 で埋める。
   */
  bzero (&table[index], sizeof (struct data_entry_t));

  return (E_PORT_OK);  
}


/*
 * 引数 name で指定した名前をもつエントリを見つけてメッセージバッファ 
 * ID を返す。
 * もし、テーブルの中に該当する名前をもつエントリがなかった場合には、
 * E_PORT_NOTFOUND のエラーを返す。
 *
 */
PORT_MANAGER_ERROR
find_database (PORT_NAME name, ID *port)
{
  W	index;	/* データベーステーブルから見つけたエントリを表す  */
		/* table 変数のインデックス。			   */

  /*
   * 対応するエントリがないかを探す。
   * find_entry() を使っている。
   */
  index = find_entry (name);
  if (index == -1)
    {
      /*
       * find_entry() の返り値が -1 だった。
       * 対応するエントリがないので、E_PORT_NOTFOUND のエラーを返す。
       */
      return (E_PORT_NOTFOUND);
    }

  /*
   * 引数 port に探し出したエントリに入っているメッセージバッファの id 
   * を設定する。 
   */
  *port = table[index].port;

  return (E_PORT_OK);		/* 正常終了。E_PORT_OK を返す。 */
}


/*
 * 情報テーブルからエントリを見つけるための下請け関数。
 * 返り値として情報テーブルのインデックス番号を返す。
 * テーブルから引数 name に対応する名前をもつエントリが発見できなかっ
 * た場合には、-1 を返す。
 */
static W
find_entry (PORT_NAME name)
{
  W	index;			/* 情報テーブルをサーチするときに、使 */
				/* 用するカウンタ。                   */

  /*
   * 該当する名前をもつエントリがないかどうかを情報テーブル (table) を
   * 先頭から検索する。
   */
  for (index = 0; index < MAX_PORT_ENTRY; index++)
    {
#if defined(DEBUG)
      dbg_printf ("find_entry: <%s>, <%s>\n", 
		  table[index].name, name);
#endif /* DEBUG */

      if (strncmp (name, table[index].name, PORT_NAME_LEN) == 0)
	{
	  /*
	   * 引数 name と同じ名前をもつエントリを発見した。
	   * 情報テーブルの インデックス値を返す。
	   */
	  return (index);
	}
    }

  /*
   * 情報テーブルには、該当するエントリがなかった。
   * -1 を返す。
   */
  return (-1);
}
