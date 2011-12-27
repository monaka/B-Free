/*
   FoRTh Modoki
   debug.c ver 1.1
   Copyright (C) 1999, 2000 Tomohide Naniwa. All rights reserved.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Last modified: Jan. 30, 2000.
*/

#include "stdlib.h"
#include "frtm.h"

void f_tasklist (void)
{
  misc (-6, 0, NULL);
}

void f_pmemstat(void)
{
  misc (-7, 0, NULL);
}

void f_prtflist(void)
{
  /* malloc の空きリストの表示 */
  misc (4, 0, NULL);
}
