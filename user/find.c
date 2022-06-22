#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

const char *extract_filename(const char *path)
{
  const char *lastslash;
  for (lastslash = path; *path; path++)
  {
    if (*path == '/')
      lastslash = path + 1;
  }
  return lastslash;
}

int find(const char *const path, const char *const filename)
{
  int fd;
  if ((fd = open(path, 0)) < 0)
  {
    fprintf(2, "find: cannot open %s\n", path);
    return -1;
  }

  struct stat st;
  if (fstat(fd, &st) < 0)
  {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return -1;
  }

  if (st.type == T_FILE)
  {
    if (strcmp(extract_filename(path), filename) == 0)
      printf("%s\n", path);
    close(fd);
    return 0;
  }

  if (st.type != T_DIR)
  {
    close(fd);
    return -1;
  }

  char buf[512];
  const uint pathlen = strlen(path);
  if (pathlen + 1 + DIRSIZ + 1 > sizeof buf)
  {
    printf("find: path too long\n");
    return -1;
  }
  strcpy(buf, path);
  char *p = buf + pathlen;
  *p++ = '/';
  struct dirent de;
  while (read(fd, &de, sizeof(de)) == sizeof(de))
  {
    if (de.inum == 0 ||
        strcmp(de.name, ".") == 0 ||
        strcmp(de.name, "..") == 0)
      continue;
    char *ptr = p;
    memmove(ptr, de.name, DIRSIZ);
    ptr[DIRSIZ] = 0;
    find(buf, filename);
  }
  close(fd);
  return 0;
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    fprintf(2, "Usage: %s <path> <filename>\n", argv[1]);
    exit(1);
  }
  if (find(argv[1], argv[2]) < 0)
    exit(1);
  exit(0);
}
