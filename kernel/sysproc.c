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

int
sys_pages(void)
{
//get the pid from arguments
  int pid;
  struct cpu *c=mycpu();
  argint(0, &pid);

  

//with the pid, find p
  int found = 0;
  struct proc *my_p;
  struct proc *proc;
  struct proc *p;
  uint64 pt[NPROC];

  for( p = proc; p < &proc[NPROC]; p++) {
      if (p->pid == pid) {
          found = 1;
          my_p = p;
          break;
      }
  }

//loop through the pagetable
  pt = my_p->pagetable;

return -1;
}

//i is what page we are looking at (I.E what number in what layer)
//depth is how far we are down, max of 3.
//list is the return of the flags and the physical location
uint64 rec_page(i,depth,list){
  uint64 j=i;
  while(j, j<512,j++ ){
    if (j &&1){
      if (depth==3){
        
      }
      rec_page(j,depth+1,list);
    }

  }
}