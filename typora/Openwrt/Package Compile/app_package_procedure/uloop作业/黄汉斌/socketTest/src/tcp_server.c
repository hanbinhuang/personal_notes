#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <libubox/usock.h>
#include <libubox/uloop.h>

static char *strhello = "hello!\n";

void server_cb(struct uloop_fd *fd, unsigned int events);
void sevent_cb(struct uloop_fd *fd, unsigned int events);

/* 服务器的uloop文件描述符变量 */
struct uloop_fd server_fd = {
    .cb = server_cb, /* 回调函数 */
};

/* 客户端的uloop文件描述符变量 */
struct uloop_fd ucli_fd = {
    .cb = sevent_cb, /* 回调函数 */
};

/**
 * @brief        ：客户端文件描述符事件回调函数
 *  
 * @param fd     ：uloop文件描述符
 * @param events ：事件类型
 */
void sevent_cb(struct uloop_fd *fd, unsigned int events)
{
    char buf[1024];
    int ret;
    if (events & ULOOP_READ)
    {
        /* 接收到客户端发送的消息并输出到屏幕上 */
        printf("ULOOP_READ is triggered! sock fd=%d,  events=%d\n", fd->fd, events);
        ret = read(fd->fd, buf, sizeof(buf));
        buf[ret] = 0;
        printf("Client MSG: %s \n", buf);

        if (events & ULOOP_WRITE)
        {
            printf("ULOOP_WRITE is triggered! sock fd=%d,  events=%d\n", fd->fd, events);
            write(fd->fd, strhello, strlen(strhello));
        }
    }
}

/**
 * @brief        :服务器文件描述符事件回调函数
 *
 * @param fd     :uloop文件描述符
 * @param events :事件类型
 */
void server_cb(struct uloop_fd *fd, unsigned int events)
{
    printf("server_cb is triggered! events=%d\n", events);

    /* 监听客户端 */
    int client_fd = accept(fd->fd, NULL, 0);
    if (client_fd < 0)
    {
        perror("server_cb accept");
        return;
    }

    printf(" >>>>>>> New client accepted! >>>>>>\n");
    write(client_fd, strhello, strlen(strhello));

    /* 注册一个新描述符到客户端文件描述的事件处理循环 */
    printf("Add client to uloop_fd...\n");
    ucli_fd.fd = client_fd;
    uloop_fd_add(&ucli_fd, ULOOP_READ | ULOOP_WRITE | ULOOP_EDGE_TRIGGER);
}

/**
 * @brief 主函数
 */
void main(int argc, char **argv)
{
    int sfd = -1;                                                           /* 服务器的文件描述符(usockID) */
    const char *host_addr = "127.0.0.1";                                    /* 主机地址 */
    const char *port = "8000";                                              /* 端口号 */
    int type = USOCK_TCP | USOCK_SERVER | USOCK_NOCLOEXEC | USOCK_IPV4ONLY; /* usock类型 */

    /* 初始化事件循环 */
    uloop_init();

    /* 创建服务器的文件描述符,失败则直接报错返回-1 */
    sfd = usock(type, host_addr, port);
    if (sfd < 0)
    {
        perror("usock");
        exit(-1);
    }
    printf("Succeed to create server at %s:%s\n", host_addr, port);

    /* 注册一个新描述符到事件处理循环 */
    server_fd.fd = sfd;
    uloop_fd_add(&server_fd, ULOOP_READ | ULOOP_EDGE_TRIGGER); /* 事件类型为只读和边缘触发 */

    /* 事件循环主处理入口 */
    uloop_run();

    /* 销毁事件循环 */
    uloop_done();
    close(sfd);
}