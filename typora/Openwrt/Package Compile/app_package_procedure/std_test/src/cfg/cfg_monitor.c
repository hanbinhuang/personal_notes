/*
 * Copyright(C) 2020 Ruijie Ctcapd. All right reserved.
 */
/*
 * cfg_render.c
 * Original Author: huanghanbin@ruijie.com.cn, 2023-03-03
 *
 * Description:
 *      监控配置变化功能
 *
 * History:
 *  v1.0    huanghanbin@ruijie.com.cn 2023-03-03
 *          创建初稿
 */
#include <sys/types.h>
#include <libubox/uloop.h>
#include <sys/stat.h>
#include "cfg/cfg_monitor.h"
#include "cfg/cfg_debug.h"

#define MONI_FILE_NUM_MAX       20
#define MONI_FILE_TIMER_TIME    2

/* TODO：可以用链表代替数组实现 */
static moni_ctrl_t g_moni_ctrl[MONI_FILE_NUM_MAX];
static struct uloop_timeout moni_file_change_timer;

static int moni_file_change(moni_ctrl_t *moni_ctrl)
{
    struct stat st_buf;
    const char *file = NULL;

    file = moni_ctrl->file;

    /* printf("Enter moni_file_change(), file: %s\n", file); */

    if (stat(file, &st_buf) == 0) {
        /* printf("Enter moni_file_change(), file: %s is exist!\n", file); */
        if (moni_ctrl->last_st_mtime == 0) {
            moni_ctrl->last_st_mtime = st_buf.st_mtime;
        }
        if (st_buf.st_mtime != moni_ctrl->last_st_mtime) {
            /* printf("monitor file:%s is changed, then callback func\n", file); */
            CFG_LOG("monitor file:%s is changed, then callback func\n", file);
            moni_ctrl->last_st_mtime = st_buf.st_mtime;
            moni_ctrl->cb();
            return 0;
        }
    }

    return 0;
}

static void moni_file_timer_cb(struct uloop_timeout *uloop_t)
{
    int i;

    /* printf("Enter moni_file_timer_cb()\n"); */

    for (i = 0; i < MONI_FILE_NUM_MAX; i++) {//轮询每一个配置文件
        /* printf("Enter for loop, moni_ctrl[%d].status=%d, moni_ctrl[%d].file=%s\n",
            i, g_moni_ctrl[i].status, i, g_moni_ctrl[i].file); */
        if (g_moni_ctrl[i].status == true) {
            /* printf("get monitor file:%s \n", g_moni_ctrl[i].file); */
            moni_file_change(&g_moni_ctrl[i]);
        }
    }
    uloop_t->cb = moni_file_timer_cb;
    uloop_timeout_set(uloop_t, MONI_FILE_TIMER_TIME * 1000);

    return;
}

int moni_file_init(void)
{
    int rc;

    /* printf("Enter moni_file_init()\n"); */

    memset(g_moni_ctrl, 0, MONI_FILE_NUM_MAX * sizeof(moni_ctrl_t));
    moni_file_timer_cb(&moni_file_change_timer);

    return 0;
}

int moni_file_free(void)
{
    /* printf("Enter moni_file_free()\n"); */

    return 0;
}

int moni_file_add(moni_ctrl_t *moni_ctrl)
{
    int i;
    bool flag;

    /* printf("Enter moni_file_add()\n"); */

    for (i = 0; i < MONI_FILE_NUM_MAX; i++) {
        if (g_moni_ctrl[i].status == true) {
            continue;
        }
        g_moni_ctrl[i].status = true;
        strncpy(g_moni_ctrl[i].file, moni_ctrl->file, FILENAME_LEN_MAX);
        g_moni_ctrl[i].cb = moni_ctrl->cb;
        flag = true;
        /* printf("moni_file_add() add new file:%s success\n", g_moni_ctrl[i].file); */
        CFG_LOG("moni_file_add() add new file:%s success\n", g_moni_ctrl[i].file);
        break;
    }

    if (flag == false) {
        /* printf("moni_file_add() add new file:%s failed, reach max:%d\n", g_moni_ctrl[i].file,
            MONI_FILE_NUM_MAX); */
        CFG_ERROR("moni_file_add() add new file:%s failed, reach max:%d\n", g_moni_ctrl[i].file,
            MONI_FILE_NUM_MAX);
        return -1;
    }

    return 0;
}
