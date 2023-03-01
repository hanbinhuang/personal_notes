#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <libubox/usock.h>
#include <libubox/uloop.h>

int cfd = -1; /* 客户端的文件描述符(usockID) */
const char *strhello = "Hello";

void ctime_cb(struct uloop_timeout *t);
struct uloop_timeout client_tm = {
    .cb = ctime_cb,
};

void cprocess_cb(struct uloop_process *c, int ret);
struct uloop_process client_ps = {
    .cb = cprocess_cb,
    .pending = false,
};

/**
 * @brief 进程事件回调函数
 */
void cprocess_cb(struct uloop_process *c, int ret)
{
}

/**
 * @brief 定时器回调函数
 *
 * @param t
 */
void ctime_cb(struct uloop_timeout *t)
{
    char buf[1024];
    pid_t pid;
    /* 客户端统计发送几次 */
    static int send_count = 0;
    printf("\nsend times: %d\n", ++send_count);

    /* 发送hello给服务器 */
    send(cfd, strhello, strlen(strhello), 0);

    /* 接收服务器的信息 */
    if (recv(cfd, buf, 1024, 0) > 0)
    {
        printf("\nrecv_buf: %s\n", buf);

        pid = fork();
        if (pid == 0)
        {
            int ret = 0;
            ret = execl("/usr/sbin/hello.sh", NULL);
            if (ret < 0)
            {
                printf("exec failure!");
            }
            exit(0);
        }
        else
        {
            client_ps.pid = pid;
            if (uloop_process_add(&client_ps) < 0)
            {
                printf("uloop_process_add fail !!\n");
            }
            printf("success create process pid: %d\n", pid);
        }
    }

    /* 再次添加定时器，实现循环定时 */
    uloop_timeout_set(&client_tm, 5000);
}

/**
 * @brief 主函数
 */
void main(int argc, char **argv)
{
    const char *host_addr = "127.0.0.1"; /* 主机地址 */
    const char *port = "8000";           /* 端口号 */
    char buf[1024];
    int ret;

    /* 初始化事件循环 */
    uloop_init();

    /* 创建服务器的文件描述符,失败则直接报错返回-1 */
    cfd = usock(USOCK_TCP, host_addr, port);
    if (cfd < 0)
    {
        perror("usock");
        exit(-1);
    }
    printf("Connect to server %s:%s successfuly!\n", host_addr, port);

    ret = read(cfd, buf, sizeof(buf));
    buf[ret] = 0;
    printf("receive msg: %s\n", buf);

    /* 设置定时器，第一次3s往后5s发送hello */
    uloop_timeout_set(&client_tm, 3000);

    /* 事件循环主处理入口 */
    printf("start uloop_run() ...\n");
    uloop_run();

    /* 销毁事件循环 */
    uloop_done();
    close(cfd);
}