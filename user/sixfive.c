#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc<2){printf("ERROR! no argument\n");exit(0);}
  for(int i=1;i<argc;++i){
    int fp=open(argv[i],0);
    char c;
    char num[200];
    int index=0;
    while(read(fp,&c,1)){
        if(strchr(" -\r\t\n./,",c)==0){
            num[index++]=c;
        }else{
            num[index++]=0;
            int n=atoi(num);
            if(n%5==0||n%6==0){
                printf("%d\n",n);
            }
            index=0;
        }
    }
    num[index++]=0;
    int n=atoi(num);
    if(n%5==0||n%6==0){
        printf("%d\n",n);
    }
  }
  exit(0);
}
