#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  if(argc < 2){
    printf("usage: pages pid\n");
    exit(1);
  }
  //printf("Calling pages on process: %s \n", argv[1]);
  pages(atoi(argv[1]));

  exit(0);
}