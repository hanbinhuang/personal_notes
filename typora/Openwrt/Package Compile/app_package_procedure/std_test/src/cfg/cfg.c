/*
 * Copyright(C) 2022 Ruijie Network. All rights reserved.
 */
/*
 * cfg.c
 * Original Author: huanghanbin@ruijie.com.cn, 2023-03-03
 *
 * Description:
 *    读取相关配置信息
 *
 * History:
 *  v1.0    huanghanbin@ruijie.com.cn 2023-03-03
 *          创建初稿
 */

#include "cfg.h"
#include "cfg_debug.h"
#include "common.h"

dbg_md_t g_cfg_id = -1;
extern main_gl_t main_gl;


/**
 * @brief: cfg debug初始化
 */
 static int cfg_debug_init(void)
 {
    g_cfg_id = dbg_module_reg("cfg");
    if (g_cfg_id < 0) {
        CFG_ERROR("register debug module failed \n");
        return -1;
    }

    return RET_OK;
 }


/**
 * @brief:加载默认配置
 *
 * @description:
 *      加载应用的启机默认配置
 */
static int cfg_load_default_config(void)
{
    CFG_LOG("Enter cfg_load_default_config.\n");

    return RET_OK;
}

/**
 * @brief:加载设备的sysinfo信息
 */
static int cfg_load_sysinfo(void)
{
    CFG_LOG("Enter cfg_load_sysinfo.\n");

    return RET_OK;
}
/**
 * @brief:配置初始化
 *
 * @description:
 *      读取相关配置
 */
int cfg_init(void)
{
    int ret;

    /* debug初始化 */
    ret = cfg_debug_init();
    if (ret != RET_OK) {
        CFG_ERROR("ubus debug init is error\n");
        return RET_ERROR;
    }
    CFG_LOG("dbg_module_reg() cfg success\n");

    /* 加载应用默认配置 */
    ret = cfg_load_default_config();
    if (ret != RET_OK) {
        CFG_ERROR("cfg load default config  failed\n");
        return RET_ERROR;
    }
    CFG_LOG("load default config success\n");

    /* 加载设备sysinfo信息 */
    ret = cfg_load_sysinfo();
    if (ret != RET_OK) {
        CFG_ERROR("cfg load sysinfo  failed\n");
        return RET_ERROR;
    }
    CFG_LOG("load sysinfo success\n");

    return RET_OK;
}




