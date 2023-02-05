[TOC]

参考文章：[设备树](https://blog.csdn.net/weixin_45264425/article/list/3?t=1)、[设备树2](https://blog.csdn.net/qq_17639223/category_10341756.html)

# 1. 基础认识

## 1.1. 设备树概念

​		设备树 = device tree = dt .

​		内核版本2.6 之后引入设备树的概念，用于描述硬件平台的硬件资源信息，如：CPU的数量和类别、内存基地址和大小、总线和桥、外设连接、中断控制器和中断使用情况、GPIO控制器和GPIO使用情况、Clock控制器和Clock使用情况等等。

​		linux内核采用的参数表示和传递技术，在系统引导启动阶段进行设备初始化的时候，将设备树中描述的硬件信息传递给操作系统；

## 1.2. 设备树文件格式

### 1.2.1 dts文件

​		**（1）简介**：**dts** = device tree source  =  **设备树源文件**

​		**（2）作用**：描述开发板结构和开发板上的设备信息。

​		**（3）文件路径：**

​				**工程路径**：`./target/linux/mediatek/files-5.4.194/arch/arm64/boot/dts/mediatek/ruijie_h30m_gsw.dts`

​				**内核源码路径**：`./build_dir/target-aarch64_cortex-a53_musl/linux-mediatek_h30m/linux-5.4.194/arch/arm64/boot/dts/mediatek/ruijie_h30m_gsw.dts`

### 1.2.2 dtc文件

​		**（1）简介**：**dtc** = device tree compiler  =  **设备树编译/反编译/调试工具**

​		**（2）作用**：将dts文件编译成dtb文件的工具。

​		**（3）文件路径：**

​				`./build_dir/toolchain-aarch64_cortex-a53_gcc-7.3.0_musl/linux-5.4.194/scripts/dtc/`

​				`./build_dir/target-aarch64_cortex-a53_musl/linux-mediatek_h30m/linux-5.4.194/scripts/dtc/`

### 1.2.3 dtb文件

​		**（1）简介**：**dtb**  =  device tree binary =  **二进制设备树文件**

​		**（2）作用**：dts编译后得到的二进制文件。

​		**（3）文件路径：**

### 1.2.4 dtsi文件

​		**（1）简介**：**dtsi** = device tree source include =  **设备树头文件**

​		**（2）作用**：可被dts文件通过include引用，一般描述设备公共部分，如：一个Soc可对应多个产品和电路板，故可将共同部分提炼放在dtsi文件中。

​		**（3）文件路径：**

## 1.3. 设备树作用

​	（1）设备驱动源码中，分为：①**驱动代码**：操作硬件的方法.    ②**设备代码**：硬件资源、数据. 

当驱动代码和设备代码匹配时就会调用驱动的probe函数，probe函数会利用设备代码的资源去初始化设备；

​	（2）有无设备树的区别：

​				①**无设备树**：设备代码直接写在源码中，以`platform_device`结构体的形式存在。

驱动代码和设备代码通过`platform`总线上匹配，若需修改设备资源时，则需修改内核源码；

​				②**有设备树**：设备的硬件资源信息写在dts文件中，只需修改dts文件，不必修改内核源码.

​				③**总结**：不采用设备树技术：内核源码中会充斥大量设备硬件描述信息，导致内核源码不停增多，但是增多的硬件描述信息代码和内核功能并不相关；采用设备树技术之后：设备的硬件描述信息都在dts文件中，修改方便，但是内核要增加解析dts文件格式的代码；

​	（3）特点：

​				①实现驱动代码与设备硬件信息分离

​				②通过bootloader(uboot)和Linux传递到内核，内核从设备树中获取对应的硬件信息。

​				③同一soc不同主板只需更换设备树文件即可实现不同主板的无差异支持，无需更换内核，实现内核和不同板级硬件数据的拆分

## 1.4. 设备树工作流程

​		（1）驱动开发者根据硬件编写/修改dts文件，使得将来驱动代码能匹配到合适的设备硬件信息；
​		

​		（2）编译内核时，kernel会先编译出dtc，然后再用dtc将dts文件编译成dtb；
​		

​		（3） uboot启动kernel时，将内核镜像和dtb都重定位到内存，并告诉内核dtb的所在内存地址；
​		

​		（4）内核启动初期调用内部函数解析dtb，把每个结点转换为`device_node`结构体。

​		（5）对于某些`device_node`结构体会被转换为`platform_device`结构体。

​		（6）得到硬件信息后再组装成硬件函数，最后去和驱动代码进行匹配；

## 1.5. 设备树编译命令

​		（1）编译命令：`make  dtbs`

​		（2）单独编译某个dts文件命令：`make xxx.dtb`

​		（3）反汇编命令：`./scripts/dtc/dtc -I dtb -O dts -o xxx.dts arch/arm/boot/dts/xxx.dtb`

# 2. 设备树源码格式

## 2.1.  格式样式及语法描述

### 2.1.1 dts文件通用样式

```
/dts-v1/;

#include <dt-bindings/xxx/xxx.h>
#include "xxx.dtsi"


/ {
	[label:] <node1> [@<unit-address>]{
      [property]
      [child nodes]
      [child nodes]
      ......
	};
	
	[label:] <node2> [@<unit-address>]{
      [property]
      [child nodes]
      ......
	};
};

&label{
	[property]
}
或在根节点之外使用全路径
&{/label@<unit-address>}{
	[property]
}
PS:[]表示可省略,<>：表示该项不可省略
```

### 2.1.2. dts样式解析

（1）`/dts-v1/;`：表示dits版本号，目前都是v1

（2）**文件包含（include）**：

​			①`#include <dt-bindings/xxx/xxx.h>`：包含C头文件.

该文件位于内核的`arch/arm/boot/dts/include/dt-bindings`.

​			②`#include "xxx.dtsi"`：包含设备树头文件

（3）**节点（node）**：

​			①`/{}`，表示根节点，理论只有一个根节点，由此开始描述设备信息

​			②节点名不可省略

​			③dts为树状节点组织，除根节点外其他结点均为父结点。

（4）**标签（label）：**

​			①`[label:]`：label为标签名，可省略。

​			②目的：方便访问后面可使用标签引用节点，如：`&label`

（5）**设备地址(unit-address)：**

​			①`[@unit-address]`:unit-address为设备地址，如：`cpu node 就是0、1`。

​			②若节点无地址或寄存器则可省略。

（6）**属性格式(property)：**

​		属性格式形式："name = value",value有多种取值方式

​			①一个**32位**的数据，用**尖括号**包围起来，如：`interrupts = <17 0xc>;`
​				

​			②一个**64位**数据（使用2个32位数据表示），用**尖括号**包围起来，如：

​					`clock-frequency = <0x00000001 0x00000000>;`
​				

​			③有**结束符**的字符串，用**双引号**包围起来，如：	`compatible = "simple-bus";`
​				

​			④**字节序列**，用**中括号**包围起来，如：
​						

​						`local-mac-address = [00 00 12 34 56 78]; // 每个byte使用2个16进制数来表示`
​						

​						`local-mac-address = [000012345678];      // 每个byte使用2个16进制数来表示`
​				

​			⑤可以是各种值的**组合**，用**逗号**隔开，如：
​						

​						`compatible = "ns16550", "ns8250";`
​						

​						`	example = <0xf00f0000 19>, "a strange property format";`

（7）**追加结点**：

​			①`&label`: 对某结点的追加，该结点前有label修饰，这种引用是通过phandle(pointer handle，指针句柄)进行。

（8）**注释**： `/* */`

（9）**分隔符**：

​			<1>**段落块之间分隔符：**`;`

​			<2>**段落段分隔符：**`{}`

## 2.2. 结点属性分析

[linux设备树dts文件详解](https://blog.csdn.net/weixin_42031299/article/details/125813060?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-125813060-blog-126114806.pc_relevant_default&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-125813060-blog-126114806.pc_relevant_default&utm_relevant_index=2)

### 2.2.1. compatible属性

​		**<1>格式**：`compatible = "manufacturer1,model1"，"manufacturer2,model2";`

​				`manufacturer`：厂商，``model`：驱动名。

​		**<2>作用**：<u>compatible属性用于结点与驱动匹配</u>，在内核描述驱动的`struct device_driver`结构体中，若与`compatible`属性中字符串相匹配，则设备代码与驱动代码成功匹配。

​		PS:`compatible`后可有多个字符串，优先匹配靠前的字符串，若靠前字符串匹配不上则向后匹配。

​		**<3>举例**：

​		`compatible = "fsl,mpc8641", "ns16550";`

### 2.2.2. model 属性

​		**<1>格式**：`model = "xxx";`

​		**<2>作用**：

​				①<u>`model`是描述模块信息的</u>，一般只有根节点才有，标明设备树文件对应的开发板的名称；

​				②在内核的启动打印中可以看到`model`的值：`“OF: fdt:Machine model: Tyr DEMO Board”;`

​		**<3>举例**：

​				`model = "fsl,MPC8349EMITX";`

### 2.2.3. status属性

​		**<1>格式**：`status = "xxx";`xxx可描述为：

​			①**okey**：表示设备是可操作的。

​			②**disabled**：表示当前不可操作，但未来可更改为可操作性，如：热插拔设备

​			③**fail、failed**：表示有严重错误，几乎不可能再可操作了

​		**<2>作用**：

​			①<u>status描述设备信息状态</u>，在设备树文件中可以根据需求设置模块的状态，功能就是开启/关闭某个模块；

​			②在dtsi文件中，默认都是关闭模块的，在开发板对应的dts文件中自己去打开需要的模块；

​		**<3>举例**：

### 2.2.4. #address-cells和#size-cells属性

​		**<1>格式**：

```
			#address-cells = <1>;  #size-cells = <0>;
			//表示该结点属性起始地址所占字长为1，地址长度所占字长为0.相当于设置起始地址没有设置地址长度。
			//cell = 32位的整型。
```

​		**<2>作用**：

​			①`#address-cells`属性值决定了子节点reg属性中**地址信息所占用的字长**（32位），即：  
 				定义了该节点的子节点中的reg属性该用几个u32类型的数字编码其地址字段。  
​			②`			#size-cells`属性值决定了子节点reg属性中**长度信息所占的字长(**32 位)   
​				定义了该节点的子节点中的reg属性的size字段需要使用的u32类型的数字的数量  
​			③该属性可用于任何在设备树层次结构中拥有子节点的设备节点，并描述子设备节点应该如何被寻址。  
​			④这两个属性值都不从父节点那继承，需明确的定义。

​		**<3>举例**：

```
soc {
    #address-cells = <1>;
    #size-cells = <1>;
    serial {
        compatible = "ns16550";
        reg = <0x4600 0x100>;
        clock-frequency = <0>;
        interrupts = <0xA 0x8>;
        interrupt-parent = <&ipic>;
    };
};
```

### 2.2.5. reg属性

​		**<1>格式**：`reg = <address1 length1 address2 length2 …>; `

​						address一般用来表示起始地址，length一般表示持续长度；

​		**<2>作用**：*<u>配置某个硬件模块对应的地址范围信息</u>* ,即：  
​				 描述了在父总线定义的地址空间中设备资源的地址。通常表示寄存器映射在内存中的起始地址和长度或CPU寄存器地址的真实地址。

​    			当寄存器在内存中有两段地址时，reg需要指定两对#address-cells和#size-cells

​		**<3>举例**：			

```
reg = <0x3000 0x20 0xFE00 0x100>;
```

### 2.2.6. phandle 属性

​		**<1>格式**：`phandle = <num>`

​		**<2>作用**：

​			指定一个设备树中唯一的数字标识。带有phandle属性的节点可以被其他节点引用

​		**<3>举例**：

```
pic@10000000 {
	phandle = <1>;
	interrupt-controller;
};

another-device-node {
    interrupt-parent = <1>;
};
```

## 2.3. 结点类型

### 2.3.1. 根节点（/）

​		`/{}`，表示根节点，理论只有一个根节点，由此开始描述设备信息

### 2.3.2. aliases节点

​		aliases节点是用来定义一个设备节点的别名的，aliases节点应该在设备树的根节点中使用。

​		aliases的属性名是需要指定的别名，属性值是设备树中某个节点的完整路径，示例如下:

```
aliases {
    serial0 = "/simple-bus@fe000000/serial@llc500";
    ethernet0 = "/simple-bus@fe000000/ethernet@31c000";
};
```

### 2.3.3. memory节点

​		memory节点是用于描述系统的物理内存的的布局的，若系统有多个地址范围的内存，则多个memory节点需要创建，或者在reg节点中指定多个范围。示例如下：

```
memory@0 {
    device_type = "memory";
    reg = <0x000000000 0x00000000 0x00000000 0x80000000
           0x000000001 0x00000000 0x00000001 0x00000000>;
};
```

### 2.3.4.chosen节点

​		chosen节点不用来描述一个真实的设备，而是描述设备的启动和运行参数，例如bootargs，示例如下：

```
chosen {
    bootargs = "root=/dev/nfs rw nfsroot=192.168.1.1 console=ttyS0,115200";
};
```

## 2.4. dts格式示例：

```
/dts-v1/;
#include "mt7981.dtsi"
#include <dt-bindings/input/input.h>
#include <dt-bindings/gpio/gpio.h>
/ {
	model = "MediaTek MT7981 RFB";
	compatible = "mediatek,mt7981-spim-nor-rfb";

	gpio-keys {
		compatible = "gpio-keys";
		#address-cells = <1>;
		#size-cells = <0>;
		reset {
			label = "reset";
			gpios = <&pio 1 1>;
			linux,code = <KEY_RESTART>;
		};
	};
	
	leds-color {
		compatible = "color-leds";
		sysled {
			label = "sysled";
			red-gpios = <&pio 5 1>;
			green-gpios = <&pio 4 1>;
			default-color = "green";
			linux,default-trigger = "timer";
		};
	};
	
	chosen {
		bootargs = "console=ttyS0,115200n1 loglevel=8  \
				earlycon=uart8250,mmio32,0x11002000";
	};
	
	gsw: gsw@0 {
		compatible = "mediatek,mt753x";
		mediatek,ethsys = <&ethsys>;
		#address-cells = <1>;
		#size-cells = <0>;
	};
	
	memory {
		// fpga ddr2: 128MB*2
		reg = <0 0x40000000 0 0x10000000>;
	};

};
```

# 3. 设备树文档

### 3.1. 引入：

​		设备树中的**节点和属性具体是如何来描述设备的硬件细节的**，一般需要文档来进行讲解，文档的后缀名一般为.txt。在这个.txt文件中，需要描述对应**节点的兼容性**、**必需的属性**和**可选的属性**。

### 3.2. 文档路径：

​			路径：`/build_dir/target-aarch64_cortex-a53_musl/linux-mediatek_h30m/linux-5.4.194/Documentation/devicetree/bindings`

### 3.3. 文档检查：

​		Linux内核下的`scripts/checkpatch.pl`会运行一个检查，如果有人在设备树中新添加了compatible字符串，而没有添加相应的文档进行解释，checkpatch程序会报出警告：UNDOCUMENTED_DT_STRINGDT compatible string xxx appears un-documented，因此程序员要养成及时写DT Binding文档的习惯。

### 3.4. 文档内容：

​		①最基本描述。

​		②必需属性描述。

​		③可选属性描述。

​		④实例。

​		（2）

​		（3）

​		（4）

​		（5）

​				

​		









