参考网址：

​		[Linux下搭建 PPPoE Server 问题总结](https://blog.csdn.net/pdcxs007/article/details/44599885?spm=1001.2101.3001.6650.4&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-4-44599885-blog-5481355.pc_relevant_3mothn_strategy_and_data_recovery&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-4-44599885-blog-5481355.pc_relevant_3mothn_strategy_and_data_recovery&utm_relevant_index=4)

​		[Ubuntu 12.04上PPPoe服务的搭建和使用](http://blog.chinaunix.net/uid-9525959-id-4008338.html)

​		[服务器软件安装](http://t.zoukankan.com/loganblogs-p-7544608.html)

# 1. 搭建环境：

## 1.1. 搭建环境版本：

**①命令**：`uname -a`

```bash
 Linux ubuntu1604 4.15.0-112-generic #113~16.04.1-Ubuntu SMP Fri Jul 10 04:37:08 UTC 2020 x86_64 x86_64 x86_64 GNU/Linux
```

**②命令**：`lsb_release  -a`

   ``` bash
    Distributor ID:	Ubuntu
    Description:	Ubuntu 16.04.7 LTS
    Release:	16.04
    Codename:	xenial
   ```

## 1.2. rp-pppoe软件安装

（使用最新即可，此处使用3.15）
**①软件下载：**

​		官网下载：rp-pppoe-3.15.tar.gz
**②安装步骤：**
​    1.`sudo apt-get install ppp` (一般系统默认安装)
​    2.`tar -zxvf rp-pppoe-3.15.tar.gz`（解压压缩包到任意路径）
​    3.`sudo chown -R root:root  /xxx/rp-pppoe-3.15/`（改变文件属性）

​	4.`sudo apt-get installlibc6-dev`（安装软件开发函数库）

​	5.`sudo /xxx/rp-pppoe-3.15/go` (执行安装软件脚本，该脚本包括：configure,make, make install一条龙脚本)
​    **PS:**
​    	最好将go脚本的最后一句注释掉，这个是客户端的配置,即：`#sh../scripts/pppoe-setup`
**③检查是否安装成功**
​    **命令**：`whereis pppoe-server`

```bash
pppoe-server: /usr/sbin/pppoe-server /usr/share/man/man8/pppoe-server.8
```

# 2. 软件配置

## 2.1 配置文件路径

**路径：**/etc/ppp

## 2.2 配置文件修订

### 2.2.1. option配置文件:

**（1）快速查看文件配置选项：**`egrep -v '#|^ *$' /etc/ppp/options`

(2)

### 2.2.2

### 2.2.3

# 3. PPPoE命令

**命令：**`pppoe-server -h `

**结果：**

```bash
   -I if_name     -- Specify interface (default eth0.)
   -T timeout     -- Specify inactivity timeout in seconds.
   -C name        -- Set access concentrator name.
   -m MSS         -- Clamp incoming and outgoing MSS options.
   -L ip          -- Set local IP address.
   -l             -- Increment local IP address for each session.
   -R ip          -- Set start address of remote IP pool.
   -S name        -- Advertise specified service-name.
   -O fname       -- Use PPPD options from specified file
                     (default /etc/ppp/pppoe-server-options).
   -p fname       -- Optain IP address pool from specified file.
   -N num         -- Allow 'num' concurrent sessions.
   -o offset      -- Assign session numbers starting at offset+1.
   -f disc:sess   -- Set Ethernet frame types (hex).
   -s             -- Use synchronous PPP mode.
   -X pidfile     -- Write PID and lock pidfile.
   -q /path/pppd  -- Specify full path to pppd.
   -Q /path/pppoe -- Specify full path to pppoe.
   -u             -- Pass 'unit' option to pppd.
   -r             -- Randomize session numbers.
   -d             -- Debug session creation.
   -x n           -- Limit to 'n' sessions/MAC address.
   -P             -- Check pool file for correctness and exit.
   -i             -- Ignore PADI if no free sessions.
   -M msg         -- Send MSG in a MOTM tag in PADM packet after PADS.
   -H url         -- Send URL in a HURL tag in PADM packet after PADS.
   -h             -- Print usage information.
```


（5）

