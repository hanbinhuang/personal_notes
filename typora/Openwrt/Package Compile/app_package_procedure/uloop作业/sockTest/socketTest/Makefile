include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PRE_NAME:=netmng
COM_NAME:=socketTest

PKG_NAME:=$(PRE_NAME)-$(COM_NAME)
PKG_RELEASE:=1.0


PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)_$(PKG_RELEASE)

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  CATEGORY:=Ruijie modules
  SUBMENU:= Device management modules
  TITLE:=Ruijie socketTest configuration
  DEPENDS:= +libubox
endef

define Package/$(PKG_NAME)/description
    Device socketTest configuration
endef


define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)"\
		CROSS_COMPILE="$(TARGET_CROSS)"\
		DESTDIR="$(PKG_INSTALL_DIR)"
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/usr/sbin/

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/server $(1)/usr/sbin/server
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/client $(1)/usr/sbin/client
	$(INSTALL_BIN) ./files/usr/sbin/* $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
