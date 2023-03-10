**说明**：

​		本文章旨在总结备份、方便以后查询，由于是个人总结，如有不对，欢迎指正；另外，内容大部分来自网络、书籍、和各类手册，如若侵权请告知，马上删帖致歉。  
​		QQ 群 号：513683159 【相互学习】  
​		[openssl官网](https://www.openssl.org/)
&emsp;&emsp;[什么是Openssl](https://blog.csdn.net/a4729821/article/details/81635069)、[linux openssl简单的介绍与实例讲解](https://blog.csdn.net/funkri/article/details/17533887)、[openssl基本原理 + 生成证书 + 使用实例](https://blog.csdn.net/oldmtn/article/details/52208747)、[
&emsp;&emsp;SSL简介](https://www.cnblogs.com/chinabrle/p/4238637.html)、[SSL/TLS协议运行机制的概述](http://www.ruanyifeng.com/blog/2014/02/ssl_tls.html)、[Linux自学笔记——OpenSSL命令行工具](https://blog.51cto.com/claude666/1971422)

[TOC]

# 1.openssl基础概念

## 1.1. 什么是openssl？

​		[openssl-github](git%20clone%20https://github.com/openssl/openssl.git)、[openssl官网](https://www.openssl.org/)

&emsp;&emsp;一个强大的、开放源代码的、功能齐全的 **传输层安全(TLS) / 安全套接字层(SSL)协议的工具包**，实现一个**完整的通用密码库**，囊括：主要的密码算法、常用的密钥和证书封装管理功能及SSL协议，并提供丰富的应用程序供测试或其它目的使用。  
		采用C语言作为开发语言，具有优秀的跨平台性。

**（一）目标**  
		保证信息：  
			①保密性（防止窃听、通信量分析）  
			②完整性（防止更改、伪装、重放、否认）  
			③可用性（拒绝服务[Dos]）  
		可进行安全通信，避免窃听、确认连接身份等。  
**（二）解决方案**  
		①加密与解密：替换加密、置换加密、现代块加密  
		②服务：认证机制、访问控制机制    
		③密钥算法和协议：对称加密、公钥加密（非对称加密）、单向加密、认证协议  
**（三）什么是SSL？**  
		**1.描述**：SSL = Secure Sockets Layer = 安全套接层协议  
			位于可靠的面向连接的网络层协议和应用层协议之间的一种协议层。  
			能使用户/服务器应用之间的通信不被攻击者窃听，并且始终对服务器进行认证，还可选择对用户进行认证。

​		**2.历史**：  
​			由Netscape(网景)公司推出第一个Web浏览器的同时提出了SSL协议标准.  
​			TLS1.0与SSL3.0的差别非常微小。故：TLS 1.0通常被标示为SSL 3.1，TLS 1.1为SSL 3.2，TLS 1.2为SSL 3.3。  
​		**3.目的**：  
​			通过互相认证、使用数字签名确保完整性、使用加密确保私密性，以实现Internet上客户端和服务器之间的安全通讯（保密性和可靠性）。

## 1.2. openssl包含哪些组件？

&emsp;&emsp;密码算法库、SSL协议库以及应用程序。
### 1.2.1. 密码算法库（[libcrypto](https://www.openssl.org/docs/man3.0/man7/crypto.html)）
&emsp;&emsp;实现各种因特网标准中广泛使用的密码算法，即：通用密码库，是TLS实现的基础，可单独使用。  
**==（1）功能==**：  
		对称加密、公钥加密(非对称加密算法)、密钥协议、证书处理、密码散列函数、密码伪随机数生成器、消息验证代码(MACS)、密钥派生函数(KDF)和各种实用程序。  
**==（2）算法==**：  
		1.**对称加密算法**（8种）  
		==加密解密均使用同一种密钥==，包含：  
			①分组加密算法（7种）：<font color="red">**AES、DES、Blowfish、CAST、IDEA、RC2、RC5**</font>  
					均支持电子密码本模式（ECB）、加密分组链接模式（CBC）、加密反馈模式（CFB）和输出反馈模式（OFB）四种常用的分组密码加密模式  
			②流加密算法（1种）：<font color="red">**RC4**</font>  
		2.**非对称加密算法**（4种）：  
		==加密解密使用不同种密钥==，包含：  
			<font color="red">**DH算法、RSA算法、DSA算法和椭圆曲线算法（EC）**</font>  
			DH算法一般用户密钥交换，DSA算法一般只用于数字签名，RSA算法既可以用于密钥交换，也可以用于数字签名  
		3.**信息摘要算法**：（5种）  
		==无论原始数据长度输出均为固定长度的密文，该过程不可逆==，包含：  
			<font color="red">**MD2、MD5、MDC2、SHA（SHA1）和RIPEMD**</font>  
			作用：数据完整性的验证（若数据遭更改则输出摘要不同）。

#### 1.2.2.  SSL协议库（libssl）
&emsp;&emsp;TLSv1.3以下所有TLS协议版本的实现。  
		加密模块应用库，实现了ssl及tls。

#### 1.2.3. 应用程序（openssl）
&emsp;&emsp;多用途命令行工具，可用于加密、测试、分析。  
		1.**作用**：  
			①加解密、②消息摘要的计算、③创建X.509证书、CSR和CRLS、④关键参数的创建、⑤SSL/TLS客户端和服务器测试 等等。
	  		2.**运行方式**：  
			①交互模式。  
			②批处理/命令行模式。

## 1.3. 文献资料参考地址：
​		1️⃣[维基百科](https://wiki.openssl.org/index.php/Main_Page)  
​		2️⃣[手册](https://www.openssl.org/docs/manmaster/)  
​		3️⃣[openssl命令行](https://www.openssl.org/docs/manmaster/man1/)

# 2. 编译构建

## 2.1. 文件目录结构

### 2.1.1. 文件夹目录

```bash
.
├── apps
├── Configurations							#配置文件夹，README中有详细介绍
├── crypto
├── demos									#OpenSSL演示应用程序，README中有详细介绍
├── dev										#开发文件
├── doc										#OpenSSL 文档，里面还有介绍证书与密钥。
├── engines
├── external
├── fuzz									#OpenSSL可以使用LibFuzzer或AFL来进行fuzzer。
├── gost-engine
├── include
├── krb5
├── ms
├── os-dep
├── providers
├── pyca-cryptography
├── ssl
├── test
├── tools
├── util
├── VMS
└── wycheproof
```

### 2.1.2. 文件目录

```bash
.
├── ACKNOWLEDGEMENTS.md
├── appveyor.yml
├── AUTHORS.md					#OpenSSL作者列表
├── build.info
├── CHANGES.md					#重要更改的总结
├── config
├── config.com
├── configdata.pm.in
├── Configure
├── CONTRIBUTING.md				#指导如何贡献力量
├── e_os.h
├── FAQ.md						#常见问题		
├── HACKING.md					#如何添加自定义方法
├── INSTALL.md					#所有平台安装OpenSSL汇总，里面有非常详细的构建描述
├── LICENSE.txt					#许可证描述
├── NEWS.md						#一些更新说明
├── NOTES-ANDROID.md		
├── NOTES-DJGPP.md
├── NOTES-NONSTOP.md
├── NOTES-PERL.md
├── NOTES-UNIX.md				#
├── NOTES-VALGRIND.md
├── NOTES-VMS.md
├── NOTES-WINDOWS.md
├── README-ENGINES.md
├── README-FIPS.md
├── README.md					#
├── README-PROVIDERS.md
├── SUPPORT.md					#
├── VERSION.dat					#
```

## 2.2. 编译构建 简单描述（INSTALL.md）

### 2.2.1.源代码下载

&emsp;&emsp;方式一：git下载  
			执行命令：`git clone https://github.com/openssl/openssl.git`  
		方式二：官网下载  
			[openssl官网下载地址](Www.openssl.org/source)

### 2.2.2.编译环境支持：

​		1.make工具  
​		2.perl及其相关模组  
​		3.ANSI C编译器  
​		4.C的开发环境  
​		5.支持的操作系统

### 2.2.3.构建openssl

&emsp;&emsp;Linux：

```bash
    $ ./Configure
    $ make
    $ make test
```

### 2.2.4.安装openssl

​		建议:  
​			1.出于安全考虑：默认安装地址为`/usr/local`下，该位置默认不可写，故需切换为权限用户  
​			2.若某些平台自带openssl，建议不要覆盖，因为其他应用可能依赖。  
​		（故安装在不同位置，非系统库的全局搜索路径）（需加`sudo`）

```bash
make install
```

### 2.2.5.配置选项：

​		**1.安装到不同位置**：  
​			1️⃣`--prefix=<PREFIX>`：OpenSSL安装的顶部目录  
​					Makefile中`INSTALLTOP`的值，默认为：`/usr/local`，  
​					若不配置该选项，安装后可执行文件默认放在`/usr /local/bin`，库文件默认放在`/usr/local/lib`，配置文件默认放在`/usr/local/etc`，其它的资源文件放在`/usr /local/share`，比较凌乱

​			2️⃣`--openssldir=<PREFIX>`：OpenSSL配置文件的目录，以及默认的证书和密钥存储库。  
​					若为相对目录，将被添加到带有`prefix`的目录上，默认为：`/usr/local/ssl`  
​					如：`./Configure --prefix=/opt/openssl --openssldir=/usr/local/ssl`  
​		**2.指定不同版本API**：  
​			1️⃣`--api=x.y[.z]` 指定版本  
​			2️⃣`no-deprecated `：删除当前版本的所有已弃用的api    
​					如：`--api=1.1.0 no-deprecated `：将删除对所有在OpenSSL 1.1.0或更低版本中已弃用的api的支持。  
​		**3.指定交叉工具链**：  
​			` --cross-compile-prefix=<PREFIX> `：添加指定的前缀到binutils组件。    
​			如：`--cross-compile-prefix=x86_64-w64-mingw32-`在Linux上构建mingw64目标，指定交叉工具：mingw32.(需要安装好工具链及配置相关环境)

​		**4.构建带调试符的Openssl**
​	  
​			`  --debug`:适用调试符号和零优化级别构建openssl    
​		**5.构建不带调试符的Openssl**  
​			` --release `：默认选项  
​		**6.指定安装库的目录名**：  
​			`--libdir=DIR `：默认为lib。  
​		**7.切换OpenSSL开发中推荐的各种编译器选项（只在gcc或clang编译器时使用）**  
​			`--strict-warnings `：若正在开发OpenSSL的补丁，建议适用该选项。  
​		**8.添加给定的配置文件:**   
​			 `–config`：该文件将被读取在与此脚本相同目录下的任何`“Configurations*”`文件之后
​    
​		**9.显示Configure警告后不要等待**  
​			 `-w `：显示Configure警告后不要等待    
​		**10.不使用汇编程序**  
​			`no-asm `：不使用汇编程序  
​		**11.选择要不要zlib压缩**  
​			 `[no-]zlib`：[don’t]编译支持zlib压缩。   
​		**12关于`-match`的说明**
​    
​			 (*)尽管在llvm 3.6中实现了对AVX512的支持，但在7.0版本之前，显然需要显式的-march标志来编译汇编模块。对于大于7.0的版本，可以通过强制构建过程使用以下脚本来解决这个问题: 

```bash
  #!/bin/sh
    exec clang -no-integrated-as "$@"
```

​			 而不是真正的clang。在这种情况下，使用什么clang版本并不重要，因为要检查的是GNU汇编程序的版本。

==**更多请查看`INSTALL.md`文档。**==

### 2.2.6.脚本的环境配置选项

​    与普通环境变量赋值相同，但在所有平台都支持，并仅限配置脚本。  
​	若有相同环境变量，则会覆盖掉，选项有：

<table>
<tr>
<th>环境变量
<th>说明
</tr>
<tr>
<td>AR
<td>静态库归档程序
</tr>
<tr>
<td> ARFLAGS
<td>静态库存档器的标志。
</tr>
<tr>
<td>AS
<td>汇编编译器
</tr>
<tr>
<td>ASFLAGS
<td>汇编编译器的标志
</tr>
<tr>
<td>CC
<td>C编译器
</tr>
<tr>
<td>CFLAGS
<td>C编译器的标志
</tr>
<tr>
<td>CXX
<td>c++编译器
</tr>
<tr>
<td>CXXFLAGS
<td>c++编译器的标志
</tr>
<tr>
<td>CPP
<td>C/ C++预处理器
</tr>
<tr>
<td>CPPFLAGS
<td>C/ C++预处理器的标志
</tr>
<tr>
<td>CPPDEFINES
<td>CPP宏定义列表,Unix使用：空格或：分格。
</tr>
<tr>
<td> CPPINCLUDES 
<td>CPP包含目录列表，Unix使用：空格或：分格。
</tr>
<tr>
<td> HASHBANGPERL
<td>Perl调用被插入在'#!'在公共perl脚本(仅在Unix上相关)
</tr>
<tr>
<td> LD
<td>程序链接器(在Unix上不使用，这里使用$(CC))。
</tr>
<tr>
<td>LDFLAGS 
<td>共享库、DSO和程序链接器的标志
</tr>
<tr>
<td> LDLIBS
<td>链接时使用的额外库，Unix上采用空格分隔的库规范列表的形式
</tr>
<tr>
<td>RANLIB
<td>库归档索引器
</tr>
<tr>
<td> RC
<td>Windows资源编译器
</tr>
<tr>
<td>RCFLAGS
<td>Windows资源编译器的标志
</tr>
<tr>
<td>RM
<td>删除文件和目录的命令
</tr>
<tr>
<td>
<td>
</tr>
<tr>
<td>
<td>
</tr>
</table>


&emsp;&emsp;这些不能与命令行上给出的编译/链接标志混合
&emsp;&emsp;若设置`CC`，建议也设置`CXX`。

#### 7.重新配置

&emsp;&emsp;将先前的数据重新配置
&emsp;&emsp;这将从`configdata. pm`中保存的数据中获取之前的命令行选项和环境,并使用这些选项和环境再次运行配置过程。
&emsp;&emsp;注意:
&emsp;&emsp;&emsp;&emsp;其他选项不能和' reconf '一起使用。
&emsp;&emsp;&emsp;&emsp;原始配置保存了所使用的所有环境变量的值，如果它们没有定义，它们仍然保存了它们最初没有定义的信息。此信息优先于重新配置时定义的环境变量。

```bash
reconf 
reconfigure 
```

#### 8.显示配置数据

&emsp;&emsp;配置脚本本身描述得很少，并通过创建' configdata.pm '完成。这个perl模块可以由其他脚本加载，以查找所有配置数据，它还可以用作脚本，以人类可读的形式显示所有类型的配置数据。

```bash
   $ ./configdata. pm --help                         # Unix 
```

#### 9.安装目录介绍

&emsp;&emsp;找到安装目录，执行指令：`tree -d`

```bash
.
├── bin							#包含openssl二进制文件和一些其他实用程序脚本
├── include
│   └── openssl					#包含需要的头文件，如果您想构建自己的程序使用libcrypto或libssl
├── lib64						# OpenSSL库文件
│   ├── engines-3				# OpenSSL动态加载引擎
│   ├── ossl-modules
│   └── pkgconfig
├── share
│   ├── doc						#包含手册页的HTML呈现
│   │   └── openssl
│   │       └── html
│   │           ├── man1
│   │           ├── man3
│   │           ├── man5
│   │           └── man7
│   │               └── img
│   └── man
│       ├── man1				#OpenSSL命令行手册页。
│       ├── man3				#包含OpenSSL库调用手册页
│       ├── man5				# OpenSSL配置格式手册页
│       └── man7				#包含OpenSSL其他各种手册页
└── ssl
     ├── certs					# 最初为空，这是证书文件的默认位置
     ├── misc					# 不同的脚本
     └── private				# 最初为空，这是私钥文件的默认位置。  	
```

&emsp;&emsp;注意：安装目录应该得到适当的保护，以确保没有特权的用户不能更改OpenSSL二进制文件或文件，也不能安装引擎。如果您已经有一个预安装的OpenSSL版本作为您的操作系统的一部分，建议您不要覆盖该系统版本，而是将其安装到其他地方

#### 10.打包

&emsp;&emsp;希望将库配置为标准位置，但将包安装在其他地方以便容易打包的包构建者可以使用
&emsp;&emsp;指定的目标目录将预放在所有安装目标路径上。

```bash
make DESTDIR=/tmp/package-root
```

&emsp;&emsp;

#### 其他

&emsp;&emsp;下面还有环境变量的具体描述
&emsp;&emsp;make工具中的目标解释。
&emsp;&emsp;建议详细在查看该文档，此处仅简要描述。

## NOTES-UNIX.md（[看此翻译更优](https://github.com/openssl/openssl/blob/master/NOTES-UNIX.md)）

**OpenSSL使用编译器链接程序和共享库**
&emsp;&emsp;OpenSSL生成的Makefile使用C编译器命令行链接程序、共享库和可动态加载的共享对象。
&emsp;&emsp;因此，提供给配置脚本的任何链接选项必须是编译器可以接受的形式。这在不同的系统之间有所不同，有些系统的编译器直接接受链接标志，而另一些系统则采用`-Wl`形式。
&emsp;&emsp;您需要阅读编译器文档，以确定什么是可接受的，并' ld(1) '，以确定哪些链接器选项是可用的。
**共享库和安装在非默认位置**
&emsp;&emsp;每个Unix系统都有自己的共享库默认位置集，例如` /lib `、`/usr/lib `或可能的` /usr/local/lib `。如果库安装在非默认位置，动态链接的二进制文件将找不到它们，因此无法运行，除非它们从定义的运行时共享库搜索路径获得一些帮助。
&emsp;&emsp;对于OpenSSL的应用程序(' OpenSSL'命令)，我们的配置脚本通常不会为您设置运行时共享库搜索路径。因此，建议在配置时显式地设置它，除非将库安装在您知道位于默认列表中的目录中。
&emsp;&emsp;运行时共享库搜索路径由不同的操作系统和版本指定，并在各自的文档中进行了不同的讨论;`RPATH`的变量是最常见的(注意:ELF系统有两个这样的标记，下面有更多)。
&emsp;&emsp;==设置运行时共享库搜索路径的可能选项包括==:
` -Wl,-rpath,/whatever/path   # Linux, *BSD, etc.`
&emsp;&emsp;OpenSSL的配置脚本能够识别所有这些选项，并将它们传递给所构建的Makefile。(事实上，所有以` -Wl `开头的参数都被识别为链接器选项。)请注意`-Wl`中的`l`是小写的`l`而不是`1`。
&emsp;&emsp;请不要在运行时共享库搜索路径中使用逐字目录!一些OpenSSL配置目标为多库安装添加了额外的目录级别。为了帮助实现这一点，生成的makefile包含了变量`LIBRPATH`，这是一个方便的变量，可与运行时共享库搜索路径选项一起使用，如下例所示:

```bash
 $ ./Configure --prefix=/usr/local/ssl --openssldir=/usr/local/ssl   '-Wl,-rpath,$(LIBRPATH)'
```

&emsp;&emsp;在现代基于ELF的系统上，需要考虑两个运行时搜索路径标记，`DT_RPATH`和`DT_RUNPATH `。共享对象的搜索顺序如下:
&emsp;&emsp;&emsp;&emsp;1.使用`DT_RPATH`中指定的目录，除非也设置了`DT_RUNPATH`。
&emsp;&emsp;&emsp;&emsp;2.使用环境变量`LD_LIBRARY_PATH`
&emsp;&emsp;&emsp;&emsp;3.使用`DT_RUNPATH`指定的目录。
&emsp;&emsp;&emsp;&emsp;4.使用系统共享对象缓存和默认目录。
&emsp;&emsp;这意味着，如果库在`DT_RPATH`给出的路径中找到(并且`DT_RUNPATH`没有设置)，环境变量`LD_LIBRARY_PATH`中的值将无关紧要。
&emsp;&emsp;究竟是`DT_RPATH`还是`DT_RUNPATH`被默认设置，这取决于系统。例如，根据文档，` DT_RPATH `在Solaris上似乎不赞成使用`DT_RUNPATH `，而在Debian GNU/Linux上，两者都可以设置，`DT_RPATH `是写入时的默认值。
&emsp;&emsp;如何选择要设置的运行时搜索路径标签取决于您的系统，请参考ld(1)以获得关于您系统的确切信息。例如，确保` DT_RUNPATH `在Debian GNU/Linux系统上设置而不是`DT_RPATH`的方法是告诉链接器设置新的dtags，如下所示:

```bash
    $ ./Configure --prefix=/usr/local/ssl --openssldir=/usr/local/ssl '-Wl,--enable-new-dtags,-rpath,$(LIBRPATH)' 
```

&emsp;&emsp;值得注意的是，一些/大多数ELF系统通过解释`$ORIGIN`和其他一些内部变量，实现了相对于包含当前可执行文件的目录的运行时搜索路径的支持。参考您的系统文档。
**连接您的应用程序**
&emsp;&emsp;动态链接OpenSSL(或任何其他)共享库的第三方应用程序在非默认位置也面临完全相同的问题。上面提到的OpenSSL配置选项可能对目标应用程序的链接有影响，也可能没有影响。“Might”意味着在某些情况下，“自然地”链接OpenSSL共享库就足够了，即使用“-L/whatever/path -lssl -lcrypto”。但也有一些情况，当链接应用程序时，您必须显式地指定运行时搜索路径。参考你的系统文档并使用上面的章节作为灵感…
&emsp;&emsp;共享OpenSSL构建还安装静态库。与后者的链接可能需要特别注意，因为链接器通常首先查找共享库，并且倾向于对静态OpenSSL库保持“盲”。如果不是为了特殊情况，参考系统文档就足够了。在AIX静态库(在共享构建中)的命名是不同的，添加' _a '后缀来链接它们，例如。“-lcrypto_a”。

