include $(TOPDIR)/rules.mk# 包含顶层目录rule.mk

PRE_NAME:=std# 前缀名
COM_NAME:=test# 合成名
PKG_NAME:=$(PRE_NAME)_$(COM_NAME)# 软件包名称=前缀名+合成名,可通过menuconfig和ipkg查看到
PKG_VERSION:=1.0# 上游软件的版本号
PKG_RELEASE:=1# Makefile的版本号

include $(INCLUDE_DIR)/package.mk# 软件包基本信息配置完成再引入

####  "Package/" 定义用于① “make menuconfig”选择，②编译生成软件包（调用BuildPackage） ####
####  软件包定义用于：①编译前的软件包选择，②编译后的ipkg安装包生成。  ####
##  软件包在menuconfig和ipkg中的描述信息  ##
# SECTION： 软件包的类型，如：network、Utilities等
# CATEGORY：在menuconfig中显示到菜单分类（一级目录）
# SUBMENU： CATEGORY下的子分类（二级目录）
# TITLE：   标题，软件包的简短描述
# DEPENDS：	依赖项，本软件包之前编译和安装的软件包
define Package/$(PKG_NAME)
  SECTION:=Utilities
  CATEGORY:=Ruijie modules
  SUBMENU:=Device management modules
  TITLE:=std test package
  DEPENDS:=+libc +libuci +libdebug +libubox +ubus +libjson-c
endef

##  软件的详细描述  ##
define Package/$(PKG_NAME)/description
  This package contains the $(PKG_NAME) programs.
endef

##  软件包运行配置文件列表，一个文件占一行  ##
define Package/$(PKG_NAME)/conffiles
  /etc/config/$(PKG_NAME)
endef

####  "Build/" 定义用于代码编译（可选）  ####
####  软件包模块的编译步骤：准备，配置，编译，安装  ####
####  编译选项在“package-defaults.mk”文件中定义，可在软件包的Makefile修改  ####
##  一组用于解包，打补丁的命令  ##
define Build/Prepare
	$(call Build/Prepare/Default)
	mkdir -p $(PKG_BUILD_DIR)
	mkdir -p $(PKG_BUILD_DIR)/$(PKG_NAME)
	$(CP) -r ./src/* $(PKG_BUILD_DIR)/$(PKG_NAME)
	$(INSTALL_DIR) $(PKG_INSTALL_DIR)/usr/include/$(PKG_NAME)
endef

# 可使用$(call Build/Compile/Default,FOO=bar)传递特定参数传递给make
define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/$(PKG_NAME) \
		CC="$(TARGET_CC)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		LIBS="$(TARGET_LDFLAGS)"
endef

#安装之前实际执行的脚本，如：启动程序（不要忘了包含#!/bin/sh）
define Package/$(PKG_NAME)/preinst
	#!/bin/sh
	if [ -f /etc/init.d/$(PKG_NAME) ]; then
	    /etc/init.d/$(PKG_NAME) stop
	fi
	exit 0
endef

#安装完成后执行的脚本，如：停止程序（不要忘了包含#!/bin/sh）
#若需终止删除则返回false
define Package/$(PKG_NAME)/postinst
	#!/bin/sh
	/etc/init.d/$(PKG_NAME) start
	exit 0
endef

##  软件的安装操作  ##
# $(1)：ipkg的目录，在源代码中使用的相对目录，
# 编译生成的安装文件由$(PKG_INSTALL_DIR)目录下复制到ipkg的目录下
# ● INSTALL_DIR 创建目录，并设置目录权限。
# ● INSTALL_DATA 安装数据文件，即复制并设置权限为 0644。
# ● INSTALL_CONF 安装配置文件，即复制并设置权限为 0600。
# ● INSTALL_BIN 安装可执行程序，即复制并增加执行权限，设置权限表示为 0777。
define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) $(PKG_BUILD_DIR)/$(PKG_NAME)/build/$(PKG_NAME).elf $(1)/usr/sbin/

endef

$(eval $(call BuildPackage,$(PKG_NAME)))
