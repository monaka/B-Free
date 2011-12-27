#include <stdlib.h>

int main(int argc,char *argv[])
{
int i;
  for(i=0;i<argc;i++){
    printf("argv[%d]=%s",i,*argv);
  }
  printf("Hello World\n");
}
