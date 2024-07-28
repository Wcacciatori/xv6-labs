#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"



int main(int argc, char *argv[]){
    if((argc!=1)){
        printf("参数错误！");
        exit(1);
    }
    int p1[2], p2[2];
    pipe(p1);
    pipe(p2);
    if((fork()) == 0){
        printf("%d: received ping\n", getpid());
        
        close(0);
        close(1);
        dup(p1[0]);//子进程p10作为输入
        dup(p2[1]);//子进程p21作为输出 
        read(p1[0], "byte", 4);
        write(p2[1], "return", 6);
        close(p1[0]);
        close(p2[1]);
        printf("123");
        exit(0);
    }else{
        sleep(9);

        printf("%d: received pong\n", getpid());
        close(0);
        close(1);
        dup(p2[0]);//父进程p20作为输入
        dup(p1[1]);//父进程p11作为输出
        close(p2[0]);
        close(p1[1]);
        exit(0);
    }


    exit(0);
}