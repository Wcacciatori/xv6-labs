/*
 * @Author: wjy：2786484775@qq.com
 * @Date: 2024-07-27 16:21:00
 * @LastEditors: git config Wcacciatori && git config 2786484775@qq.com
 * @LastEditTime: 2024-08-04 19:20:36
 * @FilePath: /xv6-labs-2020/kernel/param.h
 * @Description: 
 * 
 * Copyright (c) 2024 by ${2786484775@qq.com}, All Rights Reserved. 
 */
#define NPROC        64  // maximum number of processes
#define NCPU          8  // maximum number of CPUs
#define NOFILE       16  // open files per process
#define NFILE       100  // open files per system
#define NINODE       50  // maximum number of active i-nodes
#define NDEV         10  // maximum major device number
#define ROOTDEV       1  // device number of file system root disk
#define MAXARG       32  // max exec arguments
#define MAXOPBLOCKS  10  // max # of blocks any FS op writes
#define LOGSIZE      (MAXOPBLOCKS*3)  // max data blocks in on-disk log
#define NBUF         (MAXOPBLOCKS*3)  // size of disk block cache
#define FSSIZE       1000  // size of file system in blocks
#define MAXPATH      128   // maximum file path name
