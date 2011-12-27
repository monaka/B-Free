/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/jmp_user.S,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * POSIX 環境 on B-Free 
 */


.globl _jmp_user_entry

/*
 * ユーザモードに遷移しつつ、指定したエントリアドレスへジャンプする
 */
_jmp_user_entry:
	
	ret