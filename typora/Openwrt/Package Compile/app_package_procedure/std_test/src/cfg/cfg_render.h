#ifndef __CFG_RENDER_H__
#define __CFG_RENDER_H__

/************************************** 宏定义类型 **************************************/
#define __PROC_POOLS_MAX 10                      /* 进程池中进程数总量 */


/************************************** 对外函数 **************************************/
int config_render_init(void);                    /* 初始化 */
int config_render(char* args[]);

#endif /* __CFG_RENDER_H__ */