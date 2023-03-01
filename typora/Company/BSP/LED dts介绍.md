[TOC]



# 1.LED

## 1.1.简介：

文件路径：`build_dir/target-aarch64_cortex-a53_musl/linux-mediatek_h30m/linux-5.4.194/Documentation/devicetree/bindings/leds/common.txt`

通用LED属性。

## 1.2.节点属性



## 1.3.子结点属性

### 1.3.1. led-sources

**（1）格式：**

**（2）简介：**led所连接的设备电流输出列表。输出由必须定义的数字标识LED设备绑定文档中。

### 1.3.2. function

**（1）格式：**

**（2）简介：**使用``LED_FUNCTION_* `前缀定义之一从头文件：`include/dt-bindings/leds/common.h`

若没有匹配的`LED_FUNCTION`可用，请添加一个新的。

### 1.3.3.color

**（1）格式：**

**（2）简介：**使用``LED_COLOR_ID_* `前缀定义之一从头文件：`include/dt-bindings/leds/common.h`

若没有匹配的`LED_COLOR_ID`可用，请添加一个新的。

### 1.3.4.function-enumerator

**（1）格式：**

**（2）简介：**当多个实例时使用的整数需要具有相同功能的，不同之处仅在于序数。

### 1.3.5.label

**（1）格式：**

**（2）简介：**LED标签，若省略则标签取自节点名（不包括单位地址）。必须唯一表示一个设备，即不能为其他LED类设备分配相同的设备标签。（该属性已弃用）

### 1.3.6. default-state

**（1）格式：**`default-state:on/off/keep`(打开/关闭/保持)

**（2）简介：**LED初始状态。若不存在该属性则默认为关闭。

### 1.3.7.linux,default-trigger

**（1）格式：**`default-trigger`

"backlight"：LED将充当背光，由帧缓冲区控制系统。

 "default-on" ：默认打开（对于LED gpio请参考：Documentation/devictree/bindings/leds/leds-gpio.txt）

"heartbeat"：LED“双”以基于负载平均值的速率闪烁

 "disk-activity"：LED指示磁盘活动

 "ide-disk"：LED指示ide磁盘活动（已弃用）

"timer"：LED以固定、可配置的速率闪烁

"pattern"：LED在指定的持续时间内改变亮度软件计时器（需要“led-pattern”属性）

...

**（2）简介：**若存在则定义分配给LED的触发器的字符串。

### 1.3.8. led-pattern 

**（1）格式：**

**（2）简介：**

## 1.4.举例

```
#include <dt-bindings/leds/common.h>

led-controller@0 {
	compatible = "gpio-leds";

	led0 {
		function = LED_FUNCTION_STATUS;
		linux,default-trigger = "heartbeat";
		gpios = <&gpio0 0 GPIO_ACTIVE_HIGH>;
	};
	
	led1 {
		function = LED_FUNCTION_USB;
		gpios = <&gpio0 1 GPIO_ACTIVE_HIGH>;
		trigger-sources = <&ohci_port1>, <&ehci_port1>;
	};

};

led-controller@0 {
	compatible = "maxim,max77693-led";

	led {
		function = LED_FUNCTION_FLASH;
		color = <LED_COLOR_ID_WHITE>;
		led-sources = <0>, <1>;
		led-max-microamp = <50000>;
		flash-max-microamp = <320000>;
		flash-max-timeout-us = <500000>;
	};

};

led-controller@30 {
        compatible = "panasonic,an30259a";
        reg = <0x30>;
        #address-cells = <1>;
        #size-cells = <0>;

        led@1 {
    	reg = <1>;
    	linux,default-trigger = "heartbeat";
    	function = LED_FUNCTION_INDICATOR;
    	function-enumerator = <1>;
        };
    
        led@2 {
    	reg = <2>;
    	function = LED_FUNCTION_INDICATOR;
    	function-enumerator = <2>;
        };
    
        led@3 {
    	reg = <3>;
    	function = LED_FUNCTION_INDICATOR;
    	function-enumerator = <3>;
        };

};
```



# 2.PWM

## 2.1. 简介：

为设备指定PWM信息。

使用多个PWM设备的设备的驱动程序可以使用`pwm-names`属性来映射`pwm_get()`调用`pwms`属性给出的列表索引。

## 2.2. 结点属性

### 2.2.1. pwm-list 

**（1）格式：**`pwm-list ::= <single-pwm> [pwm-list]`

**（2）简介：**指定想要使用的PWM设备列表。

### 2.2.2. single-pwm

**（1）格式：**`single-pwm ::= <pwm-phandle> <pwm-specifier>`

**（2）简介：**

### 2.2.3. pwm-phandle

**（1）格式：**`pwm-phandle :`

**（2）简介：**pwm控制器节点

### 2.2.4.pwm-specifier

**（1）格式：**`pwm-specifier :`

**（2）简介：**指定给定pwm的#pwm-cells（特定控制器）



## 2.3. 举例

### 2.3.1. 描述基于PWM的背光设备。

```dts
pwm: pwm {
	#pwm-cells = <2>;
};

[...]

bl: backlight {
	pwms = <&pwm 0 5000000>;
	pwm-names = "backlight";
};
```

PS:

pwm-names是多余的，因为backlight这个名称无论如何都会被用作回退。

pwm-specifier 通常编码芯片相对pwm数和pwm以纳秒为单位的周期，可以编码多个标志（定义于<dt-bindings/pwm/pwm.h>）在第三cell 中：PWM_POLARITY_INVERTED：反转PWM信号极性。

### 2.3.2.反极性可选PWM

	bl: backlight {
		pwms = <&pwm 0 5000000 PWM_POLARITY_INVERTED>;
		pwm-names = "backlight";
	};

### 2.3.3.PWM控制节点

PWM控制器节点必须指定用于说明符使用`#pwm cells`属性

	pwm: pwm@7000a000 {
		compatible = "nvidia,tegra20-pwm";
		reg = <0x7000a000 0x100>;
		#pwm-cells = <2>;
	};



# 3.PWM-LED

## 3.1.简介

用于PWM控制LED

## 3.2. 结点属性

**（1）格式：**compatible :"pwm-leds"

**（2）简介：**每个LED表示为pwm LED器件的子节点。每个节点的名称表示相应LED的名称。

## 3.3. 子结点属性

### 3.2.1. pwms

**（1）格式：**pwms:<&phandle id period_ns>；

**（2）简介：**

​		①每个pwms的确切含义属性必须记录在每个设备的设备树绑定中。

​		②指向PWM设备（phandle）/端口（id）和指定要使用的时段时间。

### 3.2.2. pwm-names(可选)

**（1）格式：**

**（2）简介：**

​		①可能包含要标记的字符串列表，``pwms`属性中列出每个PWM设备，若没有`pwm-names`属性则用户节点名将用作回退。

​		②pwm子系统用于pwm设备的名称有关pwms和pwm名称属性。

### 3.2.3. max-brightness

**（1）格式：**

**（2）简介：**	LED可能的最大亮度



### 3.2.3.active-low(可选)

**（1）格式：**

**（2）简介：**	用于表示LED连接的是正极而不是负极。

### 3.2.4.label (可选)

**（1）格式：**

**（2）简介：**	

### 3.2.5.linux,default-trigger

**（1）格式：**

**（2）简介：**	

## 3.3.举例

```
twl_pwm: pwm {
	/* provides two PWMs (id 0, 1 for PWM1 and PWM2) */
	compatible = "ti,twl6030-pwm";
	#pwm-cells = <2>;
};

twl_pwmled: pwmled {
	/* provides one PWM (id 0 for Charing indicator LED) */
	compatible = "ti,twl6030-pwmled";
	#pwm-cells = <2>;
};

pwmleds {
	compatible = "pwm-leds";
	kpad {
		label = "omap4::keypad";
		pwms = <&twl_pwm 0 7812500>;
		max-brightness = <127>;
	};

	charging {
		label = "omap4:green:chrg";
		pwms = <&twl_pwmled 0 7812500>;
		max-brightness = <255>;
	};

};
```



**（1）格式：**

**（2）简介：**	