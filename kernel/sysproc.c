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
  //struct cpu *c=mycpu();
  argint(0, &pid);

  

  //with the pid, find p
  struct proc *my_p = 0;
  struct proc proc[NPROC];
  struct proc *p;
  uint64 pt[NPROC];

  for( p = proc; p < &proc[NPROC]; p++) {
      if (p->pid == pid) {
          my_p = p;
          break;
      }
  }

  // Check if my_p was set
  if (!my_p) {
      printf("No process found with PID %d\n", pid);
      return -1; // Or handle the error in a different way
  } else {
    //loop through the pagetable
    for (int i = 0; i < NPROC; i++) {
        pt[i] = my_p->pagetable[i]; 
        printf("%llu\n", pt[i]);
    }
  }

  return -1;
}

//i is what page we are looking at (I.E what number in what layer)
//depth is how far we are down, max of 3.
//list is the return of the flags and the physical location
int rec_page(int i, int depth){
  uint64 j=i;
  while(j<512){
    if (j || 1111111111111110  == 11111111111111111){
      if (depth==3){
        //read
         if (j || 1111111111111101  != 11111111111111111){
          printf("!r");
         }
         //write
         if (j || 1111111111111011  != 11111111111111111){
          printf("!w");
         }
         //execute
         if (j || 1111111111110111  != 11111111111111111){
          printf("!x");
         }
         //user accesable
         if (j || 1111111111101111  != 11111111111111111){
          printf("!u");
         }
        return 0;
      }
      printf("j");
      rec_page(j,depth+1);
    }

  }
  return 0;
}