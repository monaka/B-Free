/*
   FoRTh Modoki
   stack.c ver 1.3.0
   Copyright (C) 1999 Tomohide Naniwa. All rights reserved.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Last modified: May 9, 1999.
*/

#include "frtm.h"

static int stack[STMAX];
static int sp = 0;

void push(val)
     int val;
{
  if (sp == STMAX) {
    error_no = E_VSO;
    return;
  }
  stack[sp++] = val;
}

int pop()
{
  if (sp == 0) {
    error_no = E_VSU;
    return;
  }
  return stack[--sp];
}

void clear_stack()
{
  sp = 0;
}

void push_str(str)
     char *str;
{
  int i, len, cm, cl, il, val;
  cl = sizeof(char)*8;
  il = sizeof(int) / sizeof(char);
  len = strlen(str)-2;
  cm = len % il;
  if (cm == 0) {
    push(0); EC;
    cm = il;
  }
  val = 0;
  for(i = len; i > 0; --i) {
    val <<= cl;
    val += (0x00FF & str[i]);
    if (--cm == 0) {
      push(val); EC;
      cm = il;
      val = 0;
    }
  }
}

void pop_string(str)
     char *str;
{
  int val1, i, j, cl, il;

  *str = 0;
  j = 0;
  cl = sizeof(char)*8;
  il = sizeof(int) / sizeof(char);
  while(1) {
    val1 = pop(); EC;
    for(i = 0; i < il; ++i) {
      str[j+i] = val1 & 0x00FF;
      val1 >>= cl;
      if (str[j+i] == 0) break;
    }
    if (i != il) break;
    j += i;
  }
}

/* Stack Manipulation Function */
void f_ndup()
{
  int v1, v2;

  v1 = pop(); EC;
  if ((v1 <= 0) || (v1 > sp)) {
    error_no = E_SOR;
    return;
  }
  v2 = stack[sp - v1];
  push(v2); EC;
}

void f_nrot()
{
  int v1, v2, i;
  
  v1 = pop(); EC;
  if ((v1 <= 0) || (v1 > sp)) {
    error_no = E_SOR;
    return;
  }
  if (v1 == 1) return;
  v2 = stack[sp - v1];
  for(i = v1; i > 1; --i) {
    stack[sp - i] = stack[sp -i + 1];
  }
  stack[sp - 1] = v2;
}

void f_nrotr()
{
  int v1, v2, i;
  
  v1 = pop(); EC;
  if ((v1 <= 0) || (v1 > sp)) {
    error_no = E_SOR;
    return;
  }
  if (v1 == 1) return;
  v2 = stack[sp - 1];
  for(i = 1; i < v1; ++i) {
    stack[sp - i] = stack[sp -i - 1];
  }
  stack[sp - v1] = v2;
}

void f_ndrop()
{
  int v1, v2, i;
  
  v1 = pop(); EC;
  if ((v1 <= 0) || (v1 > sp)) {
    error_no = E_SOR;
    return;
  }
  sp -= v1;
}

void f_SP()
{
  push(sp); EC;
}

void f_copy()
{
  int v1, v2, i;
  
  v1 = pop(); EC;
  if ((v1 <= 0) || (v1 > sp)) {
    error_no = E_SOR;
    return;
  }
  for(i = 0; i < v1; ++i) {
    push(stack[sp-v1]); EC;
  }
}
