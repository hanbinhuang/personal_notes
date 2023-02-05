Linux在启动后，到C入口时，会执行以下操作，加载系统平台上的总线和设备：

start_kernel() --> setup_arch() --> unflatten_device_tree() 





make config：这个要求你手动选择所有配置项，配置完成后生成.config文件

make menuconfig：显示以curses的图形配置菜单，当已有.config文件存在时，它会根据.config文件设定默认项。若不存在.config文件，则根据各级Kconfig文件来设定菜单项，完成配置后生成.config文件

make oldconfig：与make menuconfig相同，区别在于这个没有图形界面，当已有.config文件存在时，它根据.config文件设定默认项，若kconfig有新配置项时，会提示你进行选择，若不存在.config文件，则根据各级Kconfig文件来设定菜单项，完成配置后生成.config文件。若已存在.config文件, make menuconfig及make oldconfig都会把原.config另存为.config.old

make xx_defconfig:  一般源码中都有不同硬件平台的默认配置文件，你也可以制做自己的默认配置文件,当这个命令执行时，它会根据kconfig及xx_defconfig来生成.config文件。在执行完其中一条config命令后，会生成.config及autoconf文件，autoconf是根据配置项生成的相应宏定义，供makefile使用，当执行make命令时，就会根据autoconf定义的宏及makefile去编译源码







typoya 如何换行间隙正常：两个空格+ Shift 回车

https://blog.csdn.net/liangzc1124/article/details/127586874?spm=1001.2014.3001.5501



https://blog.csdn.net/sfdgfdvc/article/details/119514247



