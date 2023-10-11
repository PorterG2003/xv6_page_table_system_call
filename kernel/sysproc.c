#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
// i is what page we are looking at (I.E what number in what layer)
// depth is how far we are down, max of 3.
// list is the return of the flags and the physical location

// These are macros provided in riscv.h that we can use as a mask
// #define PTE_V (1L << 0) // valid
// #define PTE_R (1L << 1)
// #define PTE_W (1L << 2)
// #define PTE_X (1L << 3)
// #define PTE_U (1L << 4)

int rec_page(pagetable_t pagetable, int depth)
{
  uint64 entry = 0;
  for (entry = 0; entry < 512; entry++)
  {
    pte_t pte = pagetable[entry];
    if (pte & PTE_V)
    {
      printf("%", pte);
      if (depth == 2)
      {
        // read
        if (!(pte & PTE_R))
        {
          printf("!r");
        }
        // write
        if (!(pte & PTE_W))
        {
          printf("!w");
        }
        // execute
        if (!(pte & PTE_X))
        {
          printf("!x");
        }
        // user accesable
        if (!(pte & PTE_U))
        {
          printf("!u");
        }
        return 0;
      }
    }
    if (depth == 2)
    {
      return 0;
    }
    uint64 child = PTE2PA(pte);
    rec_page((pagetable_t)child, depth + 1);
  }
  return 0;
}


int sys_pages(void)
{
  // get the pid from arguments
  int pid;
  // struct cpu *c=mycpu();
  argint(0, &pid);

  // with the pid, find p
  struct proc *my_p = 0;
  struct proc proc[NPROC];
  struct proc *p;
  uint64 *pt;

  for (p = proc; p < &proc[NPROC]; p++)
  {
    if (p->pid == pid)
    {
      my_p = p;
      break;
    }
  }

  // Check if my_p was set
  if (!my_p)
  {
    printf("No process found with PID %d\n", pid);
    return -1; // Or handle the error in a different way
  }
  else
  {
    // loop through the pagetable
    pt = my_p->pagetable;
    rec_page((pagetable_t)pt,0);
  }
  pt = pt;
  return -1;
}

