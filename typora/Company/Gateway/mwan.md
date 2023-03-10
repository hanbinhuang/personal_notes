





Wifi6:

**network:**  
		工程路径：`packages18_06/network/services/netifd/files/etc/init.d/netwrok`  
		设备路径：`/etc/init.d/netwrok`  
		内容：start与reload接口中会调用`/etc/network.start.user `

**network.start.user:**   
		工程路径：`devmng/netmng/network/files/etc/network.start.user`  
		设备路径：`/etc/network.start.user`  
		内容：若`/usr/sbin/fc_restart`文件存在则调用

**fc_restart:**  
		工程路径：`gateway/gw_pub/src/fc_restart`  
		设备路径：`/usr/sbin/fc_restart`  
		内容：







gateway/mwan/files/etc/hotplug.d/iface

调用到`check_mwan_state`脚本的应用程序有：  
		①**gateway/mwan/files/etc/hotplug.d/iface/15-mwan3**  
		②**gateway/mwan/files/usr/sbin/mwan3**











mwan



查看mwan功能是否开启：

​	uci get mwan3.@interface[0].enabled

打开mwan功能：

dev_config set -m mllb "{\"wan\":\"1\",\"wan1\":\"1\",\"weight_list\":[{\"w\":\"1\",\"ifname\":\"wan\"},{\"w\":\"1\",\"ifname\":\"wan1\"}],\"intf_cnt\":\"2\",\"policy\":\"session\",\"enable\":\"1\",\"mode\":\"weight\"}"

关闭mwan功能：

dev_config set -m mllb "{\"wan\":\"1\",\"wan1\":\"1\",\"weight_list\":[{\"w\":\"1\",\"ifname\":\"wan\"},{\"w\":\"1\",\"ifname\":\"wan1\"}],\"intf_cnt\":\"2\",\"policy\":\"session\",\"enable\":\"0\",\"mode\":\"weight\"}"



 

双WAN的场景：

1.策略路由：

①部分内网用户走WAN1，而不是负载均衡的走WAN和WAN1。

②端口80的走TCP报文走WAN口，端口8080的TCP报文走WAN1.

③双WAN主备模式，另一条线路做备份，主路出问题自动切到备份线路，而不是负载均衡。

2.静态路由：基于目标网络进行路由。

 

模式：

1.负载均衡模式（默认）

2.主备模式（正常走主路，出问题自动切备份路线）。

3.策略模式（配置参数）

①源IP地址，②目的IP地址，③出接口

**④协议号**：IP/TCP/UDP/等协议

⑤源端口（TCP/UDP必选）

⑥目的端口（TCP/UDP必选）

 

策略路由的实现：

使用linux平台支持的ip rule命令进行配置。

该指令默认支持，①②③。

④、⑤、⑥通过ip rule支持对fwmark的条件，采用添加iptables规则，

对满足策略的报文打上mark，实现策略路由。









