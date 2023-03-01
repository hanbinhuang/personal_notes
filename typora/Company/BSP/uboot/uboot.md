

# 1. uboot 概念



# 2. uboot启动

## 2.1. uboot启动简述

​		设备上电或复位，不会马上进入SDRAM执行程序，而是先从flash中读取前4k代码复制到SDRAM中（由CPU自动完成），并将SRAM映射到0x0的地址，也是0x0地址开始执行。  
​		前4K程序负责将整个uboot拷贝到SDRAM，然后才跳到SDRAM开始真正启动Uboot。

PS:  
		flash种类：①nandflash ②norflash  
		SDRAM = 内存

## 2.2. uboot代码