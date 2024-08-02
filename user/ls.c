#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


/**
 * @description: 找到路径倒数第一个文件名
 * @param *path：路径
 * @return 路径倒数第一个文件名   
 */
char* fmtname(char *path)
{
  static char buf[DIRSIZ+1];
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

/**
 * @description: 列出指定目录下的文件和目录
 * @param *path：指定的路径
 * @return null
 */
void ls(char *path)
{
  char buf[512], *p;//buf用来暂存目录下的文件们
  int fd;
  struct dirent de;//目录项结构
  struct stat st;//文件信息

  if((fd = open(path, 0)) < 0){//打开路径的文件描述符
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){//把文件描述符的信息保存到结构体方便后续读出
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  //如果是个文件，直接打印
  case T_FILE:
    printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
    break;
  //如果是个目录，则：
  case T_DIR:
    //先判断是否过长
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);//备份到buf，省的对path改动造成不必要的麻烦
    p = buf+strlen(buf);//p指向路径的最后一个位置
    *p++ = '/';//最后一个位置后面加个“/”，方便后面找buf里的最新文件名（见函数fmtname）
    
    while(read(fd, &de, sizeof(de)) == sizeof(de)){//开始从文件描述符读目录项到de中
      if(de.inum == 0)//如果目录项被删除或者未被使用过，则跳过
        continue;
      memmove(p, de.name, DIRSIZ);//把读到的文件名加到buf里
      p[DIRSIZ] = 0;//加个结束标志
      if(stat(buf, &st) < 0){//把buf里的文件信息存到st
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;

  //没有其他参数，则列出当前目录内容
  if(argc < 2){
    ls(".");
    exit(0);
  }

  //遍历列出每个指定路径内容
  for(i=1; i<argc; i++)
    ls(argv[i]);
  exit(0);
}
