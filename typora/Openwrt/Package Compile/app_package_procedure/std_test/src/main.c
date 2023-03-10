#include "common.h"
#include "main_debug.h"
#include "cfg/cfg.h"
#include "utils/common_utils.h"
#include "ubus/ubus.h"


dbg_md_t g_main_id;
main_gl_t main_gl;

/**
 * @brief: 主函数 debug初始化
 */
static int main_debug_init(void)
{
    int ret = RET_FAIL;

    ret = dbg_init("main",LOG_FILE_PATH,LOG_FILE_SIZE);
    if (ret != 0) {
        fprintf(stderr, "ERROR: debug init failed in %s on %d lines\n",
                __FILE__, __LINE__);
        return RET_ERROR;
    }

    g_main_id = dbg_module_reg("main");
    if (g_main_id < 0) {
        fprintf(stderr, "ERROR: register debug module failed in %s on %d lines\n",
                __FILE__, __LINE__);
        return RET_ERROR;
    }

    MAIN_LOG("main debug module reg success");

    return RET_OK;
}

/**
 * @brief:全局变量的初始化
 */
static int globals_init(void)
{
    MAIN_LOG("Enter globals_init().\n");
    int ret;

    /* 全局变量初始化 */
    memset(&main_gl, 0, sizeof(main_gl_t));

    /* 单实例检查 */
    ret = utils_single_ins();
    if (ret != RET_OK) {
        MAIN_ERROR("another process is running!\n");
        return RET_FAIL;
    }

    /* 配置初始化 */
    ret = cfg_init();
    if (ret != RET_OK) {
        MAIN_ERROR("cfg init failed\n");
        return RET_FAIL;
    }

    /* ubus 初始化 */
    ret = ubus_init();
    if (ret != RET_OK) {
        MAIN_ERROR("ubus init failed\n");
        return RET_FAIL;
    }

    return RET_OK;
}

/**
 * @brief:主函数
 */
int main(int argc,char *argv[])
{
    /* main debug初始化 */
    main_debug_init();
    MAIN_LOG("Enter main().\n");

    /* 创建epoll句柄，最多可监听32个fd */
    uloop_init();

    /* 全局初始化 */
    globals_init();

    uloop_run();
    uloop_done();

    return RET_OK;
}







