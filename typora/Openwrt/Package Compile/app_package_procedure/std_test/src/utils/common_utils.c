/*
 * Copyright(C) 2023 Ruijie Network. All rights reserved.
 */
/*
 * common_utils.c
 * Original Author: huanghanbin@ruijie.com.cn, 2023-03-03
 *
 * Description:
 *   通用工具源文件
 *
 * History:
 *  v1.0    huanghanbin@ruijie.com.cn 2023-03-03
 *          创建初稿
 */
#include "common.h"

/* 封装fgets,超时检测 */
static int __fgets(char *buf, int len, FILE *fp)
{
    char *temp = NULL;
    fd_set fds;
    struct timeval tv;
    int ret = -1;

    FD_ZERO(&fds);
    FD_SET(fileno(fp), &fds);
    tv.tv_sec = 30; /* 设置select超时为30秒，避免永久挂住 */
    tv.tv_usec = 0;

    while (1) {
        ret = select(fileno(fp) + 1, &fds, NULL, NULL, &tv);
        if (ret < 0) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        } else if (ret == 0) {
            return -2;
        }
        break;
    }
    /* 读取数据 */
    temp = fgets(buf, len, fp);
    if (temp == NULL) {
        return -3;
    }
    return 0;

}/* End of __fgets */

/**
 *@brief        系统调用命令并执行返回结果
 *@details
 *@param    pCmd 要执行的命令串
 *@param    pResult 命令串返回的结果（如果不关心执行结果传NULL）
 *@param    nSize 结果缓冲区大小（sizeof(pResult),如果不关心返回结果传任意值）
 *@return
 *@li    返回system()执行的返回值/输出结果长度
*/
int system_call(const char* pCmd, char* pResult, int nSize)
{
    FILE *fp;

    /* 参数检测 */
    if (pCmd == NULL || strlen(pCmd) == 0) {
        return -1;
    }

    /* 若不关心返回结果则直接执行system */
    if (pResult == NULL) {
        return system(pCmd);
    } else {
    /* 若关心返回结果则将结果存入pResult中 */
        fp = popen(pCmd, "r");
        if (fp == NULL) {
            return -2;
        }

        /* 写入pResult */
        memset(pResult, 0, nSize);
        if(__fgets(pResult, nSize,fp) < 0) {
            pclose(fp);
            return -3;
        }
        if (pResult[strlen(pResult) - 1] == '\n') {
            pResult[strlen(pResult)-1] = '\0';  /* 去除换行符 */
        }

        pclose(fp);
    }
    return strlen(pResult);
}/* End of system_call */

/**************************************** 内存分配 *******************************************/
mem_cnt_t g_mem_cnt = { 0 };

/* 显示当前内存分配情况 */
void show_malloc(void)
{
    MAIN_LOG("count of memory malloc: %d\n", g_mem_cnt.mloc_cnt);
    MAIN_LOG("count of memory free: %d\n", g_mem_cnt.free_cnt);
    MAIN_LOG("count of memory total malloc: %d\n", g_mem_cnt.total_cnt);
}

/**
 * @brief: 字符串复制，并且分配内存
 * @param str: 输入参数，字符串
 * @return: 返回成功分配内存的指针
 */
char *mem_strdup(const char *str)
{
    if ( str == NULL ) {
        MAIN_ERROR("mem_strdup str is NULL\n");
        return NULL;
    }

    g_mem_cnt.mloc_cnt++;
    g_mem_cnt.total_cnt++;

    return strdup(str);
}/* End of mem_strdup */

/**
 * @brief : 内存分配并初始化
 * @param size: 输入输出参数，待分配内存的大小
 * @return: 返回成功分配内存的指针
 */
void *mem_malloc(size_t size)
{
    void *ptr = NULL;
    MAIN_DEBUG("Enter mem_malloc\n");

    if ( size <= 0 ) {
        MAIN_ERROR("mem_malloc size is error\n");
        return NULL;
    }

    ptr = malloc(size);
    if (ptr != NULL) {
        memset((char *)ptr, 0, size);
    } else {
        MAIN_ERROR("malloc failed! run out of memory, size: %lu\n", size);
        (void)show_malloc();
    }

    g_mem_cnt.mloc_cnt++;
    g_mem_cnt.total_cnt++;

    return ptr;
}/* End of mem_malloc */

/**
 * @brief :  内存释放，并把指针置空
 * @param p: 输入输出参数，待释放内存的指针
 */
void mem_free(void *p)
{
    MAIN_DEBUG("Enter mem_free\n");
    if (p != NULL) {
        free(p);
        p = NULL;
    }

    g_mem_cnt.free_cnt++;
    g_mem_cnt.total_cnt--;
}/* End of mem_free */

/***************************************** 字符串 *****************************************/

/**
 * @brief : 判断字符串是否是数字
 * @param src : 待判断的字符串
 * @return: true:是数字；false:不是数字
 */
int str_is_digit(const char *src)
{
    unsigned int i = 0 ;
    unsigned int len = 0;

    if (src == NULL) {
        MAIN_ERROR("str_is_digit src is null.\n");
        return -1;
    }

    len = strlen(src);
    for (i = 0; i < len; i++) {
        /* isdigit()，不是数字返回0 */
        if (isdigit(src[i]) == 0) {
            /* 字符不是数字，但还要判断第一个字符是否为负号，有可能字符串是负数 */
            if (i == 0 && src[i] == '-') {
                continue;
            }
            return -1;
        }
    }

   return 0;
}/* End of str_is_digit */

/**
 * @brief : 字符串逆序
 * @param src: 待逆序的字符串
 * @return: 逆序后的字符串；空指针 = 逆序失败
 * @note  ：有申请空间，必须外部释放
 */
char *str_reverse(char *src)
{
    char *dst = NULL;
    char *tail = NULL;/* 指向输入字符串尾部 */
    char *head = NULL;/* 指向输出字符串尾部 */

    if (src == NULL) {
        MAIN_ERROR("str_reverse src is null.\n");
        return NULL;
    }

    /* 给返回的变量申请内存空间，在外部释放 */
    dst = (char *)mem_malloc(strlen(src) + 1);
    if (dst == NULL) {
        MAIN_ERROR("dst malloc memory failed\n");
        return NULL;
    }
    memset(dst, 0, strlen(src) + 1);
    head = dst;

    /* 让tail指向字符串尾部的字符 */
    tail = src + (strlen(src) - 1);

    /* 逆序拷贝字符，字符拷贝直接用等号= */
    while (src <= tail) {
        *head++ = *tail--;
    }
    *head ='\0';

    return dst;
}/* End of str_reverse */

/**
 * @brief : 字符串逆序拷贝
 * @param src: 输入参数，待逆序的字符串
 * @param dst: 输出参数，逆序后的字符串
 * @return: 0:逆序成功；非0，逆序失败
 * @note  ：dst必须有内存空间，且空间必须大于等于src
 */
int str_reverse_copy(char *dst, char *src)
{
    char *tail = NULL;/* 指向输入字符串尾部 */
    char *head = NULL;/* 指向输出字符串头部 */

    if (src == NULL || dst == NULL) {
        MAIN_ERROR("src or dst is null pointer\n");
        return -1;
    }
    if (strlen(dst) > strlen(src)) {
        MAIN_ERROR("strlen(dst) is large than strlen(src)\n");
        return -1;
    }

    /* 输出字符串初始化 */
    memset((char *)dst, 0, strlen(src));
    head = dst;

    /* 让tail指向字符串尾部的字符 */
    tail = src + (strlen(src) - 1);

    /* 逆序拷贝字符，字符拷贝直接用等号= */
    while (src <= tail) {
        *head++ = *tail--;
    }
    *head ='\0';

    return 0;
}/* End of str_reverse_copy */



/**
 * @brief:单实例检测
 */
int utils_single_ins(void)
{
    int fd, ret;
    struct flock fl;

    fd = open(LOCK_FILE_NAME, O_CREAT | O_TRUNC | O_RDWR, 0666);
    if (fd < 0) {
        printf("Failed to open file: %s, errno=%d, %s.\n", LOCK_FILE_NAME,
                errno, strerror(errno));
        return -1;
    }

    /* 对整个文件加写锁*/
    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    ret = fcntl(fd, F_SETLK, &fl);
    if (ret < 0) {
        if (errno == EACCES || errno == EAGAIN) {
            close(fd);
        }
        printf("Failed to lock file: %s, errno=%d, %s.\n", LOCK_FILE_NAME,
                errno, strerror(errno));
        return -1;
    }

    return 0;
}