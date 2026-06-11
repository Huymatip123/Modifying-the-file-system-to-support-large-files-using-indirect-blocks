#include "kernel/fcntl.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BSIZE 1024

int
main()
{
  int fd, i;
  char buf[BSIZE];

  printf("Start creating big.file...\n");

  fd = open("big.file", O_CREATE | O_RDWR);
  if(fd < 0){
    printf("cannot open file\n");
    exit(1);
  }

  printf("File opened successfully\n");

  for(i = 0; i < 10000; i++){
    if(i % 500 == 0){
      printf("Writing block %d / %d\n", i, 10000);
    }

    int n = write(fd, buf, sizeof(buf));

    if(n != sizeof(buf)){
      printf("write failed at block %d, written=%d\n", i, n);
      break;
    }
  }

  printf("Finished loop, closing file...\n");

  close(fd);

  printf("done\n");
  exit(0);
}
