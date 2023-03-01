

[TOC]



## 1. UPnP

​		UPnP  =  Universal Plug and Play  =  通用即插即用  
​		一套[网络协议](https://baike.baidu.com/item/网络协议/328636)。该协议的目标是使家庭网络（数据共享、通信和娱乐）和公司网络中的各种设备能够相互无缝连接，并简化相关网络的实现。UPnP通过定义和发布基于开放、因特网通讯网协议标准的UPnP设备控制协议来实现这一目标。

## 2. MQTT

​		MQTT =  Message Queuing Telemetry Transport = 消息队列遥测传输 

​		基于[发布](https://baike.baidu.com/item/发布/33814)/[订阅](https://baike.baidu.com/item/订阅/8724574)范式的消息协议。它工作在 TCP/IP协议族上，是为[硬件性能](https://baike.baidu.com/item/硬件性能/12730200)低下的远程设备以及网络状况糟糕的情况下而设计的发布/订阅型消息[协议](https://baike.baidu.com/item/协议/670528)

## 3. CLI

​		CLI = Command-Line Interface  =  命令行接口

## 4. RPC

​		RPC =  Remote Procedure Call Protocol  =  远过程调用协议

​		一种通过网络从远程计算机程序上请求服务，而不需要了解底层网络技术的协议。RPC协议假定某些传输协议的存在，如TCP或UDP，为通信程序之间携带信息数据。在OSI网络通信模型中，RPC跨越了传输层和应用层。RPC使得开发包括网络分布式多程序在内的应用程序更加容易。

​		RPC采用客户机/服务器模式。请求程序就是一个客户机，而服务提供程序就是一个服务器。首先，调用进程发送一个有进程参数的调用信息到服务进程，然后等待应答信息。在服务器端，进程保持睡眠状态直到调用信息的到达为止。当一个调用信息到达，服务器获得进程参数，计算结果，发送答复信息，然后等待下一个调用信息，最后，客户端调用过程接收答复信息，获得进程结果，然后调用执行继续进行。

## 5.SDK

​		SDK = software Development Kit = 软件开发工具包

​		软件开发包，包含Linux系统开发用到的各种系统源码，驱动，工具，应用软件包。