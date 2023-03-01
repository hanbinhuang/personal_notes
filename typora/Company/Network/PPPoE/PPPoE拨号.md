[pppoe](https://blog.csdn.net/qq_43608147/article/details/127442859)

[pppoeeeee](https://blog.csdn.net/phunxm/article/details/9384123?spm=1001.2101.3001.6650.2&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-2-9384123-blog-124988104.pc_relevant_3mothn_strategy_and_data_recovery&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-2-9384123-blog-124988104.pc_relevant_3mothn_strategy_and_data_recovery&utm_relevant_index=5)

[pppoe1](https://linyug.blog.csdn.net/article/details/128378824?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EYuanLiJiHua%7EPosition-1-128378824-blog-9384123.pc_relevant_multi_platform_whitelistv3&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EYuanLiJiHua%7EPosition-1-128378824-blog-9384123.pc_relevant_multi_platform_whitelistv3&utm_relevant_index=2)



# 1. PPPoE概念

​		PPPoE = Point-to-Point Protocol Over Ethernet = 基于以太网的点对点通讯协议

​		**角色**：  
​				1.PPPoE Client：Host,一般为家用路由器   
​				2.PPPoE Server：AC，一般为运营商路由器



# 2.PPPoE过程

## 2.1. 发现阶段（discovery阶段）

​		该阶段用来建立Host(**Client**)与AC(**Server**)连接，通过识别PPPoE Server的以太网MAC地址并确定建立的PPP会话标识符(Session ID)，同时获得的对方点到点的连接信息。  

​		协议：PPPoeED

### 2.1.1. PADI

​		PADI = PPPoE Active Discovery Initiation.  

​		<font color="red">**描述：**</font>**用户主机（PPPoE  Client）**以**广播**的方式发送 PADI数报包，**请求**建立链路。   
​					Code域置为0x09，会话ID域必须置为0x0000。  
​					PADI数据包含有客户端请求的服务。

​		<font color="red">**举例：**</font>

​				**Source:** Cilent设备` (30:0d:29:a1:c6:11)`  [Host]  
​                **Destination**: `Broadcast (ff:ff:ff:ff:ff:ff)`

​			![image-20230210124237879](C:\Users\acer\AppData\Roaming\Typora\typora-user-images\image-20230210124237879.png)

### 2.1.2. PADO

​		PADO = PPPoE Active Discovery Offer

​		<font color="red">**描述：**</font>**访问集中器/AC（PPPoE  Server）**收到PADI包后并判断是否可提供服务。    
​							若可则以**单播**的方式发送一个PADO数据包对主机的请求做出**应答**。   
​							若不可则不提供响应报文。   
​					目的地址为主机的MAC地址，Code域置为0x07，会话ID域必须置为0x0000。  
​					PADO数据包必须包含一个类型为AC-Name的Tag（包含了访问集中器的名字）可能包含一/多个服务名。

​		<font color="red">**举例：**</font>

​				**Source**: Server设备`(9c:74:1a:4c:68:6b)`[AC]  
​				**Destination**: Cilent设备`(30:0d:29:a1:c6:11)`[Host]

![image-20230210124431698](C:\Users\acer\AppData\Roaming\Typora\typora-user-images\image-20230210124431698.png)

### 2.1.3. PADR 

​		PADR = PPPoE Active Discovery Reques

​		<font color="red">**描述：**</font>因为 PADI数据包是广播方式发送，所以主机可能收到不止一个的PADO报文。主机在收到报文后，会根据AC-Name或PADO所提供的服务来选择一个AC，然后**用户主机（Client）**向选中的AC**单播**一个PADR数据包做出**应答**。   
​					目的地址域为AC的MAC地址，Code域置为0x19，会话ID域必须置为0x0000。  
​					PADR报文必须且只能包含一个Tag_Type为Service-Name的Tag，表明主机请求的服务。  
​					当一个用户主机在确定时间没有收到PADO,它会从发一个PADI,同时等待两倍的时间，可能需要重复多次。

​		<font color="red">**举例：**</font>

​					**Source**: Cilent设备`(30:0d:29:a1:c6:11)`[Host]  
​                    **Destination**: Server设备`(9c:74:1a:4c:68:6b)` [AC]   

![image-20230210124644847](C:\Users\acer\AppData\Roaming\Typora\typora-user-images\image-20230210124644847.png)

### 2.1.4. PADS

​		PADS = PPPoE Active Discovery Session-confirmation

​		<font color="red">**描述：**</font>**访问集中器（ AC/Server）**在收到PADR报文时，就准备开始一个PPP的会话了。

​					为PPPoE会话**创建一个唯一的会话ID**并用**单播**一个PADS数据包来给主机做出**响应**。  
​					目的地址域为主机的MAC地址，Code域置为0x65，会话ID必须设置为所创建好的会话ID。

​		<font color="red">**举例：**</font>

​					**Source**:Server设备`(9c:74:1a:4c:68:6b)`[AC]  
​					**Destination**:Cilent设备 `(30:0d:29:a1:c6:11)`[Host]

![image-20230210125023386](C:\Users\acer\AppData\Roaming\Typora\typora-user-images\image-20230210125023386.png)




## 2.2. 会话阶段（Session阶段）

​		该阶段Host(Client)与AC(Server)进行标准的PPP协商，协商后数据通过PPP封装发送。

### 2.2.1. LCP协商阶段

​		LCP = Link Control Protocol  
​		协议：PPP LCP

#### 2.2.1.1. LCP链路配置报文

​		用来建立和配置一条链路，主要包括Configure-Request、Configure-Ack、Configure-Nak和Configure-Reject报文  
​		协商双方相互发送一个LCP Configure-Request报文，确认报文中的协商选项根据选项的支持与接受情况回应LCP Configure-ACK报文作出回应，标志着LCP链路建立成功，否则会持续发送Request报文直到回复ACK报文为止。

​		说明：  
​				（1）Config-ACK：若完全支持对端的LCP选项，则回应Config-ACK报文，报文中必须完全协带对端Request报文中的选项。				（2）Config-NAK：若支持对端的协商选项，但不认可该项协商的内容，则回应Config-NAK报文，在Config-NAK的选项中填上自己期望的内容，如:对端MRU值为1500，而自己期望MRU值为1492，则在Config-NAK报文中埴上自己的期望值1492。  
​				（3）Config-Reject：若不能支持对端的协商选项，则回应Config-Reject报文，报文中带上不能支持的选项，如Windows拨号器会协商CBCP（被叫回呼），而ME60不支持CBCP功能，则回将此选项拒绝掉。

**①Configure-Request报文（AC->Host）**

​		<font color="red">**举例：**</font>

​					**Source**:Server设备`(9c:74:1a:4c:68:6b)`[AC]  
​					**Destination**:Cilent设备 `(30:0d:29:a1:c6:11)`[Host]

![image-20230210135547320](C:\Users\acer\AppData\Roaming\Typora\typora-user-images\image-20230210135547320.png)



**②Configure-Request报文（Host->AC）**

​		<font color="red">**举例：**</font>

​					**Source**:Cilent设备 `(30:0d:29:a1:c6:11)`[Host]  
​					**Destination**:Server设备`(9c:74:1a:4c:68:6b)`[AC]

![image-20230210135737328](C:\Users\acer\AppData\Roaming\Typora\typora-user-images\image-20230210135737328.png)



**③Configure-ACK报文（AC->Host）**

​		<font color="red">**举例：**</font>

​					**Source**:Server设备`(9c:74:1a:4c:68:6b)`[AC]  
​					**Destination**:Cilent设备 `(30:0d:29:a1:c6:11)`[Host]

![image-20230210140405247](C:\Users\acer\AppData\Roaming\Typora\typora-user-images\image-20230210140405247.png)



**④Configure-ACK报文（Host->AC）**

​		<font color="red">**举例：**</font>

​					**Source**:Cilent设备 `(30:0d:29:a1:c6:11)`[Host]  
​					**Destination**:Server设备`(9c:74:1a:4c:68:6b)`[AC]

![image-20230210140729908](C:\Users\acer\AppData\Roaming\Typora\typora-user-images\image-20230210140729908.png)



#### 2.2.1.2.  LCP链路维护报文

​		用来管理和调试链路，主要包括Code-Reject、Protocol-Reject、Echo-Request、Echo-Reply和Discard-Request报文

**①Echo Request报文**

​		<font color="red">**举例：**</font>

​					**Source**:Cilent设备 `(30:0d:29:a1:c6:11)`[Host]  
​					**Destination**:Server设备`(9c:74:1a:4c:68:6b)`[AC]

![image-20230210143821550](C:\Users\acer\AppData\Roaming\Typora\typora-user-images\image-20230210143821550.png)

**②Echo Reply报文** 

​		<font color="red">**举例：**</font>

​					**Source**:Server设备`(9c:74:1a:4c:68:6b)`[AC]  
​					**Destination**:Cilent设备 `(30:0d:29:a1:c6:11)`[Host]

![image-20230210143846605](C:\Users\acer\AppData\Roaming\Typora\typora-user-images\image-20230210143846605.png)





#### 2.2.1.3. LCP链路终止报文

用来终止一条链路，主要包括Terminate-Request和Terminate-Reply报文



### 2.2.2. LCP认证阶段

​		认证协议类型：  
​				①PAP = Password Authentication Protocol  
​				②CHAP = Challenge Handshake Authentication Protocol = 质询握手认证协议    

​		协议：		

2.2.2.1.

**①Challenge报文**

​		**验证方**向**被验证方**发送随机产生的数据和本端主机名。

​		<font color="red">**举例：**</font>

​					**Source**:Server设备`(9c:74:1a:4c:68:6b)`[AC]  验证方  
​					**Destination**:Cilent设备 `(30:0d:29:a1:c6:11)`[Host]  被验证方

![image-20230210144738407](C:\Users\acer\AppData\Roaming\Typora\typora-user-images\image-20230210144738407.png)



**②Response报文**、

​		被验证方 接收到验证方的验证请求（Challenge）时，根据此报文中验证方的主机名和本端的用户表查找用户口令字，如找到用户表中与验证方主机名相同的用户，便利用报文ID、此用户的密钥用Md5算法生成应答（Response），随后将应答和自己的**主机名**送回

​		<font color="red">**举例：**</font>

​					**Source**:Cilent设备 `(30:0d:29:a1:c6:11)`[Host]  被验证方  
​					**Destination**:Server设备`(9c:74:1a:4c:68:6b)`[AC]  验证方

​		![image-20230210145127648](C:\Users\acer\AppData\Roaming\Typora\typora-user-images\image-20230210145127648.png)

**③Success报文**

​		**验证方**接到此应答后，用报文ID、本方保留的口令字（密钥）和随机报文用Md5算法得出结果，与被验证方应答比较，根据比较结果返回相应的结果（ACK or NAK）

​		<font color="red">**举例：**</font>

​					**Source**:Server设备`(9c:74:1a:4c:68:6b)`[AC]  验证方  
​					**Destination**:Cilent设备 `(30:0d:29:a1:c6:11)`[Host]  被验证方

![image-20230210150145182](C:\Users\acer\AppData\Roaming\Typora\typora-user-images\image-20230210150145182.png)

2.2.2.2.



### NCP协商阶段

​		NCP = Network Control Protocol  
​		协议种类：  
​				IPCP = Internet Protocol Control Protocol



![image-20230210162347018](C:\Users\acer\AppData\Roaming\Typora\typora-user-images\image-20230210162347018.png)











## 2.3. PADT

​		PADT = PPPoE Active Discovery Terminate

​		<font color="red">**描述：**</font>



![image-20230210133656838](C:\Users\acer\AppData\Roaming\Typora\typora-user-images\image-20230210133656838.png)





