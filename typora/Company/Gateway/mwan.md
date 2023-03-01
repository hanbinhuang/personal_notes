





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













