





1.

​	步骤一：make <型号> ，如：make h20m   
​			作用：在顶级目录中生成.config文件   
​	步骤二：make rg_pre  
​			作用：更新软链接。调用一系列命令，包括：  
​					./scripts/feeds update -I               //通过 feeds.conf.default更新组件  
​					./scripts/feeds install -a               //更新软链接，make menuconfig才可以看得到  
​	步骤三：make V=s