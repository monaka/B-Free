/*
   FoRTh Modoki
   frtm.h ver 1.3.0 B-Free 1.0
   Copyright (C) 1999 Tomohide Naniwa. All rights reserved.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Last modified: Dec 29, 1999.
*/

#define BFREE
 
typedef enum {RUN, SKIP, DIC1, DIC2, FGT, DEF,
		IF, DO, FOR, ON, OFF} MODE;

/* エラーコード */
#define E_QUT (-1)
#define E_OK  0
#define E_VSO 1
#define E_VSU 2
#define E_UKC 3
#define E_DV0 4
#define E_IVN 5
#define E_UCB 6
#define E_DSO 7
#define E_DSU 8
#define E_CSO 9
#define E_RSO 10
#define E_RSU 11
#define E_IVC 12
#define E_SSO 13
#define E_SSU 14
#define E_ILM 15
#define E_ILS 16
#define E_ILC 17
#define E_COR 18
#define E_SOR 19

#define STMAX 100
#define DICMAX 20
#define CSMAX 200
#define RSMAX 100
#define SSMAX 80
#define MAX_NAME 8
#define LINE 100
#define HASH 11

#ifdef BFREE
#ifdef notdef
#define pop POP
#define eval EVAL
#endif
#define isdigit isnum
#endif

#ifdef MAIN
int error_no;
MODE eval_mode = RUN;
MODE memory = OFF;
#else
extern int error_no;
extern MODE eval_mode;
extern MODE memory;
#endif
#define EC if (error_no != E_OK) return;
