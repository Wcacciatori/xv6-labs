#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]){
    if(argc == 1){
        printf("输入参数啊\n");
        exit(1);
    }
    if(argc > 2){
        printf("一个参数就够了\n");
        exit(1);
    }
    int time = 0;
    time = atoi(argv[1]);
    sleep(time);
    exit(0);
}


