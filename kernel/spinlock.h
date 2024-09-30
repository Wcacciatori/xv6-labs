/*
 * @Author: wjy：2786484775@qq.com
 * @Date: 2024-09-20 20:07:13
 * @LastEditors: git config Wcacciatori && git config 2786484775@qq.com
 * @LastEditTime: 2024-09-30 15:04:37
 * @FilePath: /xv6-lab2-2020/kernel/spinlock.h
 * @Description: 
 * 
 * Copyright (c) 2024 by ${2786484775@qq.com}, All Rights Reserved. 
 */
// Mutual exclusion lock.
struct spinlock {
  uint locked;       // Is the lock held?

  // For debugging:
  char *name;        // Name of lock.
  struct cpu *cpu;   // The cpu holding the lock.
};

