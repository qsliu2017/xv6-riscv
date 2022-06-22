#include "user/user.h"

int main(int argc, char *argv[])
{
  if (argc <= 1)
  {
    fprintf(2, "Usage: %s <second>\n", argv[0]);
    exit(1);
  }

  int sec = atoi(argv[1]);
  if (sec <= 0)
  {
    fprintf(2, "sleep: second should be positive\n");
    exit(1);
  }

  if (sleep(sec) != 0)
  {
    fprintf(2, "sleep: kernel error\n");
    exit(1);
  }

  exit(0);
}
