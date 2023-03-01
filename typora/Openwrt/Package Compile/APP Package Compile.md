```makefile
(1)引入文件
include $(TOPDIR)/rules.mk# 包含顶层目录rule.mk
include $(INCLUDE_DIR)/kernel.mk# 软件包为内核时不可缺少
include $(INCLUDE_DIR)/package.mk# 软件包基本信息配置完成再引入

(2)软件包基本信息
PRE_NAME:=# 前缀名
COM_NAME:=# 合成名
PKG_NAME:=$(PRE_NAME)-$(COM_NAME)# 软件包名称=前缀名+合成名,可通过menuconfig和ipkg查看到
PKG_VERSION:=# 上游软件的版本号
PKG_RELEASE:=# Makefile的版本号
PKG_SOURCE:=#源代码文件名
PKG_SOURCE_URL:=#源代码下载网址
PKG_MD5SUM:=#源代码文件校验值，用于核对软件包是否正确下载
PKG_CAT:=#源代码文件加压方法,如：zcat、bzcat、unzip等
PKG_BUILD_DIR:=#软件包编译目录，其父目录$(BUILD_DIR),若不指定则默认为$(BUILD_DIR)/$(PKG_NAME)$(PKG_VERSION)

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)_$(PKG_VERSION)# 软件包的编译目录


(3)软件包的定义
用户程序用Package.
内核模块用KernelPackage.
####  "Package/" 定义用于① “make menuconfig”选择，②编译生成软件包（调用BuildPackage） ####
####  软件包定义用于：①编译前的软件包选择，②编译后的ipkg安装包生成。  ####
##  软件包在menuconfig和ipkg中的描述信息  ##
# SECTION： 软件包的类型，如：network、Utilities等
# CATEGORY：在menuconfig中显示到菜单分类（一级目录）
# SUBMENU： CATEGORY下的子分类（二级目录）
# TITLE：   标题，软件包的简短描述
# DEPENDS：	依赖项，本软件包之前编译和安装的软件包   

define Package/$(PKG_NAME)
  CATEGORY:=Test modules					
  SUBMENU:=Socket modules
  TITLE:=socketTest configuration
  DEPENDS:= +libubox
endef

##  软件的详细描述  ##
define Package/$(PKG_NAME)/description
    Device socketTest configuration
endef

##  软件的安装操作  ##
# $(1)：ipkg的目录，在源代码中使用的相对目录，
# 编译生成的安装文件由$(PKG_INSTALL_DIR)目录下复制到ipkg的目录下
# ● INSTALL_DIR 创建目录，并设置目录权限。
# ● INSTALL_DATA 安装数据文件，即复制并设置权限为 0644。
# ● INSTALL_CONF 安装配置文件，即复制并设置权限为 0600。
# ● INSTALL_BIN 安装可执行程序，即复制并增加执行权限，设置权限表示为 0777。
define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/usr/sbin/

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/server $(1)/usr/sbin/server
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/client $(1)/usr/sbin/client
	$(INSTALL_BIN) ./files/usr/sbin/* $(1)/usr/sbin/

endef

####  "Build/" 定义用于代码编译（可选）  ####
####  软件包模块的编译步骤：准备，配置，编译，安装  ####
####  编译选项在“package-defaults.mk”文件中定义，可在软件包的Makefile修改  ####
##  一组用于解包，打补丁的命令  ##
define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

##  若源码无需configure生成Makefile则无需该部分  ##
# 可使用$(call Build/Configure/Default,FOO=bar)传递额外参数传递给configure脚本
define Build/Configure
endef

##  编译源代码  ##

# 可使用$(call Build/Compile/Default,FOO=bar)传递特定参数传递给make
define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)"\
		CROSS_COMPILE="$(TARGET_CROSS)"\
		DESTDIR="$(PKG_INSTALL_DIR)"
endef

##  安装编译后的文件 ##
# 默认调用make install
# 可使用$(call Build/Install/Default,install install-foo)
# 注意：传递所有的参数需增加在install参数后，不要忘“install”参数
define Build/Install
endef

# BuildPackage是在包含头文件“include/package.mk”中定义的，BuildPackage仅需一个直接参数——软件包名

$(eval $(call BuildPackage,$(PKG_NAME)))
```

