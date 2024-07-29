#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

//递归的思想：声明一个函数，在此函数内部再次调用该函数。注意结束条件？：管道中的数据读完了
void prime(int lpipe[2]);
int main(int argc, char const *argv[])
{
    // close(0);
    // close(1);   
    int i = 0;
    int p[2];
    int *buf;

    pipe(p);

    for(i=2; i<36; i++){
        buf = &i;
        write(p[1], buf, sizeof(i));
    }

    prime(p);

    exit(0);
}

void prime(int lpipe[2]){
    close(lpipe[1]);
    int first, num;
        if(read(lpipe[0], &first, sizeof(first)) != 0){
            printf("prime %d\n", first);
            if(fork() == 0){//在子进程中进行发送工作
                int rpipe[2];
                pipe(rpipe);
                while(read(lpipe[0], &num, sizeof(num))){
                    if((num % first)!=0){
                        write(rpipe[1], &num, sizeof(num));
                    }
                }
                close(lpipe[0]);
                prime(rpipe);
            }else{
                close(lpipe[0]);
                wait(0);
            }
        }

}
