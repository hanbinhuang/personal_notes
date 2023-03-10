[TOC]

# 1.初始化脚本系统

​		Openwrt所有的初始化脚本位于：`/etc/init.d/`目录。  
​		系统启动脚本是/etc/init.d/rcS. 启动顺序：“S+数字”

## 1.1. 组成

​		①`#!/bin/sh /etc/rc.common`：作为脚本解释器并在脚本执行前调用main和检查脚本。  
​				1209及之前的版本中并不支持procd启动，在1407版本中增加了专门针对procd的启动，该脚本向前兼容

​		② 变量

​		③函数

## 1.2. 变量

| 变量              | 功能                                                         |
| ----------------- | ------------------------------------------------------------ |
| SERVICE_USE_PID=1 | 创建pid文件                                                  |
| USE_PROCD=1       | 将进程添加到procd的监控进程中去<br />对start、stop 和 reload函数进行重新定义，在调用这些函数时，将调用start_service、stop_service和 reload_service函数等 |
|                   |                                                              |
|                   |                                                              |
|                   |                                                              |
|                   |                                                              |
|                   |                                                              |
|                   |                                                              |



## 1.3. 函数

| 函数    | 功能                                                |
| ------- | --------------------------------------------------- |
| start   | 系统启动或用户启动时会拉起对应进程时会调用start函数 |
| stop    |                                                     |
| enable  | 创建/etc/rc.d/脚本下的链接                          |
| disable | 删除/etc/rc.d/脚本下的链接                          |



## 1.4. 启机命令

​		启机脚本中，默认restart 就是包含 start 和stop

​		若未定义restart，则默认先调用start 再调用stop 

​		若未定义boot,则默认调用star



# 2. proc

## 2.1. 概念：

​		procd = process management daemon = 进程管理守护进程  
**（1）作用：**  
​		监控进程的状态，如：某些系统进程异常退出则会再次启动这些进程。  
**（2）实现：**   
​		通过init脚本将进程信息加入procd数据库中管理进程启动，通过ubus总线通讯调用，可防止进程重复启动。  
**（3）组成：**  
​		①**reload_config:**检查配置文件是否变化，若有变化则通知proc进程。  
​		**工作原理：**  
​			a. 系统启动时会执行`reload_config`将注册程序的配置文件生成MD5值保存在`/var/run/config.md5`文件中。  
​			b. 若配置文件不同则通过ubus总线向procd通知配置文件发生改变（空行和注释不引起改变），则会调用reload函数重新读取配置。  
​			c. 将运行中的配置文件MD5值保存在`/var/run/config.md5`文件中					   
​		②**procd守护进程**：接收使用者请求进行**管理进程**（增删进程）和 **监控进程状态**（进程异常退出则会再次启动）  
​		③**procd.sh**：提供函数封装procd提供系统总线方法，调用者可便利使用procd提供的方法。



| 函数             | 功能                                                         |
| ---------------- | ------------------------------------------------------------ |
| start_service    | 注册服务到procd中。<br />若程序没有配置文件，只要实现start_service()就好， <br />procd_set_param设置设置好多参数，command为应用路径， respawn可检测应用若挂掉可重启，也可设置重启间隔，其它参数可以自己查阅 |
| stop_service     | procd kill自己的应用程序后调用的，若果你的应用程序关掉后，需要一些清理工作，需要实现这个 |
| service_triggers | 应用需关联一个配置文件test（需放在/etc/config/目录下）<br />可跟踪文件的修改情况，若该文件有改变，可调用reload_service().<br />也可添加跟踪网络的修改或同时跟踪多个配置文件。 |
| reload_service   | 配置文件改变后，需要调用这个函数，可以根据自己需要实现功能   |



## 2.2.函数方法：

​		函数名：一般以"procd_"开头，除特殊外，如：uci_validate_section函数  

| 函数                               | 功能                                                         |
| ---------------------------------- | ------------------------------------------------------------ |
| procd_open_ trigger                | 创建一个触发器数组                                           |
| procd_add_config_trigger           |                                                              |
| procd_close_trigger                | 结束触发器数组的增加                                         |
| procd_add_reload_trigger           | 增加配置文件触发器                                           |
| procd_open_instance                | 开始增加一个服务实例                                         |
| procd_set_param                    | 设置服务实例的参数值。（参数类型如下：）<br />**command**：服务的启动命令行。<br />**respawn**：进程意外退出的重启机制及策略，一般为3个设置值，不写为默认值。<br />1.判断异常失败边界值（threshold）：默认3600s,若小于该时间退出则会累加重启次数，大于该时间则重启次数置为0<br />2.重启延时时间（多少秒重启进程），默认5s。<br />3.总失败重启次数（有永久退出的重启次数，超过该值则不会重启），默认5次。<br />**env**:进程的环境变量。<br />**file**：配置文件名，比较其文件内容是否改变<br />**netdev**：绑定的网络设备（探测 ifindex 更改）。<br />**limits**：进程资源限制 |
| procd_close_instance               | 完成进程实例的增加。                                         |
| procd_open_validate                | 打开一个验证数组                                             |
| procd_close_validate               | 关闭一个验证数组                                             |
| procd_open_service(name, [script]) | 用于创建一个新的 procd 进程服务消息（仅在 rc.common 中调用）<br />至少需要一个参数，第一个参数是实例名称， 第二个参数是可选参数为启动脚本。 |
| procd_close_service                | 完成进程管理服务的增加（仅在 rc.common 中调用）              |
| procd_kill                         | 杀掉服务实例（或所有的服务实例）。<br />至少需要一个参数，第一个参 数是服务名称，通常为进程名，第二个是可选参数，是进程实例名称，因为可能有多个进程.<br />示例，如果不指定所有的实例将被关闭。该函数在 rc.common 中调用，用户从命令行调 用 stop 函数时会使用该函数杀掉进程。 |
| uci_validate_section               | 调用 validate_data 命令注册为验证服务。在配置发生改变 后对配置文件的配置项合法性进行校验。验证服务是在进程启动时通过 ubus 总线注册到 procd 进程中 |
| ubus call service validate         | 看到系统所有注册的验证服务                                   |

