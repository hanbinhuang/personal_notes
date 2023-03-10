/*
 * Copyright(C) 2022 Ruijie Network. All rights reserved.
 */
/*
 * cfg_render.c
 * Original Author: huanghanbin@ruijie.com.cn, 2023-03-03
 *
 * Description:
 *
 *
 * History:
 *  v1.0    huanghanbin@ruijie.com.cn 2023-03-03
 *          创建初稿
 */
#include "common.h"
#include "cfg/cfg_debug.h"
#include "cfg_render.h"



static struct uloop_process g_process_pool[__PROC_POOLS_MAX];

/**
 * @brief:
 *
 * @param proc:
 * @param ret:
 */
static void config_render_proc_cb(struct uloop_process *proc, int ret)
{
    if (proc == NULL) {
        return;
    }

    if (ret != 0) {
        CFG_ERROR("config_render_proc_cb aborted with return code: %d\n", ret);
    }

    proc->pid = 0;

    return;
}


int config_render(char* args[])
{
    int index;

    for (index = 0; index < __PROC_POOLS_MAX; index++) {
        if (g_process_pool[index].pid == 0)
            break;
    }
    if (index == __PROC_POOLS_MAX) {
        CFG_ERROR("config render processes reach MAX:%d\n", __PROC_POOLS_MAX);
        return -1;
    }

    struct uloop_process *proc = &g_process_pool[index];
    proc->cb   = config_render_proc_cb;
    proc->pid  = fork();
    if (proc->pid < 0) {
        CFG_ERROR("config_render failed to fork\n");
        return -1;
    }
    if (!proc->pid) {
        /* Child */
        execvp(args[0], args);
        CFG_ERROR("Failed to exec ctcapd_upgrade\n");
        _exit(EXIT_FAILURE);
    }

    uloop_process_add(proc);

    return RET_OK;
}

/**
 * @brief:
 */
int config_render_init(void)
{
    memset(g_process_pool, 0, sizeof(g_process_pool));

    return RET_OK;
}

