#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

char*
filename(char *path)
{
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  return p;
}
// 添加新的递归函数
void find_exec(char *path, char *name, char *prog, char *arg)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, O_RDONLY)) < 0){
    return;
  }

  if(fstat(fd, &st) < 0){
    close(fd);
    return;
  }
  
  switch(st.type){
  case T_DEVICE:
  case T_FILE:
    if(strcmp(filename(path), name) == 0){
      if(prog==0){  //没有-exec参数和后续的程序名称
        write(1,path,strlen(path));
        write(1,"\n",1);
      }else{
        // 对每个找到的文件执行 grep
        int pid = fork();
        if(pid == 0){
          char * argv[MAXARG];
          argv[0]=prog;
          if(arg!=0){
            argv[1] = arg;
            argv[2] = path;
            argv[3] = 0;
          }else{
            argv[1] = path;
            argv[2] = 0;
          }

          exec(prog, argv);
          exit(0);
        } else if(pid > 0){
          wait((int *)0);
        }
      }
    }
    break;
  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      if(strcmp(de.name,".")==0||strcmp(de.name,"..")==0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      find_exec(buf, name, prog, arg);
    }
    break;
  }
  close(fd);
}



int
main(int argc, char *argv[])   // 误区！！不能通过管道将搜索到的文件名传给grep，管道传递的是纯文本，grep不会把管道传递的内容作为文件路径参数，而是会在管道传递的内容进行搜索。
{
  if(argc < 3){
    printf("argument error!\n");
    exit(0);
  }

  if(argc>3&&strcmp(argv[3],"-exec")==0){
    if(argc < 5){  // 确保参数格式正确
      printf("Usage: find <path> <name> -exec <prog> <arg>\n");
      exit(0);
    }
    find_exec(argv[1], argv[2], argv[4], argv[5]);
  }else{
    find_exec(argv[1],argv[2],(char*)0,(char*)0);
  }
  exit(0);
}