/*
   FoRTh Modoki
   demos.c ver 1.3.0
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

void f_puzzle()
{
  eval(": move1 (Move disk ) echo . swap  ( from ) echo . ( to ) echo . cr ;"); EC;
  eval(": hanoi dup 1 = if move1 else"); EC;
  eval("    3 ndup 3 ndup + 6 - minus 4 ndup swap 3 ndup 1- hanoi"); EC;
  eval("    3 ndup 3 ndup 3 ndup move1"); EC;
  eval("    1- rotr swap over + 6 - minus swap rot hanoi"); EC;
  eval("  endif ;"); EC;
  eval(": N 2 * 3 + ;"); EC;
  eval(": disp dup N ndup 0 for "); EC;
  eval("    dup N ndup 0 for over I = "); EC;
  eval("      if (Q ) echo else (. ) echo endif "); EC;
  eval("    loop cr drop drop "); EC;
  eval("  loop drop ;"); EC;
  eval(": check over 0 for "); EC;
  eval("    3 ndup 6 ndup = if drop 0 else "); EC;
  eval("        3 ndup 6 ndup - abs 3 ndup 6 ndup - = if drop 0 else 1 = endif "); EC;
  eval("    endif"); EC;
  eval("    5 nrot 5 nrot drop drop "); EC;
  eval("  loop rotr drop drop ;"); EC;
  eval(": nq dup N ndup 0 for "); EC;
  eval("    I over 1+ dup N 1- copy 1 check if "); EC;
  eval("      dup N ndup over 1+ = if dup N copy disp cr else nq endif "); EC;
  eval("    endif"); EC;
  eval("  drop drop loop ;"); EC;
  eval(": queens dup 0 for I 0 nq drop drop loop drop ;"); EC;
}

void f_graphics()
{
  eval(": boxes 16 0 for I setcolor 10 20 I * + dup 100 100 rectf loop ;"); EC;
  eval(": pre 5 copy drop ;"); EC;
  eval(": genx 3 ndup 6 ndup - * minus 100 / swap 4 ndup 7 ndup - * 100 / + 5 ndup +"); EC;
  eval("  5 nrotr 4 ndrop ;"); EC;
  eval(": geny swap 3 ndup 6 ndup - * 100 / swap 4 ndup 7 ndup - * 100 / + 4 ndup +"); EC;
  eval("  5 nrotr 4 ndrop ;"); EC;
  eval(": gen 6 copy genx 7 nrotr geny ;"); EC;
  eval(": keep 7 nrotr 7 nrotr pre ;"); EC;
  eval(": post 9 nrot 9 nrot 4 nrot 4 nrot 5 ndup 1- ;"); EC;
  eval(": line moveto lineto ;"); EC;
  eval(": koch dup 0= if drop line else "); EC;
  eval("    pre 0 0 gen keep 33 0 gen post koch"); EC;
  eval("    pre 33 0 gen keep 50 -28 gen post koch"); EC;
  eval("    pre 50 -28 gen keep 66 0 gen post koch"); EC;
  eval("    pre 66 0 gen keep 100 0 gen post koch"); EC;
  eval("    5 ndrop"); EC;
  eval("  endif ;"); EC;
  eval(": kochs 10 setcolor  89 440 551 440 4 koch"); EC;
  eval("        11 setcolor 551 440 320  40 4 koch"); EC;
  eval("        12 setcolor 320  40  89 440 4 koch ;"); EC;
}
