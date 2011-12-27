/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* <strings.c> --- ʸ�������ط��δؿ����
 *
 * $Date: 2011/12/27 17:13:36 $
 * $Version$
 * $Author: liu1 $
 *
 * Discription
 *
 */

#include "types.h"
#include "strings.h"

extern WORD	tron_to_number_1 (TPTR s, WORD d);
extern WORD	tcode_to_number (TCODE s);

/*************************************************************************
 * strlen
 *
 * ������
 *
 * ���͡�
 *
 * ������
 *
 */
WORD
strlen (BPTR s)
{
  int	i;

  for (i = 0; *s != '\0'; i++, s++)
    ;
  return (i);
}

/*************************************************************************
 * strnlen ---
 *
 * ������
 *
 * ���͡�
 *
 * ������
 *
 */
WORD
strnlen (BPTR s, WORD len)
{
  WORD	i;

  for (i = 0; (*s != '\0') && (i < len); i++, s++)
    ;
  return (i);
}

/*************************************************************************
 * strcpy --- 
 *
 * ������
 *
 * ���͡�
 *
 * ������
 *
 */
WORD
strcpy (BPTR s1, BPTR s2)
{
  WORD i;
  
  for (i = 0; *s2; i++)
    {
      *s1++ = *s2++;
    }
  *s1 = *s2;
  return (i);
}

/*************************************************************************
 * strncpy --- 
 *
 * ������
 *
 * ���͡�
 *
 * ������
 *
 */
WORD
strncpy (BPTR s1, BPTR s2, WORD n)
{
  WORD i;
  
  for (i = 0; *s2; i++)
    {
      if (i >= n)
	{
	  *s1 = '\0';
	  return (i);
	}
      *s1++ = *s2++;
    }
  *s1 = *s2;
  return (i);
}

/*************************************************************************
 * strncpy_with_key --- 
 *
 * ������
 *
 * ���͡�
 *
 * ������
 *
 */
WORD
strncpy_with_key (BPTR s1, BPTR s2, WORD n, int key)
{
  WORD i;
  
  for (i = 0; *s2; i++)
    {
      if ((i >= n) || (*s2 == key))
	{
	  *s1 = '\0';
	  return (i);
	}
      *s1++ = *s2++;
    }
  *s1 = *s2;
  return (i);
}

/***********************************************************************
 * strcat -- ���Ĥ�ʸ�����Ϣ��
 *
 * ������
 *
 * ���͡�
 *
 * ������
 *
 */
TPTR
strcat (TPTR s1, TPTR s2)
{
  TPTR ret_val = s1;
  
  while (*s1++)
    ;
  while (*s2)
    {
      *s1++ = *s2++;
    }
  return ret_val;
}

/***********************************************************************
 * strncat -- ���Ĥ�ʸ�����Ϣ��
 *
 * ������
 *
 * ���͡�
 *
 * ������
 *
 */
TPTR
strncat (TPTR s1, TPTR s2, WORD n)
{
  TPTR	ret_val = s1;
  WORD	i;
  
  while (*s1++)
    ;
  for (i = 0; *s2; i++)
    {
      if (i >= n)
	break;
      *s1++ = *s2++;
    }
  return ret_val;
}

/***********************************************************************
 * strcmp --- ���Ĥ�ʸ��������
 *
 * ������
 *
 * ���͡�
 *
 * ������
 *
 */
ULONG
strcmp (BPTR s1, BPTR s2)
{
  while (*s1 && *s2)
    {
      if (*s1 != *s2)
        {
          return (*s2 - *s1);
        }
      s1++;
      s2++;
    }
  return (*s1 - *s2);
}

/***********************************************************************
 * strchr -- ʸ�������˻��ꤷ��ʸ�����ޤޤ�Ƥ��ʤ�����Ĵ�٤롣
 *
 *
 */
char *
strchr (char *s, int ch)
{
  while (*s != '\0')
    {
      if (ch == *s)
	{
	  return (s);
	}
      s++;
    }
  return (NULL);
}

/***********************************************************************
 * strnchr -- ʸ�������˻��ꤷ��ʸ�����ޤޤ�Ƥ��ʤ�����Ĵ�٤롣
 *
 *
 */
char *
strnchr (char *s, int ch, int n)
{
  int	i;

  for (i = 0; *s != '\0'; i++)
    {
      if (i < n)
	break;
      if (ch == *s)
	{
	  return (s);
	}
      s++;
    }
  return (NULL);
}


/***********************************************************************
 * strncmp -- ���Ĥ�ʸ��������
 *
 * ������
 *
 * ���͡�
 *
 * ������
 *
 */
ULONG
strncmp (BPTR s1, BPTR s2, WORD n)
{
  WORD	i;

  for (i = 0; (i < n) && *s1 && *s2; i++)
    {
      if (*s1 != *s2)
        return (*s2 - *s1);
      s1++;
      s2++;
    }
  if (i == n)
    return 0;
  return (*s1 - *s2);
}

/*************************************************************************
 * tron_to_number 
 *
 * ������	s
 *
 * ���͡�	�Ѵ���ο���
 *
 * ������
 *
 */
GLOBAL WORD
tron_to_number (TPTR s)
{
  WORD	retval = 0;
  LOCAL TPTR minus = (TPTR)"��";

  string_shift_to_tron (minus);
  if (strncmp ((BPTR)s, (BPTR)minus, sizeof (TCODE)) == 0)
    {
      retval = tron_to_number_1 (++s, 0);
      retval = 0 - retval;
    }
  else
    {
      retval = tron_to_number_1 (s, 0);
    }
  string_tron_to_shift (minus);
  return (retval);
}

/*************************************************************************
 * tron_to_number_1 -- tron_to_number �β������롼����
 *
 * ������	s
 *
 * ���͡�	
 *
 * ������
 *
 */
LOCAL WORD
tron_to_number_1 (TPTR s, WORD d)
{
  WORD i;

  if (*s == TNULL)
    {
      return (d);
    }

  i = tcode_to_number (*s);
  return (tron_to_number_1 (++s, d * 10 + i));
}

/*************************************************************************
 * tcode_to_number --- �ԣңϣΥ�����1ʸ������ͤ��Ѵ�����
 *
 * ������	s	tron�����ɤ�ʸ��
 *
 * ���͡�	-1	�Ѵ��Ǥ��ʤ�
 *		!= -1	�Ѵ���ο���
 *
 * ������      
 *
 */
LOCAL WORD
tcode_to_number (TCODE s)
{
  WORD i;
  LOCAL TPTR number_char = (TPTR)"��������������������";

  string_shift_to_tron (number_char);
  for (i = 0; i <= 9; i++)
    {
      if (strncmp ((BPTR)&s, (BPTR)(number_char[i]), sizeof (TCODE)) == 0)
	{
	  string_tron_to_shift (number_char);
	  return (i);
	}
    }
  
  string_tron_to_shift (number_char);
  return (-1);
}
  

toupper (int ch)
{
  if ((ch >= 'a') && (ch <= 'z'))
    {
      ch = ch + ('A' - 'a');
    }
  return (ch);
}
