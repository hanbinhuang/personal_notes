#ifndef __CFG_MONITOR_H__
#define __CFG_MONITOR_H__

#define FILENAME_LEN_MAX        256

typedef struct moni_ctrl_s {
    char file[FILENAME_LEN_MAX];    /* 要监控的文件名 */
    bool status;                    /* 要监控的文件的状态，true：监控，false：不监控 */
    time_t last_st_mtime;           /* 要监控的文件上次修订时间，初始为0 */
    void (*cb)(void);               /* 文件变化时的回调函数 */
} moni_ctrl_t;

int moni_file_init(void);
int moni_file_free(void);
int moni_file_add(moni_ctrl_t *moni_ctrl);

#endif /* __CFG_MONITOR_H__ */

