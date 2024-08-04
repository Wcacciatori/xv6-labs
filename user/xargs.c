/*
 * @Author: wjy：2786484775@qq.com
 * @Date: 2024-08-02 19:45:46
 * @LastEditTime: 2024-08-04 19:47:08
 * @FilePath: /xv6-labs-2020/user/xargs.c
 * @Description: 
 * 
 * Copyright (c) 2024 by ${2786484775@qq.com}, All Rights Reserved. 
 */
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define buf_size 512

//思路(显然不对，未能理解“|”的作用)：先检测管道前的命令，在新进程中得到输出，通过管道传给父进程，
//再存到变量里作为管道后命令的参数,管道前后的命令都需要在新进程中执行

//阅读https://lonelyuan.github.io/p/xv6-utilities/ 代码后新的理解：
//管道前面的是会自动执行的，输出的结果就在标准输出中,之后从标注输出也就是文件描述符0中读数据就行了，这就是参数


int readline(char* buf) {
    char ch;
    int bytesRead = 0;
    while (read(0, &ch, 1)) { // Read a char from stdout
        if (ch != '\n') {
            buf[bytesRead++] = ch; 
            if (bytesRead + 1 >= MAXARG) {
                printf("xargs: line too long\n");
                return -1;
            }
        } else { // End of line
            if (bytesRead > 0) {
                buf[bytesRead] = '\0'; // Null terminate the string
                return bytesRead;
            }
        }
    }
    return bytesRead;
}

int main(int argc, char *argv[]) {
  char buf[buf_size + 1] = {0};
  char *xargv[MAXARG] = {0};
//   int stdin_end = 0;

  for (int i = 1; i < argc; i++) {
    xargv[i - 1] = argv[i];
  }

  while (1) {
    int read_bytes = readline(buf);
    if (read_bytes <= 0) break;
    char xbuf[buf_size + 1] = {0};
    memcpy(xbuf, buf, strlen(buf));
    xargv[argc - 1] = xbuf;
    if (fork() == 0) { // in child
      if (exec(argv[1], xargv) < 0) {
        fprintf(2, "xargs: exec fails with -1\n");
        exit(1);
      }
    } else {
      int pid;
      wait(&pid);
    }
  }
  exit(0);
}

// void main(int argc, char *argv[])
// {
//     int i=0, j=0, k=0;
//     char *argvF[99], *argvN[99];
//     int p[2];
//     pipe(p);

//     //两个for循环分别保存管道前后的命令行指令和参数
//     for(i=0;i<argc;i++){
//         if(strcmp(argv[i], "|")){
//             argvF[j++] = argv[i];
//         }else{
//             break;
//         }
//     }
//     i++;//第一次是为了跳过“|”
//     i++;//第二次是为了跳过xargs
//     argvF[j] = "\0";
//     for(;i<argc;i++){
//         argvN[k++] = argv[i];
//     }
//     // argvN[k] = "\0";

//     //新建进程执行管道前的命令,把新进程（称为1号进程）的输出重定向到管道输入，然后再2号进程中通过此管道读，就实现了
//     if(fork()==0){


//         close(1);//关闭标准输出
//         dup(p[1]);//复制一份管道写端，代替原来的标准输出，使|左边的命令输出结果到管道中
//         exec((char *)argvF[0], argvF);
//         close(p[0]);
//         close(p[1]);

//     }else{
//         wait(0);
//     }

//     //创建第二个进程，用来执行管道后面的命令
//     if(fork()==0){
//         //在第二个进程中，需要把管道中的内容加到参数数组中，接着调用exec执行命令
//         while(read(p[0], argvN[k++], sizeof(argvN[k]))!=0);
//         exec((char *)argvN[0], argvN);
//         close(p[0]);
//         close(p[1]);
//     }else{
//         wait(0);
//     }
//     close(p[0]);
//     close(p[1]);
//     exit(0);
    
// }
