

# 1. uboot 概念

​		U-Boot = Universal Boot Loader =  
​		**uboot的入口就是开机自动启动，uboot的唯一出口就是启动内核**

## 1.1. 目录结构

|   目录   |                  描述                  |
| :------: | :------------------------------------: |
|   api    |           uboot中的接口函数            |
|   arch   |    uboot中有关处理器架构相关的代码     |
|  board   |         为开发板定制的相关代码         |
|  common  |      通用代码，大部分与命令行有关      |
|   disk   |            磁盘分区相关代码            |
|   doc    |         有关README.txt相关代码         |
| drivers  |           与驱动程序相关代码           |
| examples |                示例程序                |
|    fs    | 文件系统，适合大部分开发板上的文件系统 |
| include  |            包含全局的头文件            |
|   lib    |               通用库文件               |
|   net    |      网络相关的代码，小型的协议栈      |
|   post   |    Power On Self Test，上电自检程序    |
|  tools   | 辅助程序，用于编译和检查uboot目标文件  |

1





# 2. uboot启动

## 2.1. uboot启动简述

​		设备上电或复位，不会马上进入内存执行程序，而是先从flash中读取前4k代码复制到内存中（由CPU自动完成），并将内存映射到0x0的地址，也是0x0地址开始执行。  
​		前4K程序负责将整个uboot拷贝到内存，然后才跳到内存开始真正启动Uboot。

PS:  
		flash种类：①nandflash ②norflash  
		内存种类：①SDRAM②DDR3等

## 2.2. 启动步骤

### 2.2.1. Stage 1

​		本阶段以汇编语言为主，主要功能在start.S代码中实现。

1）设置CPU；  
2）关闭Watchdog；  
3）设置Clock；  
4）屏蔽所有的Interrupt；  
5）初始化SDRAM；  
6）初始化nandflash（如果u-boot代码在nandflash上）；
    
7）重定位BootLoader的代码到SDRAM；     
8）设置栈（C语言函数调用需要先设置栈，栈要在SDRAM初始化之后设置）；  
9）清BSS段；
    
10）进入BootLoader的Step-2阶段启动内核；  

### 2.2.2. Stage 2  

​		本阶段以C语言为主，函数入口为：start_armboot

11）设置BootLoader要传递给内核的参数；  
12）从Nor 或 Nand flash 上复制内核代码到内存的0x30007FC0的地方  
（这个地址不是固定的，可以任意指定，只要不破坏u-boot使用的内存空间，u-boot会根据uImage的Header中的Load Addr判断内核是否装载到Load Addr的地址处，Load Addr在u-boot中默认为0x30007FC0，如果不在，u-boot会移动内核代码到改地址处）；
    
13）跳转到uImage的Header中Entry Point指定的入口地址处开始执行内核代码（内核执行后，u-boot不在执行）





## uboot代码