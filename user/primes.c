#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

//递归的思想：声明一个函数，在此函数内部再次调用该函数。注意结束条件？：管道中的数据读完了
void prime(int lpipe[2]);
int main(int argc, char const *argv[])
{
    int i = 0;
    int p[2];


    pipe(p);

    for(i=2; i<36; i++){//把所有数据送进管道
        write(p[1], &i, sizeof(i));
    }

    prime(p);//开始试炼

    exit(0);
}

void prime(int lpipe[2]){
    close(lpipe[1]);//先把左邻居的写端关了
    int first, num;//用来保存从管道中读出的数据，第一个（打印）和其余的继续判断
        if(read(lpipe[0], &first, sizeof(first)) != 0){//把第一个读出来
            printf("prime %d\n", first);//打印
            if(fork() == 0){//在子进程中进行发送工作
                int rpipe[2];//定义右管道的两个口
                pipe(rpipe);//创建管道
                while(read(lpipe[0], &num, sizeof(num))){//循环从管道中读数据
                    if((num % first)!=0){//不能被第一个数整除，说明可能是素数（若能被整除则一定不是素数）
                        write(rpipe[1], &num, sizeof(num));//把有成为素数潜力的同志们送到下一次试炼
                    }
                }
                close(lpipe[0]);//关闭子进程中的左邻居读端
                prime(rpipe);//开启下一次试炼
            }else{
                close(lpipe[0]);//关闭父进程中的左邻居读端
                wait(0);//父进程等子进程结束，否则同时进行打印要出错了
            }
        }

}
