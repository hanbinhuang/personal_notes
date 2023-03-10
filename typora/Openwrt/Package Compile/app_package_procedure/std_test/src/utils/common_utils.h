/*
 * Copyright(C) 2023 Ruijie Network. All rights reserved.
 */
/*
 * common_utils.h
 * Original Author: huanghanbin@ruijie.com.cn, 2023-03-03
 *
 * Description:
 *   通用工具头文件
 *
 * History:
 *  v1.0    huanghanbin@ruijie.com.cn 2023-03-03
 *          创建初稿
 */
#ifndef __COMMON_UTILS_H__
#define __COMMON_UTILS_H__

/* 内存统计结构体 */
typedef struct mem_cnt_s {                          /* 内存控制 */
    int mloc_cnt;                                   /* 内存分配次数 */
    int free_cnt;                                   /* 内存释放次数 */
    int total_cnt;                                  /* 内存分配总次数，分配就加1，释放就减1 */
} mem_cnt_t;


/* 内存分配函数 */
void show_malloc(void);                             /* 显示当前内存分配情况 */
char *mem_strdup(const char *str);                  /* 内存分配并初始化 */
void *mem_malloc(size_t size);                      /* 内存释放，并把指针置空 */

/* 字符串 */
int str_is_digit(const char *src);                  /* 判断字符串是否是数字 */
char *str_reverse(char *src);                       /* 字符串逆序,需释放内存 */
int str_reverse_copy(char *dst, char *src);         /* 字符串逆序拷贝 */

int system_call(const char* pCmd, char* pResult, int nSize);    /* 系统调用并返回结果 */


int utils_single_ins(void);                         /* 单实例检测 */

#endif /* __COMMON_UTILS_H__ */