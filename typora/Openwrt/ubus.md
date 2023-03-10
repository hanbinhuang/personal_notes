[TOC]



# 1.libubox

## 1.1. 简述

​		一个核心库，封装一系列使用功能。  
​		**（1）功能**：  
​					① 通用基础功能接口，如：链表、平衡二叉树、二进制块处理、md5等。  
​					②  一套基于事件驱动的机制及任务队列管理功能。  
​					③  多种sock接口封装。  
​		**（2）目的**：  
​					以动态库形式提供可复用的通用功能，避免重复造轮。  
​		**（3）组成**：  
​					① libubox库  
​					② jshn库：1> jshn命令工具 2> jshn.sh脚本  
​					③ libblobmsg-json

## 1.2. libubox 头文件

​		**（1）utils.h**：提供简单实用功能，如：  
​						字节序转换、位操作、编译器属性包装、连续的内存分配函数、静态数组大小的宏、断言/错误的实用功能和 base64 编码解码等功能  
​		**（2）blob.h**：提供二进制数据处理功能，如：  
​						提供TLV数据类型  
​		**（3）usock.h**：提供简单的socket对象封装。  
​		**（4）uloop.h**：提供事件驱动机制接口（基于epoll）。

# 2.ubus

## 2.1.  简述 

​		系统总线ubus为Openwrt平台提供一个系统级的进程间通信（IPC）功能，  
​		是Openwrt的RPC工具，是一个通用的框架。  
​		**（1）消息格式：**  
​					①TLV格式（type-length-value）：  
​					②json格式：  
​		**（2）依赖性**：依赖于ubox库。  
​		**（3）组成：**  
​					**①ubusd进程**（精灵进程/守护进程）  
​							作为ubus server端，在系统启动时运行，负责进程间的消息路由和传递（将注册到ubusd进程的消息接收与发送）  
​					**②接口库**：  
​							<1>：**libubus.so**：ubus向外部提供的编程接口，例如创建socket，进行监听和连接，发送消息等接口函数。  
​							<2>：**libubox.so**：ubus向外部提供的编程接口，例如等待和读取消息    
​							<3>：**libblobmsg.so，libjson.so**：提供了封装和解析json数据的接口，编程时不需要直接使用libjson.so，而是使用libblobmsg.so提供的更灵活的接口函数
​	  
​					**③实用工具**：ubus命令行			

​			**(4) 功能：**  
​					① 提供注册对象和方法供其他实体调用。  
​					② 调用其他应用程序提供的注册对象的控制接口。  
​					③ 在特定对象上注册监听事件。  
​					④ 向特定对象发送事件消息。  
​			**(5) 优点：**  
​					① 进程间通信的实现变得简单。  
​					② 具有很强的可移植性。

## 2.2. 使用场景

​		**（1）场景一：“客户端 -- 服务器” 形式的交互**     
​				即：进程A注册一系列的服务，进程B去调用这些服务。   
​               ubus将消息处理成**对象(object)**和**方法(methods)**的概念，  
​					①一个对象可以包含多个方法。  
​					②对象和方法都有自己的名字，发送请求方在消息中指明要调用的对象和方法。   
​		**（2）场景二：“订阅 -- 通知”的方式进行进程通信。**  
​				即：进程A提供**订阅服务(subscriber)**，其他进程可以选择订阅或退订该服务，进程A可以向所有订阅者发送消息。  
​				客户端需要向服务端注册收到特定消息时的处理方法，这样当服务端发生变化是可以通知客户端。  
PS:  
​		①**适合少量，低频的数据传输**，若一次超过60kb则无法正常工作。  
​		②对**多线程支持不**好，若多线程请求同一服务可能出现不可预知的结果。  
​		③**不建议递归ubus**，如：进程A调用进程B服务，而进程B需调用进程C的服务。	  
​		（若不得已如此使用，需再调用过程避免全局变量重用问题）

## 2.3. 实现框架

​	（1）实现基础：unix socket（本地socket）,相较于inet socket更高效，更可靠。  
​	（2）简单的unix socket的服务器与客户端实现过程：  
​				① 建立socket server端，绑定到本地socket文件，并监听clients连接。  
​				② 建立socket client端（一个/多个），连接server端。  
​				③ client与server相互发送消息  
​				④ client/server 收到对象消息后做对应处理  
​	（3） ubus通信的实现过程：  
​				① 提供现成的socket server端：ubusd.（使用者不用自己实现server）  
​				② 提供创建socket client端的三种语言接口 ：shell脚本，lua脚本，C语言  
​				③ 定义通信消息格式必须为：json格式。  
​				④ 抽象消息处理方式：“对象(object)”和“方法(method)”  
​						1.一个对象中包含多种方法。    
​						2.client需向server注册特定的处理方法，请求方只需指定调用对象+方法即可。

## 2.4.接口命令：

​		ubus 显示帮助  
​		**使用方法**：`ubus [<options>] <command> [arguments...]`

### 2.4.1. command

| 命令                                  | 功能描述                                                |
| ------------------------------------- | ------------------------------------------------------- |
| ubus                                  | 显示帮助                                                |
| ubus list  [<path>]                   | 显示所有ubus 对象/显示具体ubus对象                      |
| ubus call <path> <method> [<message>] | 调用对象的指定方法/调用对象的指定方法并传递消息         |
| ubus listen [<path>...]               | 设置监听套接字来接收服务器发出的所有消息/指定对象的消息 |
| ubus send  <type> [<message>]         | 发送通知事件，listen可监听到                            |
| ubus wait_for <object> [<object>...]  | 等待一/多个对象注册到ubus中，注册完成退出               |
| ubus  monitor                         | 监控ubus详细数据                                        |

PS:  
	① **path**：对象              ② **method**：方法  
	③ **message**：消息  	④ **type**：事件。  

### 2.4.2. Options

| 选项         | 功能描述                                           |
| ------------ | -------------------------------------------------- |
| -s <socket>  | 设置要连接到的unix域套接字                         |
| -t <timeout> | 设置命令完成的超时（秒）                           |
| -S           | 使用简化输出（用于脚本）                           |
| -v           | 更详细的输出                                       |
| -m <type>    | （用于监视器）：包括特定的消息类型（可以多次使用） |
| -M <r\|t>    | （用于监视器）：仅捕获接收或发送的流量             |



## 2.5.接口函数：

### 2.5.1. ubus_connect

​		**1.函数：**`struct ubus_context *ubus_connect(const char *path)`    
​		**2.功能：**初始化client端context结构，并连接ubusd

### 2.5.2.ubus_auto_connect

​		**1.函数：**`void ubus_auto_connect(struct ubus_auto_conn *conn)`    
​		**2.功能：** 与`ubus_connect()`函数基本功能相同，但此函数在连接断开后会自动进行重连

### 2.5.3.ubus_register_event_handler

​		**1.函数：**`  int ubus_register_event_handler(struct ubus_context *ctx, struct ubus_event_handler *ev, const char *pattern) `   
​		**2.功能：**注册新事件

### 2.5.4.ubus_send_event

​		**1.函数：**`int ubus_send_event(struct ubus_context *ctx,  const char *id, struct blob_attr *data) `   
​		**2.功能：**发出事件消息

### 2.5.5.ubus_lookup

​		**1.函数：**`int ubus_lookup(struct ubus_context *ctx,  const char *path,ubus_lookup_handler_t cb, void *priv) `   		**2.功能：**向ubusd查询指定`UBUS_ATTR_OBJPATH`对应对象信息内容,内容通过输入回调函数`ubus_lookup_handler_t`由调用者自行处理

### 2.5.6.ubus_lookup_id

​		**1.函数：**`int ubus_lookup_id( struct ubus_context *ctx,  const char *path, uint32_t *id)  `  
​		**2.功能：**向ubusd查询指定`UBUS_ATTR_OBJPATH`对应的ID号

### 2.5.7.ubus_add_object

​		**1.函数：** `int ubus_add_object(struct ubus_context *ctx, struct ubus_object *obj) `  
​		**2.功能：** client端向ubusd server请求增加一个新object

### 2.5.8.ubus_remove_object

​		**1.函数：**`int ubus_remove_object(struct ubus_context *ctx, struct ubus_object *obj)  `  
​		**2.功能：** client端向ubusd server请求删除一个object

### 2.5.9.ubus_process_obj_msg

​		**1.函数：**`void __hidden ubus_process_obj_msg(struct ubus_context *ctx, struct ubus_msghdr *hdr)  `  
​		**2.功能：**处理收到与object相关报文

### 2.5.10.ubus_process_invoke

​		**1.函数：**`static void ubus_process_invoke(struct ubus_context *ctx, struct ubus_msghdr *hdr, struct ubus_object *obj,  struct blob_attr **attrbuf) `   
​		**2.功能：** 处理`UBUS_MSG_INVOKE`报文

### 2.5.11.ubus_process_unsubscribe

​		**1.函数：**`static void ubus_process_unsubscribe(struct ubus_context *ctx,   struct ubus_msghdr *hdr,struct ubus_object *obj,  struct blob_attr **attrbuf) `   
​		**2.功能：**处理`UBUS_MSG_UNSUBSCRIBE`报文

### 2.5.12.ubus_process_notify

​		**1.函数：**`static void ubus_process_notify(struct ubus_context *ctx,  struct ubus_msghdr *hdr, struct ubus_object *obj,  struct blob_attr **attrbuf)`    
​		**2.功能：**处理`UBUS_MSG_NOTIFY`报文

### 2.5.13.ubus_parse_msg

​		**1.函数：**` __hidden struct blob_attr **ubus_parse_msg(struct blob_attr *msg) `  
​		**2.功能：** 把libubus关注的无序报文转化为有序的blob_attr数组

### 2.5.14.ubus_send_msg

​		**1.函数：**`int __hidden ubus_send_msg( struct ubus_context *ctx,  uint32_t seq,struct blob_attr *msg, int cmd, uint32_t peer,  int fd)`    
​						① ctx  - client上下文对象，  
​						② seq  - 报文顺序号 hdr.seq  ，  
​						③ msg  - 报文内容，  
​						④ cmd  - 报文类型 hdr.type，  
​						⑤ perr  - ，  
​						⑥  fd   - 需传递给对端的描述符，等于-1表示不需传递  
​		**2.功能：**发送报文

### 2.5.15.ubus_handle_data

​		**1.函数：** ` void __hidden ubus_handle_data(struct uloop_fd *u, unsigned int events) `     
​		**2.功能：**client端fd收包处理函数

### 2.5.16.ubus_poll_data

​		**1.函数：**`void __hidden ubus_poll_data(struct ubus_context *ctx, int timeout) `   
​		**2.功能：**client端轮询fd收包处理函数

### 2.5.17.ubus_reconnect

​		**1.函数：**` int ubus_reconnect(struct ubus_context *ctx, const char *path) `  
​		**2.功能：**client连接ubusd server

### 2.5.18.ubus_register_subscriber

​		**1.函数：** `int ubus_register_subscriber(struct ubus_context *ctx, struct ubus_subscriber *s) `  
​		**2.功能：**

### 2.5.19.ubus_subscribe

​		**1.函数：**`int ubus_subscribe(struct ubus_context *ctx, struct ubus_subscriber *obj, uint32_t id)  `  
​		**2.功能：**

### 2.5.20.ubus_unsubscribe

​		**1.函数：**`int ubus_unsubscribe(struct ubus_context *ctx, struct ubus_subscriber *obj, uint32_t id) `   
​		**2.功能：**

### 2.5.21.ubus_send_reply

​		**1.函数：**`int ubus_send_reply(struct ubus_context *ctx, struct ubus_request_data *req,struct blob_attr *msg) `   		**2.功能：**发送回应信息，消息类型UBUS_MSG_DATA

### 2.5.22.ubus_invoke_async

​		**1.函数：**` int ubus_invoke_async(struct ubus_context *ctx, uint32_t obj, const char *method,struct blob_attr *msg, struct ubus_request *req)`   
​		**2.功能：**异步通知指定object执行其方法

### 2.5.23.ubus_invoke

​		**1.函数：**`int ubus_invoke(struct ubus_context *ctx,  uint32_t obj,   const char *method,struct blob_attr *msg,   ubus_data_handler_t cb,   void *priv, int timeout)  `  
​		**2.功能：**同步通知指定object执行其方法

### 2.5.24.ubus_notify_async

​		**1.函数：**`int ubus_notify_async(struct ubus_context *ctx, struct ubus_object *obj,const char *type,   struct blob_attr *msg, struct ubus_notify_request *req) `   
​		**2.功能：**异步发出通知消息

### 2.5.25.ubus_notify

​		**1.函数：** `int ubus_notify(struct ubus_context *ctx,  struct ubus_object *obj, const char *type,  struct blob_attr *msg,  int timeout)`   
​		**2.功能：**同步发出通知消息

### 2.5.26.

​		**1.函数：**    
​		**2.功能：**

### 2.5.27.

​		**1.函数：**    
​		**2.功能：**

### 2.5.28.

​		**1.函数：**    
​		**2.功能：**

### 2.5.29.

​		**1.函数：**    
​		**2.功能：**

### 2.5.30.

​		**1.函数：**    
​		**2.功能：**

| 函数                        | 含 义                                                        |
| --------------------------- | ------------------------------------------------------------ |
| ubus_add_objec              | 将对象加入的ubus空间中，即客户端可以访问对象                 |
| ubus_register_subscriber    | 注册订阅通知                                                 |
| ubus_connect                | 连接指定的路径，创建并返回路径所代表的ubus上下文             |
| ubus_send_reply             | 执行完成方法调用后、发送响应                                 |
| ubus_notify                 | 给对象所有的订阅者发送通知                                   |
| ubus_lookup                 | 查找对象，参数path为对象的路径,如果为空则查找所有的对象。cb为回调函数，对查找结果进行处理 |
| ubus_lookup_id              | 查找对象的id，并将id参数在指针中返回                         |
| ubus_invoke                 | 调用对象的方法                                               |
| ubus_register_event_handler | 注册事件处理句柄                                             |
| ubus_send_event             | 发送事件消息                                                 |





# 3.举例





























