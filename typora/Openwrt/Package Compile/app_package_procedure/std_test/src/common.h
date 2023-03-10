/*
 * Copyright(C) 2022 Ruijie Network. All rights reserved.
 */
/*
 * ubus.c
 * Original Author: huanghanbin@ruijie.com.cn, 2023-03-03
 *
 * Description:
 *      应用的通用全局配置，如：
 *          头文件，宏定义，枚举，结构体等。
 *
 * History:
 *  v1.0    huanghanbin@ruijie.com.cn 2023-03-03
 *          创建初稿
 */
#ifndef __COMMON_H__
#define __COMMON_H__

#include <unistd.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <uci.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libubox/blobmsg_json.h>
#include <libubox/uloop.h>
#include <libubox/list.h>
#include <libubus.h>
#include <json-c/json.h>
#include <libdebug/libdebug.h>

#include "utils/common_utils.h"
#include "main_debug.h"

/************************************** 宏定义类型 **************************************/
/* 目录及文件路径 */
#define TEST_DIR                        "/tmp/test"                             /* tmp目录路径 */
#define LOG_FILE_PATH                   "/tmp/test/test.log"                    /* log文件 */
#define LOG_FILE_SIZE                   100                                     /* log大小 */
#define LOCK_FILE_NAME                  "/tmp/test/.running"                    /* 锁文件 */

/*************************************** 枚举类型 ***************************************/
/*	返回值状态 */
typedef enum _Ret
{
	RET_OK = 0,                                         /* 正常 */
    RET_FAIL = 1,                                       /* 失败 */
    RET_ERROR = 2,                                      /* 错误 */
    RET_INVALID_PARAMS = 3,                             /* 无效参数 */
    RET_STOP = 4,                                       /* 停止 */
    RET_OOM = 5,                                        /* 内存溢出 */
    RET_NULL = 6,                                       /* 空指针 */
}Ret;


/*************************************** 结构体类型 ***************************************/

/* 全局变量 */
typedef struct _main_gl_s {
    /* malloc count */
    int g_malloc_times;                                  /* 内存分配计数 */
    int g_free_times;                                    /* 内存释放计数 */

    /* ubus */
    struct ubus_context* g_ubus_ctx;                     /* ubus上下文 */
    struct ubus_event_handler g_event;                   /* ubus事件处理句柄 */

} main_gl_t;


#endif /* __COMMON_H__ */