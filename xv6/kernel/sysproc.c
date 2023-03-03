#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "sysfunc.h"
#include "pstat.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since boot.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// set the number of tickets of the calling process.
// By default, each process should get one ticket
// calling this routine makes it such that a process can raise the number of tickets it receives,
// and thus receive a higher proportion of CPU cycles
// return 0 if seccessful, 
// -1 otherwise Ex. caller passes in a number less than one
int
sys_settickets(void){
  int num;
  if(argint(0, &num) < 0 || num <= 0){
    // either call is not successful or ticket num not valid
    return -1; 
  }
  int maxStride = 12;
  proc -> tickets = num;
  proc -> strides = maxStride / num; // formula to strides

  return 0;
}

int 
sys_getpinfo(void){
  struct pstat *table;
  table = NULL;
  if(argptr(0, (void *)&table, sizeof(table)) < 0){
    // fetching argument is not successful
    return -1;
  }
  if(table == NULL){
    return -1;
  }

  return getpinfo(table);
}
