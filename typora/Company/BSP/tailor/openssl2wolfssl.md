



[TOC]



# 1.配置宏

## 1.1. openssl的配置宏

```Makefile
关闭：
# CONFIG_PACKAGE_libopenssl is not set
# CONFIG_OPENSSL_WITH_EC is not set
# CONFIG_OPENSSL_WITH_DEPRECATED is not set
# CONFIG_OPENSSL_WITH_NPN is not set
# CONFIG_OPENSSL_WITH_PSK is not set
# CONFIG_OPENSSL_WITH_SRP is not set
若还有其他选项也可以关闭
```

## 1.2. wolfssl的配置宏

```makefile
开启：
CONFIG_PACKAGE_libwolfssl=y
CONFIG_WOLFSSL_HAS_AES_CCM=y
CONFIG_WOLFSSL_HAS_DH=y
CONFIG_WOLFSSL_HAS_ARC4=y
CONFIG_WOLFSSL_HAS_CERTGEN=y
CONFIG_WOLFSSL_HAS_TLSV10=y
CONFIG_WOLFSSL_HAS_TLSV13=y
CONFIG_WOLFSSL_HAS_SESSION_TICKET=y
CONFIG_WOLFSSL_HAS_NO_HW=y
未开启：
# CONFIG_WOLFSSL_HAS_CHACHA_POLY is not set
# CONFIG_WOLFSSL_HAS_DTLS is not set
# CONFIG_WOLFSSL_HAS_OCSP is not set
# CONFIG_WOLFSSL_HAS_WPAS is not set
# CONFIG_WOLFSSL_HAS_ECC25519 is not set
# CONFIG_WOLFSSL_HAS_OPENVPN is not set
# CONFIG_WOLFSSL_ALT_NAMES is not set
# CONFIG_WOLFSSL_HAS_AFALG is not set
# CONFIG_WOLFSSL_HAS_DEVCRYPTO_CBC is not set
# CONFIG_WOLFSSL_HAS_DEVCRYPTO_AES is not set
# CONFIG_WOLFSSL_HAS_DEVCRYPTO_FULL is not set

```

## 1.3. libcurl库的配置宏

```makefile
开启：
CONFIG_LIBCURL_WOLFSSL=y
关闭：
# CONFIG_LIBCURL_OPENSSL is not set
```

## 1.4.mosquitto库的配置宏：

```Makefile
开启：
CONFIG_PACKAGE_libmosquitto-wolfssl=y
CONFIG_PACKAGE_mosquitto-client-wolfssl=y
CONFIG_PACKAGE_mosquitto-wolfssl=y
CONFIG_MOSQUITO_CYASSL=y
关闭：
# CONFIG_PACKAGE_libmosquitto-ssl is not set
# CONFIG_PACKAGE_mosquitto-client-ssl is not set
# CONFIG_PACKAGE_mosquitto-ssl is not set
[LWS=LibWebsocketS，故被libwebsockets依赖]



lua加密
关闭：
# CONFIG_PACKAGE_luacrypto is not set
开启：
CONFIG_PACKAGE_luawolfssl=y
CONFIG_LUACRYPTO_CYASSL=y


```

## 1.5. libwebsockets的配置宏

```Makefile
开启：
CONFIG_PACKAGE_libwebsockets-wolfssl=y
CONFIG_LIBWEBSOCKETS_CYASSL=y
关闭：
# CONFIG_PACKAGE_libwebsockets-openssl is not set
```

## 1.6. lighttpd的配置宏

```makefile
开启：
CONFIG_LIGHTTPD_CYASSL=y
```

## 1.7. easydisc的配置宏

```Makefile
开启：
CONFIG_EASYDISC_CYASSL=y
```

## 1.8. rg_pnpd的配置宏

```Makefile
开启：
CONFIG_RG_PNPD_CYASSL=y
```

## 1.9. libbal的配置宏

```makefile
开启：
CONFIG_LIBBAL_CYASSL=y
```

## 1.10. rg_crypto的配置宏

```makefile
开启：
CONFIG_RG_CRYPTO_CYASSL=y
```

## 1.11. app_identify 的配置宏

```makefile
开启：
CONFIG_APP_IDENTIFY_CYASSL=y
```

## 1.12. route_db的配置宏

```makefile
开启：
CONFIG_ROUTE_DB_CYASSL=y
```

## 1.13. ROAM的配置宏

```makefile
开启：
CONFIG_ROAM_CYASSL=y
```

## 1.14.factory的配置宏

```makefile
开启：
CONFIG_FACTORY_CYASSL=y
```

## 1.15. 总结

​		所有组件有使用到加密组件一般都有依赖openssl。

# 2. 报错信息修订

## 2.1. 缺少mosquitto.h

​		`cloud_mqlink-1.0.0/src/message.c`中包含的`mosq.h`包含了`<mosquitto.h>`文件，但实际缺少该文件。

```bash
In file included from message.c:17:0:
mosq.h:12:10: fatal error: mosquitto.h: No such file or directory
 #include <mosquitto.h>
          ^~~~~~~~~~~~~
compilation terminated.
Makefile:38: recipe for target 'message.o' failed
make[4]: *** [message.o] Error 1
make[4]: Leaving directory '/code/build_dir/target-aarch64_cortex-a53_musl/cloud_mqlink-1.0.0/src'
Makefile:154: recipe for target '/code/build_dir/target-aarch64_cortex-a53_musl/cloud_mqlink-1.0.0/.built' failed
make[3]: *** [/code/build_dir/target-aarch64_cortex-a53_musl/cloud_mqlink-1.0.0/.built] Error 2
make[3]: Leaving directory '/code/feeds/devmng/cloud/mqlink'
time: package/feeds/devmng/cloud/mqlink/compile#0.22#0.14#0.34
package/Makefile:111: recipe for target 'package/feeds/devmng/cloud/mqlink/compile' failed
```

​		根据wifi5产品可知：`./mosquitto-wolfssl/mosquitto-1.5.9/lib/mosquitto.h`,该头文件是在mosquitto库中包含，查看wifi5中mosquitto的Makefile：`$(eval $(call BuildPackage,mosquitto-wolfssl))`   
​		查看wifi6产品的mosquitto库发现根本没有该部分的内容，直接将该库进行覆盖。   

## 2.2. 动态库缺少有关SSL的定义

```bash
_dir/toolchain-aarch64_cortex-a53_gcc-7.3.0_musl/usr/lib -L/code/staging_dir/toolchain-aarch64_cortex-a53_gcc-7.3.0_musl/lib -znow -zrelro  -L../lib ../lib/libmosquitto.so.1
......
../lib/libmosquitto.so.1: undefined reference to `SSL_CTX_load_verify_locations'
../lib/libmosquitto.so.1: undefined reference to `SSL_get_ex_data_X509_STORE_CTX_idx'
../lib/libmosquitto.so.1: undefined reference to `SSL_CTX_free'
../lib/libmosquitto.so.1: undefined reference to `SSL_CTX_set_default_passwd_cb_userdata'
../lib/libmosquitto.so.1: undefined reference to `sk_value'
../lib/libmosquitto.so.1: undefined reference to `SSL_CTX_set_cipher_list'
../lib/libmosquitto.so.1: undefined reference to `SSL_connect'
../lib/libmosquitto.so.1: undefined reference to `X509_get_ext_d2i'
../lib/libmosquitto.so.1: undefined reference to `GENERAL_NAME_free'
../lib/libmosquitto.so.1: undefined reference to `X509_get_subject_name'
collect2: error: ld returned 1 exit status
Makefile:30: recipe for target 'mosquitto_pub' failed
make[4]: *** [mosquitto_pub] Error 1
make[4]: Leaving directory '/code/build_dir/target-aarch64_cortex-a53_musl/mosquitto-ssl/mosquitto-1.5.9/client'
Makefile:26: recipe for target 'mosquitto' failed
make[3]: *** [mosquitto] Error 2
make[3]: Leaving directory '/code/build_dir/target-aarch64_cortex-a53_musl/mosquitto-ssl/mosquitto-1.5.9'
```

​		根据报错信息可知：在编译mosquitto-client-wolfssl包的时出现报错，该包又依赖libmosquitto-wolfssl，又依赖libwolfssl，又因为报错信息大部分内容是与SSL有关，故怀疑wolfssl内容有问题，将wifi5的wolfssl进行覆盖。

​		并且单独编译wolfssl能够成功编译：`make ./package/feeds/packages18_06/wolfssl/{clean,compile} V=s`

## 2.3. 若遇到编译wolfssl但直接编译的仍为openssl的目录

​		此时再单独编译mosquitto,发现能够编译成功，但编译目录是mosquitto.ssl，大概是因为还有其他组件会依赖ssl（libwebsocket库），故先将mosquitto中的包调用ssl先屏蔽掉，再次编程成功为wolf目录（或者先将其他的依赖openssl的相关组件先编译成功则不会有该问题）。

## 2.4. 若遇见某组件依赖openssl则修改为依赖wolfssl

​		依赖openssl的组件一般就是在链接库中会去链接两个库：`-lssl -lcrypto`其中一个。 此时只需将原本的链接库变量中的这两个库删除，并重新定义一个配置宏用来区分是openssl还是wolfssl，常见的操作如下：

### 2.4.1. 源文件中的Makefile

```makefile
LIBS = -lpthread -ljson-c -lrt -luci -lubus -lubox -lssl -lcrypto -ldebug


修改为：
LIBS = -lpthread -ljson-c -lrt -luci -lubus -lubox -ldebug

ifeq ($(RG_PNPD_CYASSL),1)
	CFLAGS+= -DHAVE_WOLFSSL -DOPENSSL_EXTRA
	LIBS += -lwolfssl
else
	CFLAGS+= -DHAVE_OPENSSL
	LIBS += -lssl -lcrypto
endif

===》wolfssl相当于ssl与crypto
```

### 2.4.2. 源文件中的头文件

```c
#include <openssl/crypto.h>


修改为
#ifdef HAVE_WOLFSSL
#include <wolfssl/openssl/crypto.h>
#endif

#ifdef HAVE_OPENSSL
#include <openssl/crypto.h>
#endif
```

### 2.4.3.组件中的Makefile

```makefile
包的依赖：
  DEPENDS:=+libuci +libubus +libjson-c +libubox +libpthread +libopenssl +libdebug +librt
  
  
修改为：
  DEPENDS:=+libuci +libubus +libjson-c +libubox +libpthread +!RG_PNPD_CYASSL:libopenssl +RG_PNPD_CYASSL:libwolfssl +libdebug +librt
  
ifeq ($(CONFIG_RG_PNPD_CYASSL),y)
	export RG_PNPD_CYASSL=1
endif

===》故对应组件的配置宏需要加上CONFIG_RG_PNPD_CYASSL
```



















 



​		

