#include <user/user.h>

int main()
{
  int p[2];
  pipe(p);

  if (fork() == 0)
  {
    /* Child */
    char buf[8];
    read(p[0], buf, sizeof buf);
    printf("%d: received ping\n", getpid());
    write(p[1], "ping", 1);

    exit(0);
  }
  else
  {
    /* Parent */
    char buf[8];
    write(p[1], "ping", 1);
    read(p[0], buf, sizeof buf);
    printf("%d: received pong\n", getpid());

    close(p[0]);
    close(p[1]);
    exit(0);
  }
}
