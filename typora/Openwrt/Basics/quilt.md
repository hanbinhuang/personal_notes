# 1.概念



# 2.命令





# 3.举例

1. 进入对应的想要打补丁的内核根目录下   
   	~/trunk51/image-build/code/build_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/linux-ramips_ew1200i/linux-3.10.108$
2. 在该目录下新建一个补丁：  
       quilt new xxx 
3. 确定该补丁是修改哪个目录下的文件：  
        quilt add xxx/xxx
4. 编辑对应的文件。
5. 将修改后内容生效形成对应的补丁文件（当前目录下的patches文件中）：  
        quilt refresh 
6. 若有两个相同内核的文件可以同时进行修改
7. 可使用 : make     target/linux/install 单独编译内核