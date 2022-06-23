#include "user/user.h"

char readnext()
{
  static char buf[4096];
  static int pos = 0, len = 0;

  if (len == -1)
    return 0;

  if (pos >= len)
  {
    if ((len = read(0, buf, sizeof buf)) <= 0)
    {
      len = -1;
      return 0;
    }
    pos = 0;
  }

  return buf[pos++];
}

int readline(void *buf, int n)
{
  char *ptr = buf;
  for (; n > 0; ptr++, n--)
  {
    char c = readnext();
    if (c == 0)
      return -1;
    if (c == '\n')
      break;
    *ptr = c;
  }
  if (n <= 0)
  {
    fprintf(2, "readline: line too long");
    return -1;
  }
  *ptr = 0;
  return (void *)ptr - buf;
}

int main(int argc, char *argv[])
{
  char *xargv[argc];
  for (int i = 1; i < argc; i++)
  {
    xargv[i - 1] = argv[i];
  }
  char buf[512];
  xargv[argc - 1] = buf;
  while (readline(buf, sizeof buf) != -1)
  {
    if (fork() == 0)
    {
      /* Child */
      exec(argv[1], xargv);
    }
  }
  exit(0);
}
