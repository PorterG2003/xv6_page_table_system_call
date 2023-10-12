#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  int i=1;
  printf("started pages.c \n");
  printf("argc = %d \n",argc);

  if(argc < 2){
    fprintf(2, "usage: pages pid...\n");
    exit(1);
  }
  //for(i=1; i<argc; i++){
    printf("trying to run pages %s \n",argv[i]);
    printf("trying to run pages %d \n",atoi(argv[i]));
    pages(atoi(argv[i]));
    printf("finished running pages %s\n",argv[i]);
  //}
  exit(0);
}
