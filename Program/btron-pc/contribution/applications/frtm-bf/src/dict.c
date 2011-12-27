/*
   FoRTh Modoki
   dict.c ver 1.3.1
   Copyright (C) 1999 Tomohide Naniwa. All rights reserved.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Last modified: Nov. 19, 1999.
*/

#include "frtm.h"

void exec_user_com();
char *search_com();

/*#define DEBUG*/

/* Dictionary Definition */
static struct dn {
  char name[MAX_NAME];
  int point;
} dict_name[DICMAX];

static int dp = 0;

/* Command Stack Definition */
static struct cs {
  enum {INT, FUNC, CSP} id;
  union {
    int val;
    void (*func)();
    int csp;
  } data;
} cstack[CSMAX];

static int csp = 0;
static int old_csp = 0;

/* Return Stack Definition */
static int rstack[RSMAX];
static int rsp = 0;

/* Structure Stack Definition */
static MODE sstack[SSMAX];
static int ssp = 0;

/* Dictionary Functions */
void add_dict_name(name)
     char *name;
{
  if (dp == DICMAX) {
    error_no = E_DSO;
    return;
  }
  strncpy(dict_name[dp].name, name, MAX_NAME);
  dict_name[dp].point = csp;
  ++dp;
}

void drop_dict()
{
  if (dp == 0) {
    error_no = E_DSU;
    return;
  }
  --dp;
}

int query_dp()
{
  return dp;
}

char *query_dict_name(n)
     int n;
{
  return dict_name[n].name;
}

int query_dict_point(n)
     int n;
{
  return dict_name[n].point;
}

/* Command Stack Functions */
void keep_old_csp()
{
  old_csp = csp;
}

void reload_old_csp()
{
  csp = old_csp;
}

void cpush_int(val)
     int val;
{
  if (csp == CSMAX) {
    error_no = E_CSO;
    return;
  }
  cstack[csp].id = INT;
  cstack[csp].data.val = val;
  ++csp;
}

void cpush_func(func)
     void (*func)();
{
  if (csp == CSMAX) {
    error_no = E_CSO;
    return;
  }
  cstack[csp].id = FUNC;
  cstack[csp].data.func = func;
  ++csp;
}

void cpush_csp(cs_p)
     int cs_p;
{
  if (csp == CSMAX) {
    error_no = E_CSO;
    return;
  }
  cstack[csp].id = CSP;
  cstack[csp].data.csp = cs_p;
  ++csp;
}

#if 0
void cpush_index(n)
     int n;
{
  if (csp == CSMAX) {
    error_no = E_CSO;
    return;
  }
  cstack[csp].id = CSP;
  cstack[csp].data.csp = dict_name[n].point;
  ++csp;
}
#endif

void cpush_str(str)
     char *str;
{
  int i, len, cm, cl, il, val;
  cl = sizeof(char)*8;
  il = sizeof(int) / sizeof(char);
  len = strlen(str)-2;
  cm = len % il;
  if (cm == 0) {
    cpush_int(0); EC;
    cm = il;
  }
  val = 0;
  for(i = len; i > 0; --i) {
    val <<= cl;
    val += (0x00FF & str[i]);
    if (--cm == 0) {
      cpush_int(val); EC;
      cm = il;
      val = 0;
    }
  }
}

/* Return Stack Functions */
void rpush(val)
     int val;
{
#ifdef DEBUG
  int i;
#endif
  if (rsp == RSMAX) {
    error_no = E_RSO;
    return;
  }
  rstack[rsp] = val;
  ++rsp;
#ifdef DEBUG
  printf("r push: ");
  for(i = 0; i < rsp; ++i) {
    printf("%d ", rstack[i]);
  }
  printf("\n");
#endif
}

int rpop()
{
#ifdef DEBUG
  int i;
#endif
  if (rsp == 0) {
    error_no = E_RSU;
    return;
  }
#ifdef DEBUG
  printf("r pop: ");
  for(i = 0; i < rsp-1; ++i) {
    printf("%d ", rstack[i]);
  }
  printf("\n");
#endif
  return rstack[--rsp];
}

int rpeek()
{
  if (rsp == 0) {
    error_no = E_RSU;
    return;
  }
  return rstack[rsp-1];
}

void clear_rstack()
{
  rsp = 0;
}

/* Structure Stack Functions */
void spush(str)
     MODE str;
{
#ifdef DEBUG
  int i;
#endif
  if (ssp == SSMAX) {
    error_no = E_SSO;
    return;
  }
  sstack[ssp] = str;
  ++ssp;
#ifdef DEBUG
  printf("s push: ");
  for(i = 0; i < ssp; ++i) {
    printf("%d ", sstack[i]);
  }
  printf("\n");
#endif
}

MODE spop()
{
#ifdef DEBUG
  int i;
#endif
  if (ssp == 0) {
    error_no = E_SSU;
    return;
  }
#ifdef DEBUG
  printf("s pop: ");
  for(i = 0; i < ssp-1; ++i) {
    printf("%d ", sstack[i]);
  }
  printf("\n");
#endif
  return sstack[--ssp];
}

void clear_sstack()
{
  ssp = 0;
}

/* Structure Command */
void f_if()
{
  int val;

  switch(eval_mode) {
  case RUN:
    spush(eval_mode); EC;
    val = pop(); EC;
    if (! val) eval_mode = SKIP;
    break;
  case SKIP:
    spush(SKIP); EC;
    break;
  default:
    error_no = E_ILM;
  }
  spush(IF); EC;
}

void f_else()
{
  MODE str, mode;

  str = spop(); EC;
  if (str != IF) {
    error_no = E_ILS;
    return;
  }
  mode = spop(); EC;
  switch(mode) {
  case RUN:
    switch(eval_mode) {
    case RUN:
      eval_mode = SKIP;
      break;
    case SKIP:
      eval_mode = mode;
      break;
    default:
      error_no = E_ILM;
    }
    break;
  case SKIP:
    break;
  default:
    error_no = E_ILM;
  }
  spush(mode); EC;
  spush(IF); EC;
}

void f_endif()
{
  MODE str, mode;

  str = spop(); EC;
  if (str != IF) {
    error_no = E_ILS;
    return;
  }
  mode = spop(); EC;
  eval_mode = mode;
}

void f_do(c_sp)
     int c_sp;
{
  spush(memory); EC;
  spush(DO); EC;
  if (c_sp == -1) {
    memory = ON;
    rpush(csp); EC;
  }
  else {
    rpush(c_sp+1); EC;
  }
}

void f_while()
{
  MODE str, keep;
  int val;

  str = spop(); EC;
  if (str != DO) {
    error_no = E_ILS;
    return;
  }
  keep = spop(); EC;

  val = pop(); EC;
  if (val) {
    if (memory == ON) {
      cpush_func(f_while); EC;
      cpush_csp(-1);
    }
    memory = OFF;
    val = rpeek(); EC;
    rpush(-1); EC;
    rpush(val); EC;
    spush(keep); EC;
    spush(str); EC;
    exec_user_com(val);
  }
  memory = keep;
  val = rpop(); EC;
  if (memory == OFF) csp = val;
  else {
    --csp;
  }
}

void f_for(c_sp)
     int c_sp;
{
  int v1, v2;

  spush(memory); EC;
  spush(FOR); EC;
  v1 = pop(); EC;
  v2 = pop(); EC;
  if (v1 < 0 || v2 < 0) {
    error_no = E_ILC;
    return;
  }
  rpush(v2); EC;
  rpush(v1); EC;
  if (c_sp == -1) {
    memory = ON;
    rpush(csp); EC;
  }
  else {
    rpush(c_sp+1); EC;
  }
}

void f_loop()
{
  MODE str, keep;
  int v1, v2, ret;

  str = spop(); EC;
  if (str != FOR) {
    error_no = E_ILS;
    return;
  }
  keep = spop(); EC;

  ret = rpop(); EC;
  v1 = rpop(); EC;
  v2 = rpop(); EC;
  ++v1;
  if (v2 > v1) {
    if (memory == ON) {
      cpush_func(f_loop); EC;
      cpush_csp(-1);
    }
    memory = OFF;
    rpush(-1); EC;
    rpush(v2); EC; rpush(v1); EC; rpush(ret); EC;
    spush(keep); EC;
    spush(str); EC;
    exec_user_com(ret);
  }
  memory = keep;
  if (memory == OFF) csp = ret;
  else {
    --csp;
  }
}

void f_loopp()
{
  MODE str, keep;
  int v1, v2, val, ret;

  str = spop(); EC;
  if (str != FOR) {
    error_no = E_ILS;
    return;
  }
  keep = spop(); EC;

  ret = rpop(); EC;
  v1 = rpop(); EC;
  v2 = rpop(); EC;
  val = pop(); EC;
  v1 += val;
  if (v2 > v1) {
    if (memory == ON) {
      cpush_func(f_loopp); EC;
      cpush_csp(-1);
    }
    memory = OFF;
    rpush(-1); EC;
    rpush(v2); EC; rpush(v1); EC; rpush(ret); EC;
    spush(keep); EC;
    spush(str); EC;
    exec_user_com(ret);
  }
  memory = keep;
  if (memory == OFF) csp = ret;
  else {
    --csp;
  }
}

void f_count()
{
  int num;
  
  if (rsp - 2 < 0) {
    error_no = E_RSU;
    return;
  }
  num = rstack[rsp-2];
  push(num); EC;
}

/* Functions */
void f_dict_start()
{
  if (eval_mode != RUN || memory == ON) {
    error_no = E_IVC;
    return;
  }
  eval_mode = DIC1;
}

void f_dict_end()
{
  if (eval_mode != DIC2) {
    error_no = E_IVC;
    return;
  }
  cpush_csp(-1); EC;
  keep_old_csp();
  eval_mode = RUN;
  printf("Memory %s [%d-%d]\n",
	 dict_name[dp-1].name, dict_name[dp-1].point, csp-1);
}

void do_forget(str)
     char *str;
{
  int i;
  char *name;
  for(i = query_dp()-1; i >= 0; --i) {
    name = dict_name[i].name;
    if (! strcmp(str, name)) {
      dp = i;
      csp = dict_name[i].point;
      keep_old_csp();
      break;
    }
  }
  if (i < 0) {
    error_no = E_IVN;
    return;
  }
  else {
    printf("Froget after %s [%d]\n", str, csp);
  }
  eval_mode = RUN;
}

char *search_dict(csp)
     int csp;
{
  int i;
  for(i = 0; i < dp; ++i) {
    if (dict_name[i].point == csp) return dict_name[i].name;
  }
  return("");
}

void do_def(str)
     char *str;
{
  int i, cm, pt, flag;
  char *name;
  for(i = query_dp()-1; i >= 0; --i) {
    name = dict_name[i].name;
    if (! strcmp(str, name)) {
      pt = dict_name[i].point;
      break;
    }
  }
  if (i < 0) {
    error_no = E_IVN;
    return;
  }
  printf(": %s ", name);
  flag = 1;
  while(flag) {
    switch(cstack[pt].id) {
    case INT:
      printf("%d ", cstack[pt].data.val);
      break;
    case FUNC:
      name = search_com(cstack[pt].data.func);
      printf("%s ", name);
      break;
    case CSP:
      i = cstack[pt].data.csp;
      if (i != -1) {
	name = search_dict(i);
	printf("%s ", name);
      }
      else {
	printf(";\n");
	flag = 0;
      }
      break;
    }
    ++pt;
  }
  eval_mode = RUN;
}

/* User Command Interpretor */
void exec_user_com(cpp)
     int cpp;
{
  int i, val, v1, v2;
  void (*func)();
  MODE str, st2;

  while(1) {
    if (cpp >= CSMAX) {
      error_no = E_COR;
      return;
    }
    switch(cstack[cpp].id) {
    case INT:
      switch(eval_mode) {
      case RUN:
	val = cstack[cpp].data.val;
	push(val); EC;
	break;
      case SKIP:
	break;
      }
      break;
    case FUNC:
      func = cstack[cpp].data.func;
      if (func == f_if || func == f_else || func == f_endif ) {
	(func)(); EC;
      }
      else if (func == f_do || func == f_for) {
	if (eval_mode == SKIP) break;
	(func)(cpp); EC;
      }
      else if (func == f_while) {
	if (eval_mode == SKIP) break;
	str = spop(); EC;
	if (str != DO) {
	  error_no = E_ILS;
	  return;
	}
	st2 = spop(); EC;
	val = pop(); EC;
	if (val) {
	  i = rpeek(); EC;
	  cpp = i;
	  spush(st2); EC;
	  spush(str); EC;
	  continue;
	}
	else {
	  rpop(); EC;
	  break;
	}
      }
      else if (func == f_loop) {
	if (eval_mode == SKIP) break;
	str = spop(); EC;
	if (str != FOR) {
	  error_no = E_ILS;
	  return;
	}
	st2 = spop(); EC;
	i = rpop(); EC;
	v1 = rpop(); EC;
	v2 = rpop(); EC;
	++v1;
	if (v2 > v1) {
	  rpush(v2); EC; rpush(v1); EC; rpush(i); EC;
	  cpp = i;
	  spush(st2); EC;
	  spush(str); EC;
	  continue;
	}
	else {
	  break;
	}
      }
      else if (func == f_loopp) {
	if (eval_mode == SKIP) break;
	str = spop(); EC;
	if (str != FOR) {
	  error_no = E_ILS;
	  return;
	}
	st2 = spop(); EC;
	i = rpop(); EC;
	v1 = rpop(); EC;
	v2 = rpop(); EC;
	val = pop(); EC;
	v1 += val;
	if (v2 > v1) {
	  rpush(v2); EC; rpush(v1); EC; rpush(i); EC;
	  cpp = i;
	  spush(st2); EC;
	  spush(str); EC;
	  continue;
	}
	else {
	  break;
	}
      }
      else {
	switch(eval_mode) {
	case RUN:
	  (func)(); EC;
	  break;
	case SKIP:
	  break;
	}
      }
      break;
    case CSP:
      switch(eval_mode) {
      case RUN:
	i = cstack[cpp].data.csp;
	if (i == -1) {
	  cpp = rpop(); EC;
	  if (cpp < 0) return;
	  continue;
	}
	else {
	  rpush(cpp+1); EC;
	  cpp = i;
	  continue;
	}
	break;
      case SKIP:
	break;
      }
    }
    ++cpp;
  }
}
