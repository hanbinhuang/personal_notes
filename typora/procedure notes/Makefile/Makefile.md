



## 1.选项：

[make -C M=与make -C SUBDIRS=](https://blog.csdn.net/qq_40334837/article/details/89515751)

`$(MAKE) -C $(KERNELDIR) M=$(PWD) modules`

​		**$(MAKE)**就相当于**make**  
​		**-C** 选项的作用是指将当前工作目录转移到你所指定的位置  
​		**“M=”**选项的作用是，当用户需要以某个内核为基础编译一个外部模块的话，需要在**make modules** 命令中加入**“M=dir”**，程序会自动到你所指定的**dir**目录中查找模块源码，将其编译，生成**KO**文件