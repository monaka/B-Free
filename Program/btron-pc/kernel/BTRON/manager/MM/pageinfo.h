/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/manager/MM/pageinfo.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */


/*
 * $Log: pageinfo.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1997-06-29 15:44:33  night
 * コメントの変更
 *
 * ・漢字コードを SJIS から EUC に変更した。
 * ・RCS の Log マクロ入りのコメントを追加した(まだ入れていなかったファイルのみ)。
 *
 *
 */


#ifndef __MM_PAGEINFO_H__
#define __MM_PAGEINFO_H__		1




struct region_tree_t
{
  
};


struct page_tree_t
{

};


struct proc_info
{
  ID	procid;

  struct page_tree_t	*page_tree;
};



#endif /* __MM_PAGEINFO_H__ */
