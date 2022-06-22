#include <user/user.h>

static int create_filter(const int n);
static int do_filter(const int r, const int n);

const int MAX = 35;
int main()
{
  int pipes = create_filter(2);
  for (int i = 2; i < MAX; i++)
  {
    write(pipes, &i, sizeof i);
  }
  close(pipes);
  wait(0);
  exit(0);
}

int create_filter(const int n)
{
  int p[2];
  pipe(p);
  if (fork() == 0)
  {
    /* Child */
    close(p[1]);
    do_filter(p[0], n);
  }
  {
    /* Parent */
    close(p[0]);
    return p[1];
  }
}

int do_filter(const int r, const int n)
{
  int w = -1;
  int buf;

  while (read(r, &buf, sizeof buf) > 0)
  {
    if (buf == n)
    {
      printf("prime %d\n", buf);
      continue;
    }
    if (buf % n == 0)
    {
      continue;
    }
    if (w == -1)
      w = create_filter(buf);
    write(w, &buf, sizeof buf);
  }

  if (w > 0)
  {
    close(w);
    wait(0);
  }

  close(r);
  exit(0);
}
