

# 1.pppoeCheck

## 1.1.pppoeCheck目的

​		用来实时检测pppoe拨号状态，进行Web弹窗处理。

## 1.2.pppoeCheck文件

**① log文件(rp-pppoe_check.log)：**  
		文件目的：输出log，记录每次set的信息  
		文件路径：`/tmp/debug/rp-pppoe/rp-pppoe_check.log`  
**②配置文件(pppoe_cfg_wan)：**  
		文件目的：用于读取作为配置参数  
		文件路径：`g_pppoe_cfg = "/tmp/debug/rp-pppoe/pppoe_cfg_" + “接口名”`  
**③pppoeCheck暂存文件（pppoe_check_Cwan.log）**  
		 文件目的：ppp写状态的文件，每次get接口调用会重命名为读取拨号状态文件  
		 文件路径：`g_check_file_tmp   = "/tmp/debug/rp-pppoe/pppoe_check_C"+ “接口名” + log`  
**④pppoeCheck文件（pppoe_check_Cwan）**  
		 文件目的：读取拨号状态文件  
		 文件路径：`g_check_file     = "/tmp/debug/rp-pppoe/pppoe_check_C"+ “接口名”`  

**⑤Cwan.log**  
		文件目的：作为up命令的参数传入pppd进程（作为pppd进程的log文件：logfile_name）该文件记录了报文的收到内容和数据  
		文件路径：`g_pppoe_path.."/".."C"..data["intf_name"]..".log"`

## 1.3. get接口

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

总结：  
		调用get接口根据，读取pppoe_Check文件内容，该文件中包含pppoe拨号的实时内容，若非正常或失败则会调动down命令关闭pppd进程，（为什么成功也要down？），失败Web会以弹窗形式返回，成功则以转圈形式，pppd进程down掉后还会被重新拉起来。同时还会删除pppoe_Check文件。

## 1.4. set接口

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

步骤四：执行"`ps |grep /usr/sbin/pppd |grep -v grep|awk '{print $1}'| xargs kill >/dev/null 2>&1`"= cmd_down将该结果写入g_pppoe_cfg 配置文件**【可优化，猜想该配置文件作用仅记录当前口拨号的pppd进程ID，可在运行进程时使用`$$`保存PID】**

步骤五：

cmd_teardown = ` sh /lib/netifd/proto/ppp.sh pppoe teardown "..data["intf_name"].." '{\"ifname\":\"br-"..data["intf_name"].."\",\"ifname\":\""..data["ifname"].."\",\"proto\":\"pppoe\",\"metric\":"..data["metric"]..",\"username\":\""..data["username"].."\",\"password\":\""..data["password"].."\",\"ppp_mtu\":"..data["mtu"].."}' br-"..data["intf_name"].." >/dev/null 2>&1`

cmd_up = `/usr/sbin/pppd nodetach ipparam "..data["intf_name"].." ifname pppoe-"..data["intf_name"].." +ipv6 nodefaultroute usepeerdns persist maxfail 1 user "..data["username"].." password "..data["password"].." ip-up-script /lib/netifd/ppp-up ipv6-up-script /lib/netifd/ppp-up ip-down-script /lib/netifd/ppp-down ipv6-down-script /lib/netifd/ppp-down mtu "..data["mtu"].." mru 1492 plugin rp-pppoe.so nic-br-wan logfile "..logfile.."  >/dev/null 2>&1 &"`

总结：

​		set下发命令，先检查下发参数是否异常，将down命令写入配置文件，删除之前的暂存的文件。若是dhcp/static协议则运行up命令，否则判断pppoe-wan接口是否link，若link则直接teardown并up，若unlink则延时3s在进行teardown并up。

## 1.5. log记录：



```
root@Ruijie:/tmp/debug/rp-pppoe# cat rp-pppoe_check.log 
[22774][2023-02-11 09:54:05] pppoeCheck.lua [15] [DEBUG] start

[22774][2023-02-11 09:54:05] pppoeCheck.lua module_set() [180] [DEBUG] ************** start pppoeCheck **************

[22774][2023-02-11 09:54:05] pppoeCheck.lua module_set() [202] [DEBUG] intf_name:wan

[22774][2023-02-11 09:54:05] pppoeCheck.lua module_set() [223] [DEBUG] ifname:eth0.1

[22774][2023-02-11 09:54:05] pppoeCheck.lua module_set() [226] [DEBUG] username:0591917886788

[22774][2023-02-11 09:54:05] pppoeCheck.lua module_set() [234] [DEBUG] password:768999

[22774][2023-02-11 09:54:05] pppoeCheck.lua module_set() [242] [DEBUG] mtu:1480

[22774][2023-02-11 09:54:05] pppoeCheck.lua module_set() [268] [DEBUG] wan is link

[22774][2023-02-11 09:54:05] pppoeCheck.lua module_set() [284] [DEBUG] cmd_teardown:sh /lib/netifd/proto/ppp.sh pppoe teardown wan '{"ifname":"br-wan","ifname":"eth0.1","proto":"pppoe","metric":0,"username":"0591917886788","password":"768999","ppp_mtu":1480}' br-wan >/dev/null 2>&1

[22774][2023-02-11 09:54:05] pppoeCheck.lua module_set() [285] [DEBUG] cmd_up:/usr/sbin/pppd nodetach ipparam wan ifname pppoe-wan +ipv6 nodefaultroute usepeerdns persist maxfail 1 user 0591917886788 password 768999 ip-up-script /lib/netifd/ppp-up ipv6-up-script /lib/netifd/ppp-up ip-down-script /lib/netifd/ppp-down ipv6-down-script /lib/netifd/ppp-down mtu 1480 mru 1492 plugin rp-pppoe.so nic-br-wan logfile /tmp/debug/rp-pppoe/Cwan.log  >/dev/null 2>&1 &

[22774][2023-02-11 09:54:05] pppoeCheck.lua module_set() [286] [DEBUG] cmd_down:ps |grep /usr/sbin/pppd |grep -v grep|awk '{print $1}'| xargs kill >/dev/null 2>&1

[23155][2023-02-11 09:54:08] pppoeCheck.lua [15] [DEBUG] start

[23155][2023-02-11 09:54:08] pppoeCheck.lua module_get() [102] [DEBUG] sysTime:1676080448

[23155][2023-02-11 09:54:08] pppoeCheck.lua module_get() [103] [DEBUG] fileTime:1676080445

[23155][2023-02-11 09:54:08] pppoeCheck.lua module_get() [104] [DEBUG] num:3

[23238][2023-02-11 09:54:09] pppoeCheck.lua [15] [DEBUG] start

[23238][2023-02-11 09:54:09] pppoeCheck.lua module_get() [102] [DEBUG] sysTime:1676080449

[23238][2023-02-11 09:54:09] pppoeCheck.lua module_get() [103] [DEBUG] fileTime:1676080445

[23238][2023-02-11 09:54:09] pppoeCheck.lua module_get() [104] [DEBUG] num:4

[23586][2023-02-11 09:54:11] pppoeCheck.lua [15] [DEBUG] start

[23586][2023-02-11 09:54:11] pppoeCheck.lua module_get() [102] [DEBUG] sysTime:1676080451

[23586][2023-02-11 09:54:11] pppoeCheck.lua module_get() [103] [DEBUG] fileTime:1676080445

[23586][2023-02-11 09:54:11] pppoeCheck.lua module_get() [104] [DEBUG] num:6
```



# 2.pppd

文件路径：`target-mipsel_24kec+dsp_uClibc-0.9.33.2/ppp-default/ppp-2.4.7/pppd`

