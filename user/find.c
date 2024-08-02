/*
 * @Author: wjy：2786484775@qq.com
 * @Date: 2024-07-30 08:47:30
 * @LastEditors: git config Wcacciatori && git config 2786484775@qq.com
 * @LastEditTime: 2024-08-01 22:38:49
 * @FilePath: /xv6-labs-2020/user/find.c
 * @Description: 
 * 
 * Copyright (c) 2024 by ${2786484775@qq.com}, All Rights Reserved. 
 */
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"




void finds(char *path, char* to_find_flie, char *filename);

/**
 * @description: 找到路径倒数第一个文件名
 * @param *path：路径
 * @return 路径倒数第一个文件名   
 */
char* fmtname(char *path)
{
  static char buf[DIRSIZ + 1];
  char *p;

  // Find first character after last slash.
  //找到最后一个斜杠后的第一个字符位置
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));//复制到buf
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));//填充空格达到固定长度
  return buf;//返回最后一个斜杠后的文件名地址
}


int main(int argc, char *argv[])
{
    /* code */
    char* to_find_flie;
    to_find_flie=argv[1];
    finds(".", to_find_flie, "");
    exit(0);
}
/**
 * @description: 查找当前目录树下的文件
 * @param {char} *path：当前路径
 * @param {char*} to_find_flie：要找的文件
 * @param {char} *filename：继承路径上最后一个文件名，用来和tofindfile对比（因为重新把路径信息读到目录项结构de中再读de.name会出错，故出此下策）
 * @return {*}
 */
void finds(char *path, char* to_find_flie, char *filename){
//先判断是目录还是文件，如果是文件，就看看是不是to_find_flie,是就直接把路径打印出来（因为第一个是“.”目录所以会遍历整个根目录）
//如果是目录，开始读目录里的每一项，注意排除.and.. 加到路径中后调用find函数接着找，直到找完整个目录

    
    char buf[512], *p;//buf用来暂存目录下的文件们
    int fd;
    struct dirent de;//目录项结构
    struct stat st;//文件信息

    //printf("test1\n");
    if((fd = open(path, 0)) < 0){//打开路径的文件描述符
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){//把文件描述符的信息保存到结构体方便后续读出
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }
    //这里开始判断
    switch(st.type){
    //如果是个文件，判断是否是目标文件，删除buf最新的文件名（为了下一个文件路径）
    case T_FILE:

        //printf("file分支%s\n", fmtname(path));

        if (strcmp(filename, to_find_flie) == 0) {
            printf("filepath:%s\n", path);
        }
        break;
    //如果是个目录，则继续调用find...............问题所在！忘了排除目录“.”了！！
    case T_DIR:
            strcpy(buf, path);//备份到buf，省的对path改动造成不必要的麻烦
            p = buf+strlen(buf);//p指向路径的最后一个位置
            *p++ = '/';//最后一个位置后面加个“/”，方便后面找buf里的最新文件名（见函数fmtname）            
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                // if(strcmp(de.name, ".")||strcmp(de.name,"..")) {continue;}
                if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
                    continue;
                }
                //printf("%sTT\n", de.name);

                memmove(p, de.name, DIRSIZ);//把读到的文件名加到buf最后
                //printf("dirpath:%s\n", buf);
                p[DIRSIZ] = 0;//加个结束标志
                finds(buf, to_find_flie, de.name);
            }
        
        break;
    }
    
    close(fd);

}
