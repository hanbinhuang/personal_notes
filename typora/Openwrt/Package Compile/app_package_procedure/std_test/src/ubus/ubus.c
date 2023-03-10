/*
 * Copyright(C) 2022 Ruijie Network. All rights reserved.
 */
/*
 * ubus.c
 * Original Author: huanghanbin@ruijie.com.cn, 2023-03-03
 *
 * Description:
 *   ubus事件处理文件，事件的监听注册：
 *      本应用作为事件的接收方，接收到发送者的相符合的事件则去调用对应函数处理。
 *      发送方以广播的形式发送，无需了解谁要接收该消息，
 *
 * History:
 *  v1.0    huanghanbin@ruijie.com.cn 2023-03-03
 *          创建初稿
 */
#include "ubus.h"
#include "ubus/ubus_debug.h"
#include "common.h"

dbg_md_t g_ubus_id = -1;
extern main_gl_t main_gl;

/**
 * @brief:netconf ubus重连回调定时器
 *
 * @param timeout:定时器
 */
static void ubus_reconnect_timer(struct uloop_timeout *timeout)
{
    int t = 2;  /* 重连时间：2s */
    struct ubus_context* ubus_ctx = main_gl.g_ubus_ctx;

    /* 重连定时器设置 */
    static struct uloop_timeout retry = {
        .cb = ubus_reconnect_timer,
    };

    /* client重新连接server */
    if (ubus_reconnect(ubus_ctx, NULL) != 0) {
        uloop_timeout_set(&retry, t * 1000);
        UBUS_ERROR("failed to reconnect, trying again in %d seconds\n", t);
        return;
    }

    UBUS_LOG("reconnected to ubus success, new id: 0x%08X\n", ubus_ctx->local_id);

    /* 将ubus上下文包含的连接加入epoll描述符集，用于监听 */
    ubus_add_uloop(ubus_ctx);

    return;
}

/**
 * @brief:netconf ubus重连回调函数
 */
static void ubus_connection_lost(struct ubus_context *ctx)
{
    ubus_reconnect_timer(NULL);
    return;
}

/**
 * @brief: 增加对象
 */
int std_ubus_add_object(struct ubus_object *obj)
{
    struct ubus_context* ubus_ctx = main_gl.g_ubus_ctx;
    UBUS_LOG("std_ubus_add_object start\n");

    /* 入参检查 */
    if (obj == NULL) {
        UBUS_ERROR("Failed to get debug ubus object\n");
        return RET_NULL;
    }
    UBUS_LOG("obj->name:%s\n", obj->name);

    /* 添加对象 */
    int ret = ubus_add_object(ubus_ctx, obj);
    if (ret != 0) {
        UBUS_ERROR("Failed to publish object '%s': %s\n", obj->name, ubus_strerror(ret));
    }
    UBUS_LOG("std_ubus_add_object end\n");

    return ret;
}

/**
 * @brief: 删除对象
 */
int std_ubus_remove_object(struct ubus_object *obj)
{
    struct ubus_context* ubus_ctx = main_gl.g_ubus_ctx;
    UBUS_LOG("std_ubus_remove_object start\n");

    /* 入参检查 */
    if (obj == NULL) {
        UBUS_ERROR("Failed to get debug ubus object\n");
        return RET_NULL;
    }
    UBUS_LOG("delete name:%s\n", obj->name);

    /* 删除对象 */
    int ret = ubus_remove_object(ubus_ctx, obj);
    if (ret != 0) {
        UBUS_ERROR("Failed to remove object '%s': %s\n", obj->name, ubus_strerror(ret));
    }
    UBUS_LOG("std_ubus_remove_object end\n");

    return ret;
}

/**
 * @brief:给所有订阅者发送通知
 */
void ubus_send_signal(struct ubus_object *obj, const char *type, struct blob_attr *msg)
{
    int ret = -1;
    struct ubus_context* ubus_ctx = main_gl.g_ubus_ctx;

    /* 给所有订阅者发送通知 */
    ret = ubus_notify(ubus_ctx, obj, type, msg, UBUS_NOTIFY_TIMEOUT);

    if (ret != 0) {
        UBUS_ERROR("Failed to publish object '%s': %s\n", obj->name, ubus_strerror(ret));
    }

    return;
}

/**
 * @brief:发送事件消息
 */
int ctcapd_send_ubus_event(const char *eventName, struct blob_buf *b)
{
    struct ubus_context* ubus_ctx = main_gl.g_ubus_ctx;
    return ubus_send_event(ubus_ctx, eventName, b->head);
}

/**
 * @brief: ubus debug初始化
 */
 static int ubus_debug_init(void)
 {
    g_ubus_id = dbg_module_reg("ubus");
    if (g_ubus_id < 0) {
        UBUS_ERROR("register debug module failed \n");
        return -1;
    }
    UBUS_LOG("dbg_module_reg() ubus success\n");

    return RET_OK;
 }

/**
 * @brief: ubus 初始化函数
 *
 * @description:
 *    用于ubus注册对象
 */
int ubus_init(void)
{
    int ret;
    struct ubus_object* dbg_obj;
    struct ubus_context* ubus_ctx = main_gl.g_ubus_ctx;

    /* debug初始化 */
    ret = ubus_debug_init();
    if (ret != RET_OK) {
        UBUS_ERROR("ubus debug init is error\n");
        return RET_ERROR;
    }

    /* 与ubusd建立连接，创建ubus上下文(包含fd等) */
    ubus_ctx = ubus_connect(NULL);
    if (ubus_ctx == NULL) {
        UBUS_ERROR("Failed to connect to ubus\n");
        return RET_NULL;
    }

    /* ubus断开回调函数 */
    ubus_ctx->connection_lost = ubus_connection_lost;

    /* 向uloop注册fd加入epoll,用于监听 */
    ubus_add_uloop(ubus_ctx);

    /* 添加 debug 对象*/
    std_ubus_add_object(dbg_get_ubus_object());

    return RET_OK;
}
