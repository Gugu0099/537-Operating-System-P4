#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "pstat.h"

void spin()
{
	for(;;);
}

int
main(int argc, char *argv[]) {
  settickets(4);
  if (fork() == 0) { // child process 1
    settickets(3);
    spin();
  } 
  spin(); // main process
}
