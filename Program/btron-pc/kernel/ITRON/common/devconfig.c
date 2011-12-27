/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* 
 * ITRON 内部のデバイスドライバの定義
 *
 */

#include "types.h"
#include "config.h"

extern ER	init_echo (void);
extern ER	init_rs232c (void);
extern ER	init_pd7220 (void);
extern ER	init_keyboard (void);

ER (*devices[MAX_DEVICE])(void) = {
  init_keyboard,
#ifdef nodef
  init_echo,
#endif
  0,
/*
  init_rs232c,
  init_pd7220
 */
};

W ndevice = (sizeof (devices) / sizeof (devices[0]));
