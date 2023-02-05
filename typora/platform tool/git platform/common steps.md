# 1.Git提交与服务器编译

1. Git add .

2. Git     restore 文件 (若某个文件不想提交则在此处进行恢复，可根据提示)

3. Git     commit -m “xxx”

4. Git review

5. 上网页：shinya.ruijie.net:8929

6. 1. menu下projects查找对应的工程,
   2. 找到对应的工程后应该可找到刚刚提交的的记录:creat      merge request
   3. 点进去后将previews选择cfm 后点击create

7. 等待蕃铭哥评审即可。

# 2.服务器网站编译

1.进入shinya.ruijie.net:8929

2.进入 mtk 

3.CI/CD

4.进入 pipeline

5.Variable填入PRODUCT 产品名

6.run pipeline

# 3.git本地编译

1️⃣准备工作

参照文档：Git 服务器编译方法 下载

2️⃣

1.进入imagexx后有：code docker xxxx文件夹

2.进入容器 make docker-slave-bash，后会进入code文件夹

3.make 产品型号   会将对应的配置文件复制到当前目录

4.make rg_pre  会从去feeds.conf中的网址去下载对应的包

5.make -j20

 

重新编译最好要删除以下两部分：

| rm build_dir/target-mipsel_24kc_musl/root-ramips/ -rf        | 这个是文件系统，删除他防止固件会从这边生成文件系统           |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| rm bin/ramips/                                               | Make rg_pre时会将该文件删除并重新创建一个新的                |
| rm build_dir/target-mipsel_24kc_musl/linux-ramixxxxxxxxxxx/linux-4.4.  xxx -rf | 这个是内核与驱动，也要删除，在往前一个目录就是对应的一些组件 |

 

我们的固件是包含两个部分：内核和文件系统

3️⃣单独编译Linux 内核

make target/linux/install V=s (需在工程根目录下进行)

4️⃣单独编译某个组件

make ./package/feeds/devmng/lan_wan_switch/compile V=s