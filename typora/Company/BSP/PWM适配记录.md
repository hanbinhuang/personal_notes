

参考网址：

1.[MTK pwm beeper配置记录](https://blog.csdn.net/hclydao/article/details/126749935?spm=1001.2101.3001.6650.7&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-7-126749935-blog-52673098.pc_relevant_3mothn_strategy_and_data_recovery&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-7-126749935-blog-52673098.pc_relevant_3mothn_strategy_and_data_recovery&utm_relevant_index=8)

2.[linux 驱动之PWM蜂鸣器驱动](https://blog.csdn.net/shenlong1356/article/details/105913224?spm=1001.2101.3001.6661.1&utm_medium=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-105913224-blog-101179621.pc_relevant_3mothn_strategy_recovery&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-105913224-blog-101179621.pc_relevant_3mothn_strategy_recovery)



# 1. PWM

## 1.1. pwm概念

​		pwm = pulse width modulation = 脉冲宽度调制

​		一个输出脉冲宽度可以调整的硬件器件，它不仅脉冲宽度可调，频率也可以调整，它的核心部件是一个硬件定时器。

​		作用：常用来调节电机转速，LED灯亮度，蜂鸣器音频等。

## 1.2. 参数

### 1.2.1. 周期

​		一个PWM高低电平的时间。

### 1.2.2. 频率

​		PWM的频率决定了所模拟出来的模拟电平的平滑度，通俗的讲，就是逼真度。不同的模拟器件，对期待的频率是有要求的，因此需要具体情况具体对待。另外，人耳能感知的频率范围是20Hz~16KHz，因此要注意PWM的频率不要落在这个范围，否则可能会产生莫名其妙的噪声。

### 1.2.3. 占空比

​		占空比，一个脉冲周期内，高电平的时间与整个周期时间的比例。决定了一个周期内PWM信号高低的比率，进而决定了一个周期内的平均电压，也即所模拟的模拟电平的电平值。

### 1.2.4. 极性

​		简单的说，一个PWM信号的极性，决定了是高占空比的信号输出电平高，还是低占空比信号输出电平高。假设一个信号的占空比为100%，如果为正常极性，则输出电平最大，如果为翻转的极性，则输出电平为0。

### 1.2.5. 开关

控制PWM信号是否输出。





1.config-5.4.194配置宏

```
CONFIG_PWM=y
CONFIG_PWM_SYSFS=y
CONFIG_PWM_MEDIATEK=y
#CONFIG_INPUT_PWM_BEEPER is not set
```

2.mt7981.dtsi 芯片公共部分

文件路径：build_dir/target-aarch64_cortex-a53_musl/linux-mediatek_h30m/linux-5.4.194/arch/arm64/boot/dts/mediatek/mt7981.dtsi

	pwm: pwm@10048000 {
		compatible = "mediatek,mt7981-pwm";
		reg = <0 0x10048000 0 0x1000>;
		#pwm-cells = <2>;
		clocks = <&infracfg_ao CK_INFRA_PWM_STA>,
			 <&infracfg_ao CK_INFRA_PWM_HCK>,
			 <&infracfg_ao CK_INFRA_PWM1_CK>,
			 <&infracfg_ao CK_INFRA_PWM2_CK>,
			 <&infracfg_ao CK_INFRA_PWM3_CK>;
		clock-names = "top", "main", "pwm1", "pwm2", "pwm3";
	};

3.pwm驱动

文件路径：build_dir/target-aarch64_cortex-a53_musl/linux-mediatek_h30m/linux-5.4.194/drivers/pwm







4.PWM概念













5.API

  1: /* include/linux/pwm.h */

  2: 

  3: /* 4: * pwm_config - change a PWM device configuration

 5: */ 6: int pwm_config(struct pwm_device *pwm, int duty_ns, int period_ns); *

*7: 8: /* 9: * pwm_enable - start a PWM output toggling 

10: */ 11: int pwm_enable(struct pwm_device *pwm); *

*12: 13: /* 14: * pwm_disable - stop a PWM output toggling

15: */ 16: void pwm_disable(struct pwm_device *pwm); *

*17: 18: /* 19: * pwm_set_polarity - configure the polarity of a PWM signal

20: */ 21: int pwm_set_polarity(struct pwm_device *pwm, enum pwm_polarity polarity);