[TOC]

# 0.注意事项：

​		若想强制升级某产品软件包则需升级前修改ProductID并修改代码，如:  
​				fw_setmac ProductID xxx  
​				vim /sbin/sysupgrade    
​				#sysupgrade_image_check="platform_check_image"

# 1. 升级固件包

## 方法一：Web升级

​		1.连接网口  
​		2.打开Web到本地升级页面并选择对应的软件包  
​		3.选择XXX.tar.gz文件上传并确认

## 方法二：TFTP升级

​		1.笔记本需打开防火墙并配置对应网卡的IP地址为192.168.1.2(默认)  
​		2.连接网口（WAN口[靠近电源]连接笔记本）  
​		3.打开TFTP软件并配置文件路径（升级包路径）  
​		4.打开SecureCRT选择串口，上电输出跳出Uboot boot menu 时快速移动，选择第二个，upgrade fireware    
​		5.设置设备IP地址：192.168.1.1  
​		6.设置笔记本IP地址：192.168.1.2  
​		7.更改笔记本IP地址为192.168.1.2   255.255.255.0  
​		8.输入需要升级的固件包文件:XXX.bin  
​		9.TFTP软件选择固件包文件的位置

## 方法三：后台升级

​		1.将文件通过SCP/hfs.new上传到设备的/tmp目录。  
​				tmp目录是内存，其他目录均为flash，内存一般为128M，flash常为8M或16M，flash较小且可能装不上，而tmp是内存较大，且掉电不保存。  
​		2.使用指令，sysupgrade -n 绝对路径/xxx.tar.gz（也可省略1直接sysupgrade -n + "地址"）  
​				(实际上web升级也是调用该条命令，-n 表示不保留配置升级，还有其他选项可查看代码)

# 2. 升级ROM

​		