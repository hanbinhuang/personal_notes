[pppoe](https://blog.csdn.net/qq_43608147/article/details/127442859)

[pppoeeeee](https://blog.csdn.net/phunxm/article/details/9384123?spm=1001.2101.3001.6650.2&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-2-9384123-blog-124988104.pc_relevant_3mothn_strategy_and_data_recovery&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-2-9384123-blog-124988104.pc_relevant_3mothn_strategy_and_data_recovery&utm_relevant_index=5)

# 1. PPPoE概念

​		PPPoE = Point-to-Point Protocol Over Ethernet = 基于以太网的点对点通讯协议

​		**角色**：  
​				1.PPPoE Client：Host,一般为家用路由器   
​				2.PPPoE Server：AC，一般为运营商路由器



# 2.PPPoE过程

## 2.1. 发现阶段（discovery阶段）

​		该阶段用来建立Host(Client)与AC(Server)连接，接入服务器确定建立的PPP会话标识符(Session ID)，同时获得的对方点到点的连接信息（通过pppoedd）  
​		通过发现阶段识别PPPoE Server的以太网MAC地址，并建立一个PPPoE会话标识（Session ID）.

### 2.1.1. PADI

​		PADI = PPPoE Active Discovery Initiation.  

​		<font color="red">**描述：**</font>**用户主机（PPPoE  Client）**以**广播**的方式发送 PADI数报包，**请求**建立链路。   
​					Code域置为0x09，会话ID域必须置为0x0000。  
​					PADI数据包含有客户端请求的服务。

​		<font color="red">**举例：**</font>

​				**Source:** `RuijieNe_d7:b0:9f (c4:70:ab:d7:b0:9f)`  [Host]  
​                **Destination**: `Broadcast (ff:ff:ff:ff:ff:ff)`

![image-20230118141324298](C:\Users\acer\AppData\Roaming\Typora\typora-user-images\image-20230118141324298.png)

​				(带Service-Name)

​			

### 2.1.2. PADO

​		PADO = PPPoE Active Discovery Offer

​		<font color="red">**描述：**</font>**访问集中器/AC（PPPoE  Server）**收到PADI包后并判断是否可提供服务。    
​							若可则以**单播**的方式发送一个PADO数据包对主机的请求做出**应答**。   
​							若不可则不提供响应报文。   
​					目的地址为主机的MAC地址，Code域置为0x07，会话ID域必须置为0x0000。  
​					PADO数据包必须包含一个类型为AC-Name的Tag（包含了访问集中器的名字）可能包含一/多个服务名。

​		<font color="red">**举例：**</font>

​				**Source**: `30:0d:29:a1:c6:12 (30:0d:29:a1:c6:12)`[AC]  
​				**Destination**: `RuijieNe_d7:b0:9f (c4:70:ab:d7:b0:9f)`[Host]

![image-20230118141650974](C:\Users\acer\AppData\Roaming\Typora\typora-user-images\image-20230118141650974.png)



​			

### 2.1.3. PADR

​		PADR = PPPoE Active Discovery Reques

​		<font color="red">**描述：**</font>因为 PADI数据包是广播方式发送，所以主机可能收到不止一个的PADO报文。主机在收到报文后，会根据AC-Name或PADO所提供的服务来选择一个AC，然后**用户主机（Client）**向选中的AC**单播**一个PADR数据包做出**应答**。   
​					目的地址域为AC的MAC地址，Code域置为0x19，会话ID域必须置为0x0000。  
​					PADR报文必须且只能包含一个Tag_Type为Service-Name的Tag，表明主机请求的服务。  
​					当一个用户主机在确定时间没有收到PADO,它会从发一个PADI,同时等待两倍的时间，可能需要重复多次。

​		<font color="red">**举例：**</font>

​					**Source**: `RuijieNe_d7:b0:9f (c4:70:ab:d7:b0:9f)`[Host]  
​                    **Destination**: `30:0d:29:a1:c6:12 (30:0d:29:a1:c6:12)` [AC]   

![image-20230118143527702](C:\Users\acer\AppData\Roaming\Typora\typora-user-images\image-20230118143527702.png)

### 2.1.4. PADS

​		PADS = PPPoE Active Discovery Session-confirmation

​		<font color="red">**描述：**</font>**访问集中器（ AC/Server）**在收到PADR报文时，就准备开始一个PPP的会话了。

​					为PPPoE会话**创建一个唯一的会话ID**并用**单播**一个PADS数据包来给主机做出**响应**。  
​					目的地址域为主机的MAC地址，Code域置为0x65，会话ID必须设置为所创建好的会话ID。

​		<font color="red">**举例：**</font>

 		**Source**: `30:0d:29:a1:c6:12 (30:0d:29:a1:c6:12)`[AC]  
 		 **Destination**: `RuijieNe_d7:b0:9f (c4:70:ab:d7:b0:9f)`[Host]  




## 2.2. 会话阶段（Session阶段）

​		该阶段Host(Client)与AC(Server)进行标准的PPP协商，协商后数据通过PPP封装发送。

### 2.2.1. 





















# 1.pppoeCheck逻辑

g_pppoe_cfg     = "/tmp/debug/rp-pppoe/pppoe_cfg_" + “接口名”

g_pppoe_path     = "/tmp/debug/rp-pppoe"

g_check_file_tmp   = "/tmp/debug/rp-pppoe/pppoe_check_C"+ “接口名” + log

g_check_file     = "/tmp/debug/rp-pppoe/pppoe_check_C"+ “接口名”



## 1.1. get接口

举例：

```lua
--[[
 dev_sta get -m pppoeCheck '{"intf_name":"wan"}'  |jq .   
module_get 返回格式：
  {
  "code": [
    "1",
    "2"
  ],
  "msg": [
    "查找到pppoe服务器",
    "账号认证失败，请检查账号名称与密码",
  ],
  "res": "failed/success/checking/none"
}
]]--
```



步骤一：打开两个文件用来读取设备信息：  
		1."/tmp/debug/rp-pppoe/pppoe_cfg_" + “接口名” （g_pppoe_cfg 文件）  
		2."/tmp/debug/rp-pppoe/pppoe_check_C"+ “接口名” + “.log”==》重命名为无log（g_check_file）

（有进程会往这两个文件中写数据）

步骤二：读取"/tmp/debug/rp-pppoe/pppoe_check_C"+ “接口名”（g_check_file）文件内容，文件内容中反应的是检测的结果，根据读取到的结果配置result的值

步骤三：若result值为success或fail，则删除pppd进程移移除g_check_file文件

步骤四，根据reullt结果给前端



## 1.2. set接口

举例：

```lua
--[[
  dev_sta set  -m rp-pppoe-check '{"intf_name":"wan","password":"111","proto":"pppoe","mtu":"1492","metric":"0","macaddr":"00:74:9c:d8:91:fb","username":"111","special_line":"0"}'
返回值
  {
	  "code": "0/1/2",  0：成功；1：已有账号在检测中；2：参数错误
	  "msg": "成功/已有账号在检测中/参数错误"  
  }
]]
```





"/tmp/debug/rp-pppoe/pppoe_cfg_" + “接口名”（g_check_file_tmp） 

步骤一：打开g_pppoe_cfg 配置文件

步骤二：检测入参的相关配置

步骤三：执行“ifconfig |grep pppoe-"..data["intf_name"]”输出该端口是否连接

步骤四：执行"`ps |grep /usr/sbin/pppd |grep -v grep|awk '{print $1}'| xargs kill >/dev/null 2>&1`"= cmd_down将该结果写入g_pppoe_cfg 配置文件

步骤五：

cmd_teardown = ` sh /lib/netifd/proto/ppp.sh pppoe teardown "..data["intf_name"].." '{\"ifname\":\"br-"..data["intf_name"].."\",\"ifname\":\""..data["ifname"].."\",\"proto\":\"pppoe\",\"metric\":"..data["metric"]..",\"username\":\""..data["username"].."\",\"password\":\""..data["password"].."\",\"ppp_mtu\":"..data["mtu"].."}' br-"..data["intf_name"].." >/dev/null 2>&1`

cmd_up = `/usr/sbin/pppd nodetach ipparam "..data["intf_name"].." ifname pppoe-"..data["intf_name"].." +ipv6 nodefaultroute usepeerdns persist maxfail 1 user "..data["username"].." password "..data["password"].." ip-up-script /lib/netifd/ppp-up ipv6-up-script /lib/netifd/ppp-up ip-down-script /lib/netifd/ppp-down ipv6-down-script /lib/netifd/ppp-down mtu "..data["mtu"].." mru 1492 plugin rp-pppoe.so nic-br-wan logfile "..logfile.."  >/dev/null 2>&1 &"`

# 2.pppd

文件路径：`target-mipsel_24kec+dsp_uClibc-0.9.33.2/ppp-default/ppp-2.4.7/pppd`

