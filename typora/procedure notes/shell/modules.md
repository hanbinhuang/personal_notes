

```shell
# 日志模块，已做适配无需修改
LOG_DIR="/tmp/sysinfo"
LOG_NAME="sysinfo.init.log"
LOG_FILE="$LOG_DIR/$LOG_NAME"
LOG_MAX_LINE_NUM=50

log() {
  if [ -f ${LOG_FILE} ]; then
     log_line=$(cat ${LOG_FILE} | wc -l)
     [ ${log_line} -ge $LOG_MAX_LINE_NUM ] && { mv -f $LOG_FILE ${LOG_FILE}.old; }
  else
     if [ ! -d ${LOG_DIR} ]; then
        mkdir $LOG_DIR
     fi
  fi
  local cur_pid="[$$]"
  local cur_time=$(date "+[%Y-%m-%d %H:%M:%S]")
  echo -e "${cur_pid}${cur_time} ${1}" >> $LOG_FILE
}
log "Enter /etc/init.d/sysinfo"
```





```shell
TTASK_USER_DIR="/usr/local/ttask"
[ -d $TTASK_USER_DIR ] && {
    for script in $(ls $TTASK_USER_DIR/*); do (
        [ -f $script ] && . $script
        sleep 1
    ); done
}

```



[下面语法相关说明](https://blog.51cto.com/u_15707676/5470235)

```shell
: << COMMENTBLOCK
  shell脚本代码段
COMMENTBLOCK
```

