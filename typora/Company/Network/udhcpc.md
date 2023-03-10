udhcpc -p /var/run/udhcpc-br-wan.pid -s /lib/netifd/dhcp.script -f -t 0 -i br-wan -x hostname:X32-F38DFD



udhcpc是一个面向嵌入式系统的非常小的DHCP客户端，字母的缩写微- DHCP -客户端（μDHCPc）。

-p:  创建一个存放pid的文件, Create pidfile

-s:  跑一个dhcp事件的程序,Run PROG at DHCP events (default /usr/share/udhcpc/default.script)

 -f   在前台运行，     Run in foreground

-t N  最多发送N个发现数据包（默认3），     Send up to N discover packets (default 3)

 -i IFACE    Interface to use (default eth0)

 

-x OPT:VAL   Include option OPT in sent packets (cumulative)

​            Examples of string, numeric, and hex byte opts:

​            -x hostname:bbox - option 12

​            -x lease:3600 - option 51 (lease time)

​            -x 0x3d:0100BEEFC0FFEE - option 61 (client id)

 

在发送的数据包中包含选项OPT（累积）字符串、数字和十六进制字节optsx主机名示例：bbox-选项12x租用：3600选项51（租用时间）x 0x3d：0100BEEFCOFFEE-选项61（客户端id）