#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "sysinfo.h"
#include "proc.h"
#include "syscall.h"
#include "defs.h"

int
sys_sysinfo(void)
{
  struct proc *p = myproc();

  struct sysinfo info;
  uint64 addr;
  if(argaddr(0, &addr) < 0)
    return -1;
  info.freemem = memstat();
  info.nproc = procstat();
  if(copyout(p->pagetable, addr, (char *)&info, sizeof(info)) < 0)
    return -1;
  return 0;
}
