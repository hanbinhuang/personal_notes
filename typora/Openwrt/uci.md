[TOC]







# 1. 简介

​		uci = Unified Configuration Interface = 统一配置接口  
​		配置文件默认路径：`/etc/config `  
​		可通过文本编辑器、uci（一个可执行程序）以及各种 API（Shell、Lua 和C）来修改这些配置文件。  

## 1.1. 基本语法：

```
config <type> ["<name>"] 	# Section
	option <name> "<value>" # Option	
	list <name> "<value1>"  # list
	list <name> "<value2>"	# list

PS:
	<xxx>:表示必须要有
	[xxx]:表示不一定要有
	type：类型标识,若对应的name为空则表示该配置节为匿名配置节。
		name的目的是：方便后面继续使用，若为匿名则使用@type[num],num=0,1..表示。
```

​		文件由配置节(section)组成，配置节由多个**选项对**或l**列表**组成。  
​				①选项对：名值对结构。  
​				②列表：一个名多值结构。













# 2. 命令行接口

​		**查看帮助**：	`uci --help`:  
​		**语法**：`uci [<options>] <command> [<arguments>]`  
​		**举例：**` uci -q show sysinfo`

## 2.1. command

​		详细示例：[uci](https://blog.csdn.net/hzlarm/article/details/102993291)

| 命令                                             | 功能                                                         | 举例 |
| ------------------------------------------------ | :----------------------------------------------------------- | ---- |
| batch                                            | 批处理uci命令，需把每条命令前的字符串”uci”去掉               |      |
| export     [<config>]                            | 显示UCI可读取到的某配置文件的信息<br />(包括已修改但未提交内容)。 |      |
| import     [<config>]                            | 以uci语法导入配置文件                                        |      |
| changes    [<config>]                            | 列出配置文件未提交的内容<br />若未指定配置文件，则导出所有配置文件的修改 |      |
| commit     [<config>]                            | 提交给定的配置文件修改内容，<br />若未指定参数则将所有的配置文件写入文件系统 |      |
| add        <config> <section-type>               | 增加指定配置文件的类型为section-type的匿名区段               |      |
| add_list   <config>.<section>.<option>=<string>  | 对已存在的list选项增加字符串                                 |      |
| del_list   <config>.<section>.<option>=<string>  | 删除已存在的list选项的字符串                                 |      |
| show       [<config>[.<section>[.<option>]]]     | 显示指定的选项、配置节或配置文件                             |      |
| get        <config>.<section>[.<option>]         | 获取指定区段选项的值                                         |      |
| set        <config>.<section>[.<option>]=<value> | 设置指定配置节选项的值，<br />或增加一个配置节，类型设置为指定的值 |      |
| delete     <config>[.<section>[[.<option>][=]]]  | 删除指定配置节或选项                                         |      |
| rename     <config>.<section>[.<option>]=<name>  | 对指定的选项或配置节重命名                                   |      |
| revert     <config>[.<section>[.<option>]]       | 恢复指定的选项，配置节或配置文件                             |      |
| reorder    <config>.<section>=<position>         | 修改配置项config在文件中的位置；<br />优先级0最大，数字越大越排后 |      |

## 2.2.options

| 选项      | 功能                                                | 举例 |
| :-------- | --------------------------------------------------- | ---- |
| -c <path> | 设置当前命令配置文件的搜索路径（默认：/etc/config） |      |
| -d <str>  | 设置uci show 中列表值的分隔符                       |      |
| -f <file> | 使用某文件作为替代stdin作为输入                     |      |
| -m        | 导入时，将数据合并到现有包中                        |      |
| -n        | 导出时命名未命名的节（默认）                        |      |
| -N        | 不要命名未命名的节                                  |      |
| -p <path> | 为配置更改文件添加搜索路径                          |      |
| -P <path> | 为配置更改文件添加搜索路径并将其用作默认值          |      |
| -q        | 静默模式（不打印任何错误信息）                      |      |
| -s        | 强制严格模式（解析器错误时停止，默认）              |      |
| -S        | 禁用严格模式                                        |      |
| -X        | 不要在“show”上使用扩展语法                          |      |



# 3. shell接口

## 3.1. 调用前提

​		（1）调用uci shell接口时，需加载`/lib/function.sh`，格式如下：  

```shell
#!/bin/sh
. /lib/functions.sh    
```

​		（2）启机脚本通常包含`#!/bin/sh /etc/rc.common,` `/etc/rc.common `包含了 `/lib/functions.sh`

## 3.2. uci_xxx 与config_xxx

​		"uci_"开头的函数和以“config_”开头的函数大多数功能完全相同，  
​		唯一不同的是`uci_get`等函数直接从文件中获取，`config_get`函数从环境变量中读取

### 3.2.1. uci_开头函数

| 函数             | 功能                                  |
| ---------------- | ------------------------------------- |
| uci_load         | 从uci文件中加载配置并设置到环境变量中 |
| uci_set_default  |                                       |
| uci_revert_state |                                       |
| uci_set_state    |                                       |
| uci_toggle_state |                                       |
| uci_set          |                                       |
| uci_get_state    | 指定从/var/state中获取状态值          |
| uci_get          | 从uci文件中获取值                     |
| uci_add          |                                       |
| uci_rename       |                                       |
| uci_remove       |                                       |
| uci_commit       |                                       |



### 3.2.2. confIg_开头函数





# 4.Lua接口





# 5. C接口