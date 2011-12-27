/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/


char *
get_next_path (char *path, char *entry)
{
  while ((*path != '/') && (*path != '\0'))
    {
      *entry++ = *path++;
    }
  *entry = '\0';
  if (*path == '/')
    {
      path++;
    }
  return (path);
}

fix_path (char *path)
{
  char *p;

  for (p = path; *p; p++)
    {
      *p = toupper (*p);
    }
}
