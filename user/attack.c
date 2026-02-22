#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

#define DATASIZE (8*4096)

char data[DATASIZE];
int
main(int argc, char *argv[])
{
  // Your code here.
  char *p=data-(4*4096);
  printf("%s\n",p);
  exit(1);
}
