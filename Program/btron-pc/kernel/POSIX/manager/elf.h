/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/


/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/manager/elf.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/* elf.h --- ELF ファイルフォーマット関連の定義
 *
 *
 * $Log: elf.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1997-10-24 13:55:33  night
 * ファイル中の文字コードを SJIS から EUC に変更。
 *
 * Revision 1.1  1997/10/23 14:32:32  night
 * exec システムコール関係の処理の更新
 *
 *
 *
 */



#ifndef __ELF_H__
#define __ELF_H__	1


#define EI_NIDENT	16


typedef	unsigned long	ELF32_Addr;
typedef unsigned short	ELF32_Half;
typedef unsigned long	ELF32_Off;
typedef unsigned long	ELF32_Sword;
typedef unsigned long	ELF32_Word;


#define PT_NULL		0
#define PT_LOAD		1
#define PT_DYNAMIC	2
#define PT_INTERP	3
#define PT_NONE		4
#define PT_SHLIB	5
#define PT_PHDR		6
#define PT_LOPROC	0x70000000
#define PT_HIPROC	0x7fffffff

#define PF_R		0x4
#define PF_W		0x2
#define PF_X		0x1


/* ELF ファイルのタイプ定義 
 */
#define ET_NONE		0
#define ET_REL		1
#define ET_EXEC		2
#define ET_DYN		3
#define ET_CORE		4
#define ET_LOPROC	5
#define ET_HIPROC	6


/* ELF ファイルのマシン定義
 */
#define EM_NONE		0
#define EM_M32		1
#define EM_SPARC	2
#define EM_386		3
#define EM_68K		4
#define EM_88K		5
#define EM_860		7
#define EM_MIPS		8


struct ELFheader
{
  unsigned char		e_ident[EI_NIDENT];
  ELF32_Half		e_type;
  ELF32_Half		e_machine;
  ELF32_Word		e_version;
  ELF32_Addr		e_entry;
  ELF32_Off		e_phoff;
  ELF32_Off		e_shoff;
  ELF32_Word		e_flags;
  ELF32_Half		e_ehsize;
  ELF32_Half		e_phentsize;
  ELF32_Half		e_phnum;
  ELF32_Half		e_shentsize;
  ELF32_Half		e_shnum;
  ELF32_Half		e_shstrndx;
};


struct ELF_Pheader
{
  ELF32_Word		p_type;
  ELF32_Off		p_offset;
  ELF32_Addr		p_vaddr;
  ELF32_Addr		p_paddr;
  ELF32_Word		p_filesz;
  ELF32_Word		p_memsz;
  ELF32_Word		p_flags;
  ELF32_Word		p_align;
};


struct ELF_Sheader 
{
  ELF32_Word	sh_name;
  ELF32_Word	sh_type;
  ELF32_Word	sh_flags;
  ELF32_Addr	sh_addr;
  ELF32_Off	sh_offset;
  ELF32_Word	sh_size;
  ELF32_Word	sh_link;
  ELF32_Word	sh_info;
  ELF32_Word	sh_addralign;
  ELF32_Word	sh_entsize;
};



#endif /* __ELF_H__ */
