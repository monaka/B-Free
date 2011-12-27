/*
   FoRTh Modoki
   interp.c ver 1.3.2 B-Free 1.7.8
   Copyright (C) 1999, 2000 Tomohide Naniwa. All rights reserved.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Last modified: Jun. 3, 2000.
*/

#include "frtm.h"
#include "funcs.h"
#include "fgraph.h"
#include "posix.h"
#include "debug.h"

/*#define SETUP*/

void f_help();
void f_forget();
void f_def();
extern int query_dp();
extern char *query_dict_name();
extern int query_dict_point();

struct bc {
  char *name;
  void (*func)();
  int next;
} builtin_command[] = {
  {".", f_dot, 0},
  {".h", f_doth, 0},
  {".o", f_doto, 0},
  {"+", f_sum, 0},
  {"-", f_sub, 0},
  {"*", f_mul, 0},
  {"/", f_div, 0},
  {"%", f_mod, 0},
  {"=", f_eq, 0},
  {"<", f_lt, 0},
  {">", f_gt, 0},
  {"<=", f_le, 0},
  {">=", f_ge, 0},
  {"sp", f_sp, 0},
  {"cr", f_cr, 0},
  {"!", f_not, 0},
  {"1+", f_inc, 0},
  {"1-", f_dec, 0},
  {"0=", f_eq0, 0},
  {"dup", f_dup, 0},
  {"swap", f_swap, 0},
  {"drop", f_drop, 0},
  {"over", f_over, 0},
  {"rot", f_rot, 0},
  {"rotr", f_rotr, 0},
  {"ndup", f_ndup, 0},
  {"nrot", f_nrot, 0},
  {"nrotr", f_nrotr, 0},
  {"ndrop", f_ndrop, 0},
  {"copy", f_copy, 0},
  {":", f_dict_start, 0},
  {";", f_dict_end, 0},
  {"if", f_if, 0},
  {"else", f_else, 0},
  {"endif", f_endif, 0},
  {"do", f_do, 0},
  {"while", f_while, 0},
  {"for", f_for, 0},
  {"loop", f_loop, 0},
  {"+loop", f_loopp, 0},
  {"I", f_count, 0},
  {"abs", f_abs, 0},
  {"max", f_max, 0},
  {"min", f_min, 0},
  {"echo", f_echo, 0},
  {"help", f_help, 0},
  {"SP", f_SP, 0},
  {"minus", f_minus, 0},
  {"forget", f_forget, 0},
  {"spaces", f_spaces, 0},
  {"esc", f_esc, 0},
  {"def", f_def, 0},
  {"gm", f_gm, 0},
  {"tm", f_tm, 0},
  {"cls", f_cls, 0},
  {"moveto", f_moveto, 0},
  {"lineto", f_lineto, 0},
  {"rmoveto", f_rmoveto, 0},
  {"rlineto", f_rlineto, 0},
  {"rect", f_rect, 0},
  {"rectf", f_rectf, 0},
  {"point", f_point, 0},
  {"ellips", f_ellips, 0},
  {"ellipsf", f_ellipsf, 0},
  {"setcolor", f_setcolor, 0},
  {"setbgc", f_setbgc, 0},
  {"setfont", f_setfont, 0},
  {"drawstr", f_drawstring, 0},
  {"drawistr", f_drawistring, 0},
  {"kfopen", f_kfopen, 0},
  {"kfclose", f_kfclose, 0},
  {"drawkch", f_drawkch, 0},
  {"winmod", f_winmod, 0},
  {"open", f_open, 0},
  {"close", f_close, 0},
  {"write", f_write, 0},
  {"cat", f_cat, 0},
  {"dir", f_dir, 0},
  {"unlink", f_unlink, 0},
  {"mkdir", f_mkdir, 0},
  {"rmdir", f_rmdir, 0},
  {"chdir", f_chdir, 0},
  {"exec", f_exec, 0},
  {"load", f_load, 0},
  {"multi", f_multi, 0},
  {"single", f_single, 0},
  {"tasklist", f_tasklist, 0},
  {"pmemstat", f_pmemstat, 0},
  {"prtflist", f_prtflist, 0},
  {"puzzle", f_puzzle, 0},
  {"graphics", f_graphics, 0},
  {"quit", f_quit, 0}
};

struct hs {
  int val, next;
} hash_tbl[HASH];

interp(str)
     char *str;
{
  int i, cm;
  char *name;
  void (*func)();

  if (*str == '(') {
    switch(eval_mode){
    case DIC1:
    case FGT:
    case DEF:
      error_no = E_IVN;
      return;
    case DIC2:
      cpush_str(str);
      return;
    case RUN:
      push_str(str);
    case SKIP:
      if (memory == ON) {
	cpush_str(str);
      }
      return;
    }
  }
  if (eval_mode == DIC1) {
    /* ¼­½ñÀßÄê */
    add_dict_name(str); EC;
    keep_old_csp();
    eval_mode = DIC2;
    return;
  }
  else if (eval_mode == FGT) {
    do_forget(str);
    return;
  }
  else if (eval_mode == DEF) {
    do_def(str);
    return;
  }
#ifndef HASH
  cm = sizeof(builtin_command)/sizeof(struct bc);
  for(i = 0; i < cm; ++i) {
    if (! strcmp(str, builtin_command[i].name)) {
#else
      i = hash_search(str);
      if (i != -1) {
#endif
	func = builtin_command[i].func;
	if (func == f_dict_end) {
	  /* ¼­½ñ¤ÎÅÐÏ¿½ªÎ» */
	  (f_dict_end)(); EC;
	  return;
	}
	else if (func == f_if) {
	  if (eval_mode == DIC2) {
	    cpush_func(f_if); EC;
	  }
	  else {
	    if (memory == ON) {
	      cpush_func(f_if); EC;
	    }
	    (f_if)(); EC;
	  }
	  return;
	}
	else if (func == f_else) {
	  if (eval_mode == DIC2) {
	    cpush_func(f_else); EC;
	  }
	  else {
	    if (memory == ON) {
	      cpush_func(f_else); EC;
	    }
	    (f_else)(); EC;
	  }
	  return;
	}
	else if (func == f_endif) {
	  if (eval_mode == DIC2) {
	    cpush_func(f_endif); EC;
	  }
	  else {
	    if (memory == ON) {
	      cpush_func(f_endif); EC;
	    }
	    (f_endif)(); EC;
	  }
	  return;
	}
	else if (func == f_do || func == f_for) {
	  if (eval_mode == DIC2) {
	    cpush_func(func);
	  }
	  else {
	    if (memory == ON) {
	      cpush_func(func); EC;
	    }
	    if (eval_mode == RUN) (func)(-1); EC;
	  }
	  return;
	}
	else if (func == f_while || func == f_loop || func == f_loopp) {
	  if (eval_mode == DIC2) {
	    cpush_func(func);
	  }
	  else {
	    if (eval_mode == RUN) (func)(); EC;
	  }
	  return;
	}
	if (eval_mode == RUN || eval_mode == SKIP) {
	  if (memory == ON) {
	    cpush_func(func); EC;
	  }
	  if (eval_mode == RUN) {
	    (func)(); EC;
	  }
	  return;
	}
	else if (eval_mode == DIC2) {
	  /* ¼­½ñÅÐÏ¿ */
	  cpush_func(func); EC;
	  return;
	}
      }
#ifndef HASH
  }
#endif
  for(i = query_dp()-1; i >= 0; --i) {
    name = query_dict_name(i);
    if (! strcmp(str, name)) {
      cm = query_dict_point(i);
      if (eval_mode == RUN || eval_mode == SKIP) {
	if (memory == ON) {
	  cpush_csp(cm); EC;
	}
	if (eval_mode ==RUN) {
	  rpush(-1); EC;
	  exec_user_com(cm); EC;
	}
	return;
      }
      else if (eval_mode == DIC2) {
	cpush_csp(cm); EC;
	return;
      }
    }
  }
  if (i < 0) {
    error_no = E_UKC;
  }
}

void f_help()
{
  int i, cm;

  cm = sizeof(builtin_command)/sizeof(struct bc);
  printf("Builtin Command:");
  for(i = 0; i < cm; ++i) {
    if (!(i%8)) printf("\n");
#ifdef BFREE
    printf("%s ", builtin_command[i].name);
#else
    printf("%8s ", builtin_command[i].name);
#endif
  }
  printf("\n");
  cm = query_dp();
  printf("User Command:");
  for(i = 0; i < cm; ++i) {
    if (!(i%8)) printf("\n");
#ifdef BFREE
    printf("%s ", query_dict_name(i));
#else
    printf("%8s ", query_dict_name(i));
#endif
  }
  printf("\n");
}

void f_forget()
{
  if (eval_mode != RUN || memory == ON) {
    error_no = E_IVC;
    return;
  }
  eval_mode = FGT;
}

void f_def()
{
  if (eval_mode != RUN || memory == ON) {
    error_no = E_IVC;
    return;
  }
  eval_mode = DEF;
}

char *search_com(func)
     void (*func)();
{
  int i, cm;
  cm = sizeof(builtin_command)/sizeof(struct bc);
  for(i = 0; i < cm; ++i) {
    if (builtin_command[i].func == func) return(builtin_command[i].name);
  }
}

/* Hash search */
int hash(str)
     char *str;
{
  int i, val, ch;
  i = 0; val = 0;
  while((ch = str[i]) != 0) {
    val += ch;
    val %= HASH;
    ++i;
  }
  return val;
}

int hash_search(name)
     char *name;
{
  int i, h;

  h = hash(name);
  i = hash_tbl[h].next;
  while(i != -1) {
    if (! strcmp(builtin_command[i].name, name)) return i;
    i = builtin_command[i].next;
  }
  return -1;
}

void init_hash()
{
  int i, cm, h;
#ifdef SETUP
  static int num[HASH];
#endif

  for(i = 0; i < HASH; ++i) {
    hash_tbl[i].next = -1;
  }
  cm = sizeof(builtin_command)/sizeof(struct bc);
  for(i = cm-1; i >= 0; --i) {
    h = hash(builtin_command[i].name);
    builtin_command[i].next = hash_tbl[h].next;
    hash_tbl[h].next = i;
#ifdef SETUP
    num[h]++;
#endif
  }
#ifdef SETUP
  for(i = 0; i < HASH; ++i) {
    printf("%d\t%d\n", i, num[i]);
  }
#endif
}
