[TOC]

[文章](https://blog.csdn.net/weixin_44903006/article/details/116978697?spm=1001.2101.3001.6650.8&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-8-116978697-blog-119394897.pc_relevant_3mothn_strategy_recovery&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-8-116978697-blog-119394897.pc_relevant_3mothn_strategy_recovery&utm_relevant_index=9)



#    1. 基础概念

## 1.1. 名词认识

**①设备（device）**：描述硬件资源，抽象系统中所有的硬件设备，描述它的名字、属性、从属的Bus、从属的class等信息。

**②驱动（driver）**：驱动硬件资源的驱动程序，包含设备初始化、电源管理相关的接口实现。

**③总线（bus）**：CPU和一/多个设备之间，各个设备之间传递信息的公共通道。  

​		总线类型：①物理总线：USB/PCI/I2C/SPI总线 。②虚拟总线：platform总线

**④分类（class）**:在Linux设备模型中，主要集合具有相似功能或属性的设备，抽象出一套可多设备之间共用的数据结构和接口函数。因而从属于相同class的设备的驱动程序，就不再需要重复定义这些公共资源，直接从class中继承即可。

# 2. 平台驱动

​		从**Linux2.6**开始**Linux**加入了一套驱动管理和注册机制—**platform**平台总线驱动模型。与传统的**bus/device/driver**机制相比，**platform机制**将设备本身的资源注册进内核，由内核统一管理，在驱动程序使用这些资源时使用统一的接口，这样提高了程序的可移植性。所谓的**platform_device**并不是与字符设备、块设备和网络设备并列的概念，而是Linux系统提供的一种附加手段。

​		简单来说总线bus，驱动driver，设备device这三者之间的关系就是：驱动开发者可以通过总线bus来将驱动driver和设备device进行隔离，这样的好处就是开发者可以将相对稳定不变的驱动driver独立起来，可以通过总线bus来桥接与之匹配的设备device。设备device只需要提供与硬件相关的底层硬件的配置，如io,中断等（也就是**分离思想**，而**输入子系统是分层思想**）

 		platform.c提供了一个平台总线platform_bus,和注册平台设备platform_device和平台驱动platform_driver的相关接口，其中平台总线platform_bus已经编进内核，开发者只需要提供平台设备platform_device和平台驱动platform_driver的相关代码就行了。

## 2.1. 平台总线(platform bus)：  

​		一种虚拟总线， 但同SPI与I2C等物理总线一样，Linux通过系统总线来管理它们。  
 		将设备和驱动注册到虚拟总线上时，当驱动与设备相匹配后完成注册。  
​        设备和驱动的关系是多对一的关系，即：多个相同设备可使用一个驱动，驱动通过设备ID进行区分。  
​		通过各自的设备控制器直接与CPU连接，CPU可通过常规寻址操作访问设备的一种连接方式。

**总线类型结构体，如下：**

```c
//path:linux-5.4.194\include\linux\device.h
struct bus_type {
  const char    *name;						/* 总线名 */
  const char    *dev_name;					/* 用于子系统枚举设备，如（“foo%u”，dev->id） */
  struct device    *dev_root;				/* 用作父设备的默认设备 */
  const struct attribute_group **bus_groups;/* 总线的默认属性 */
  const struct attribute_group **dev_groups;/* 总线上设备的默认属性 */
  const struct attribute_group **drv_groups;/* 总线上设备驱动程序的默认属性 */
  
   /* 每当为此总线添加新设备或驱动程序时，可能会多次调用。
   	如果给定的驱动程序可以处理给定的设备，则返回正值，否则返回零。
   	如果无法确定驱动程序支持该设备，它也可能返回错误代码。
   	在EPROBE_DEFER的情况下，它会将设备排队等待延迟探测 */
  int (*match)(struct device *dev, struct device_driver *drv);	
   /* 在添加、删除设备或生成uevent以添加环境变量的其他一些事情时调用。 */
  int (*uevent)(struct device *dev, struct kobj_uevent_env *env);	
   /* 当新设备或驱动程序添加到此总线时调用，并回调特定驱动程序的探测以初始化匹配的设备 */
  int (*probe)(struct device *dev);		
   /* 当设备从此总线中删除时调用 */
  int (*remove)(struct device *dev);	
   /* 在关闭时调用以使设备静止 */
  void (*shutdown)(struct device *dev);	
  /* 调用以使设备重新联机（在脱机后） */
  int (*online)(struct device *dev);	
  /* 调用以使设备脱机以进行热删除。可能会失败 */
  int (*offline)(struct device *dev);		
  /* 当此总线上的设备要进入睡眠模式时调用 */
  int (*suspend)(struct device *dev, pm_message_t state);
  /* 调用以使此总线上的设备退出睡眠模式 */
  int (*resume)(struct device *dev);					
  
  /* 调用以确定此总线上的设备支持多少虚拟函数 */
  int (*num_vf)(struct device *dev);		
  /* 调用以在此总线上的设备上设置dma配置 */
  int (*dma_configure)(struct device *dev);	
  
  /* 此总线的电源管理操作，回调特定设备驱动程序的pm操作 */
  const struct dev_pm_ops *pm;				
  /* 此总线的iommu特定操作，用于将iommu驱动程序实现附加到总线，并允许驱动程序执行总线特定设置 */
  const struct iommu_ops *iommu_ops;		
  /* 驱动程序核心的私有数据，只有驱动程序核心才能接触到这个 */
  struct subsys_private *p;				
  /* 锁定类密钥以供锁验证器使用 */
  struct lock_class_key lock_key;			
  /* 探测或删除此总线上的设备时，设备核心应锁定设备的父级。 */
  bool need_parent_lock;					
};
```

**平台总线变量定义，如下：**

```c
//path:/linux-5.4.194/drivers/base/platform.c
struct bus_type platform_bus_type = {
  .name    = "platform",					//总线名
  .dev_groups = platform_dev_groups,		//总线上设备的默认属性
  .match    = platform_match,				//匹配设备和驱动,匹配成功就调用driver的.probe函数
  .uevent   = platform_uevent,				//消息传递,比如热插拔操作
  .dma_configure  = platform_dma_configure,	//设置dma配置
  .pm   = &platform_dev_pm_ops,				//电源管理
};
```

其中最重要的是总线的**匹配回调函数**（共四种匹配方式），具体如下：  

```c
/**
 * @function: platform_match-将平台设备绑定到平台驱动程序。
 *
 * 假设平台设备IDS编码如下：“＜name＞＜instance＞“，
 * ＜name＞是设备类型的简短描述，如"pci" or "floppy".
 * ＜instance＞是设备的枚举实例，如'0' or '42'.
 * 驱动程序IDS只是＜name＞。因此，从平台设备结构中提取＜name＞，并将其与驱动程序的名称进行比较。
 * 返回它们是否匹配
 */
static int platform_match(struct device *dev, struct device_driver *drv)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct platform_driver *pdrv = to_platform_driver(drv);

	/* 设置driver_override时，仅绑定到匹配的驱动程序 */
	if (pdev->driver_override)
		return !strcmp(pdev->driver_override, drv->name);
	/* 第一种：OF类型的匹配（设备树采用的匹配方式） */
	if (of_driver_match_device(dev, drv))
		return 1;
	/* 第二种：ACPI样式匹配 */
	if (acpi_driver_match_device(dev, drv))
		return 1;
	/* 第三种：id_table匹配 */
	if (pdrv->id_table)
		return platform_match_id(pdrv->id_table, pdev) != NULL;
	/* 第四种：比较驱动和设备的name字段，看看是不是相等（用的最多） */
	return (strcmp(pdev->name, drv->name) == 0);
}
```

## 2.2. 平台设备(platform device)

​		若内核支持设备树，则直接通过设备树进行描述，若不支持设备树则通过platform_device来描述。

**平台设备结构体(`platform_device`)，如下：**

```c
//path:linux-5.4.194\include\linux\platform_device.h
struct platform_device {
  const char  *name;	/* 设备名，第四种匹配方式就是通过该字段完成,用作sys/device下显示的目录名 */
  int   id;				/* 设备ID,用于给插入给该总线并具有相同name的设备编号，若只有一个设备则填-1 */
  bool     id_auto;				/*  */
  struct device  dev;			/*  */
  u64   platform_dma_mask;		/*  */
  u32   num_resources;			/* 表示resource结构体指针指向的资源大小 */
  struct resource *resource;	/* 设备资源结构体指针，指向设备信息 */

  const struct platform_device_id *id_entry; /*  */
  char *driver_override; 		/* Driver name to force a match */
    
  /* MFD cell pointer */
  struct mfd_cell *mfd_cell;
    
  /* arch specific additions */
  struct pdev_archdata   archdata;
};

```

**设备资源结构体(`resource`)**：  
		用于存放设备的资源信息，如IO地址、中断号等。定义如下：

```c
//path:linux-5.4.194\include\linux\ioport.h(用于检测、保留和分配系统资源的例程的定义)
struct resource {
  resource_size_t start;		/* 资源的起始地址 */
  resource_size_t end;			/* 资源的终止地址 */
  const char *name;
  unsigned long flags;			/* 资源的类型 */
  unsigned long desc;
  struct resource *parent, *sibling, *child;
};
```

​		`resource`即：设备信息，是树状的，允许嵌套等，如：外设寄存器等.

## 2.3. 平台驱动(platform driver)



**平台驱动结构体，如下：**

```c
//path:linux-5.4.194\include\linux\platform_device.h
struct platform_driver {
  /* 只要匹配成功则立刻执行该回调函数 */
  int (*probe)(struct platform_device *);
  /* 卸载platform驱动时执行该函数 */
  int (*remove)(struct platform_device *);
  /*  */
  void (*shutdown)(struct platform_device *);
  /*  */
  int (*suspend)(struct platform_device *, pm_message_t state);
  /*  */
  int (*resume)(struct platform_device *);
  /* 一个重要的类：设备驱动结构体变量 */
  struct device_driver driver;
  /*  第三种匹配方式：id_table匹配 */
  const struct platform_device_id *id_table;
  /*  */
  bool prevent_deferred_probe;
};
```

​		此处的`driver`变量定义类似与C++的继承特性，即`platform_driver`结构体继承了`device_driver`结构体的成员变量.

**设备驱动结构体，如下**：

```c
struct device_driver {
  const char    *name;			/* 设备驱动名 */		
  struct bus_type   *bus;		/* 该驱动程序的设备所属的总线 */	
  struct module    *owner;		/* 模块所有者 */
  const char    *mod_name;  	/* 用于内置模块 */
  bool suppress_bind_attrs;  	/* 通过sysfs禁用绑定/解除绑定 */
  enum probe_type probe_type;	/* 要使用的探测类型（同步或异步） */
  const struct of_device_id  *of_match_table;		/* 设备树匹配表 */
  const struct acpi_device_id *acpi_match_table;	/* ACPI匹配表 */

  /* 调用以查询特定设备的存在，该驱动程序是否可以使用它，并将驱动程序绑定到特定设备 */
  int (*probe) (struct device *dev);
  /* 从系统中删除设备时调用，以解除设备与此驱动程序的绑定 */
  int (*remove) (struct device *dev);
  /* 在关闭时调用以使设备静止 */
  void (*shutdown) (struct device *dev);
  /* 调用以将设备置于睡眠模式。通常为低功率状态 */
  int (*suspend) (struct device *dev, pm_message_t state);
  /* 调用以使设备从睡眠模式恢复 */
  int (*resume) (struct device *dev);
  
  /* 由驱动程序核心自动创建的默认属性 */
  const struct attribute_group **groups;
  /* 设备实例绑定到驱动程序后，附加到设备实例的其他属性 */
  const struct attribute_group **dev_groups;
  /* 与此驱动程序匹配的设备的电源管理操作 */
  const struct dev_pm_ops *pm;
  /* 在写入sysfs条目时调用。设备驱动程序应调用dev_coredump API，从而导致uevent */
  void (*coredump) (struct device *dev);
  /* 驱动程序核心的私有数据，除驱动程序核心外，任何人都无法触及 */
  struct driver_private *p;
};
```

​		1.probe函数：当所加载驱动与相应设备匹配成功时，获得设备资源，完成一个设备驱动所有的操作接口  
​		2.remove函数：设备或驱动移除时，释放probe申请的一切资源.

**用于匹配设备的结构体，如下**：

```c
//path:linux-5.4.194\include\linux\mod_devicetable.h
struct of_device_id {
  char   name[32];
  char   type[32];
  char   compatible[128];	/* 匹配字符串数组 */
  const void *data;
};
```

​		对于设备树而言，此处设备驱动的`compatible`属性值和设备树的`of_match_table`中每个项目的`compatible`属性值比较，若相等则表示设备与此驱动匹配成功。

## 2.4. 调用过程

### 2.4.1. 平台总线注册的调用过程

​		**platform**模块的初始化是由**platform_bus_init**函数完成的。该函数在内核启动阶段被调用, **调用过程**:  
​			`start_kernel() -> arch_call_rest_init() ->  rest_init() -> kernel_init() -> kernel_init_freeable() -> do_basic_setup() -> driver_init() -> platform_bus_init()`

```c
//path:linux-5.4.194\init\main.c
asmlinkage __visible void __init start_kernel(void)
{
    ...
    arch_call_rest_init();    
    ...
}              
void __init __weak arch_call_rest_init(void)
{
	rest_init();
}
noinline void __ref rest_init(void)
{
    ...
    /*  需要首先生成init，以便它获得pid 1，但init任务最终会希望创建kthread，
    	如果我们在创建kthreadd之前调度它，则会导致OOPS。 */
    pid = kernel_thread(kernel_init, NULL, CLONE_FS);  
    ...
}         
static int __ref kernel_init(void *unused)
{
    ...
	kernel_init_freeable();
    ...
}   
static noinline void __init kernel_init_freeable(void)
{
	...
	do_basic_setup();
	...
}
/* 机器已初始化。没有一个设备被触摸过，但CPU子系统已经启动并运行，内存和进程管理也正常。现在终于可以开始做一些真正的工作了 */
static void __init do_basic_setup(void)
{
	...
  driver_init();				/* 驱动初始化函数 */
	...
}
```

​		`driver_init() `= 驱动初始化函数，作用：    
​				调用驱动程序模型init函数来初始化其子系统。早期从init/main.c调用。

```c
//path:linux-5.4.194\drivers\base\init.c
void __init driver_init(void)
{
  /* 一些核心部件 */
  devtmpfs_init();
  devices_init();
  buses_init();
  classes_init();
  firmware_init();
  hypervisor_init();

  /* 也是些核心部件，但是上面之后 */
  of_core_init();
  platform_bus_init();			/* 平台总线初始化函数 */
  cpu_dev_init();
  memory_dev_init();
  container_dev_init();
}
```

​		`platform_bus_init() `= 平台总线注册函数，作用：    
​				调用总线注册函数，传入平台总线结构体变量进行总线的注册。

```c
//path:linux-5.4.194\drivers\base\platform.c
int __init platform_bus_init(void)
{
  int error;
  early_platform_cleanup();
  error = device_register(&platform_bus);
  if (error) {
     put_device(&platform_bus);
     return error;
  }
  error =  bus_register(&platform_bus_type);		//平台总线注册
  if (error)
     device_unregister(&platform_bus);
  of_platform_register_reconfig_notifier();
  return error;
}
```

### 2.4.2. 平台设备注册

​		`platform_device_register()`  = 注册`platform`设备  
​		`platform_add_devices`() = 注册一些`platform`设备   
​		`platform_device_unregister()`  = 注册`platform`设备  

```c
//path:linux-5.4.194\drivers\base\platform.c
/* 注册platform设备 */
int platform_device_register(struct platform_device *pdev)
{
  device_initialize(&pdev->dev);
  setup_pdev_dma_masks(pdev);
  return platform_device_add(pdev);
}
/* 注册一些platform设备 */
int platform_add_devices(struct platform_device **devs, int num)
{
  int i, ret = 0;
  for (i = 0; i < num; i++) {
     ret = platform_device_register(devs[i]);
     if (ret) {
       while (--i >= 0)
         platform_device_unregister(devs[i]);
       break;
     }
  }
  return ret;
}

void platform_device_unregister(struct platform_device *pdev)
{
	platform_device_del(pdev);
	platform_device_put(pdev);
}
```

​		`platform`设备注册由`platform_device_register()` 完成，调用过程：   
​	 	 `platform_device_register()` ： 
​           `-> platform_device_add()  ` ：  **添加platform设备**
   		`-> device_add()   `：				      **添加设备，将platform_device结构信息存到device结构里**
​		   `-> bus_add_device() `：			  **添加设备到platform总线上的dev链表**
​		    	 ` bus_probe_device()   `：		  **探测新设备的驱动程序**

```v
//path:linux-5.4.194\drivers\base\platform.c
/* 添加platform设备 */
int platform_device_add(struct platform_device *pdev)
{
	...
	device_add();
	...
}

//path:linux-5.4.194\drivers\base\core.c
/* 将设备添加到设备层次结构 */
int device_add(struct device *dev)
{
	...
    
	bus_add_device();		/* 将设备添加到总线 */
    bus_probe_device();		
	...
}

//path:linux-5.4.194\drivers\base\bus.c
/* bus_add_device-将设备添加到总线
*-添加设备的总线属性。
*-创建到设备总线的链接。
*-将设备添加到其总线的设备列表中*/
int bus_add_device(struct device *dev)
{		...		} 
/* bus_probe_device-探测新设备的驱动程序 */
void bus_probe_device(struct device *dev)
{
	...
    device_initial_probe()
	...		
} 
//path:linux-5.4.194\drivers\base\dd.c
void device_initial_probe(struct device *dev)
{
	__device_attach(dev, true);
}
static int __device_attach(struct device *dev, bool allow_async)
{
    ...
    device_bind_driver()	/* 将驱动程序绑定到一个设备 */
    __device_attach_driver
	...		
} 
static int __device_attach_driver(struct device_driver *drv, void *_data)
{
    ...
    driver_match_device()
   	...		
} 
```

### 2.4.3. 平台驱动注册

​		`platform_driver_register()`  = 注册platform驱动  
​		`platform_driver_unregister()`= 解注册platform驱动  

```c
//path:linux-5.4.194\drivers\base\platform.c
#define platform_driver_register(drv) \
	__platform_driver_register(drv, THIS_MODULE)

int __platform_driver_register(struct platform_driver *drv,
				struct module *owner)
{
	drv->driver.owner = owner;
	drv->driver.bus = &platform_bus_type;
	drv->driver.probe = platform_drv_probe;
	drv->driver.remove = platform_drv_remove;
	drv->driver.shutdown = platform_drv_shutdown;

	return driver_register(&drv->driver);
}

void platform_driver_unregister(struct platform_driver *drv)
{
	driver_unregister(&drv->driver);
}

//path:linux-3.10.108\drivers\base\platform.c
/* 为平台级设备注册驱动程序 */
int platform_driver_register(struct platform_driver *drv)
{
  drv->driver.bus = &platform_bus_type;
  if (drv->probe)
     drv->driver.probe = platform_drv_probe;
  if (drv->remove)
     drv->driver.remove = platform_drv_remove;
  if (drv->shutdown)
     drv->driver.shutdown = platform_drv_shutdown;
  return driver_register(&drv->driver);
}

```

`platform`驱动注册由`platform_driver_register()` 完成，调用过程：  
		 `platform_device_register()： `   **注册platform驱动**
		`->driver_register()： `  
		`->bus_add_driver()： `  					**添加驱动到platform总线上drv链表**
		`->driver_attach()： `  
		`->bus_for_each_dev()： `  				**依次匹配platform总线上的dev链表成员**

```c
//path:linux-3.10.108\drivers\base\driver.c
/* 用总线注册驱动程序 */
int driver_register(struct device_driver *drv)
{
    ...
    bus_add_driver()
   	...		
} 

//path:inux-3.10.108\drivers\base\bus.c
/* 向总线添加驱动 */
int bus_add_driver(struct device_driver *drv)
{
    ...
    driver_attach()
   	...		
} 

/* 尝试将驱动程序绑定到设备
	查看总线上的设备列表，并尝试将驱动与每个设备匹配，若driver_probe_device() 返回0且dev->driver已设置则找到一对匹配的设备驱动
*/
int driver_attach(struct device_driver *drv)
{
	return bus_for_each_dev(drv->bus, NULL, drv, __driver_attach);
}
```



## 2.5. LED-COLOR实例

​		LED-COLOR采用平台设备驱动，提高驱动可重用性，主要分为：总线，设备，驱动三部分。  
​				1.总线部分：(`bus_type`)  
​						无需我们写，由内核提供。  
​				2.设备部分：(`platform_device`)    
​						1>无设备树：    
​								通过`platform_device_alloc()`创建`platform_device`结构体空间将多色灯的平台数据补充进去后，再`platform_device_add()`将该平台设备注册到对应总线上。
​						2>有设备树：  
​				3.驱动部分：(`platform_driver`)    
​						

​				 

### 2.5.1. LED-COLOR 平台设备注册（非设备树）

​		[leds.h](.\平台驱动代码\linux-3.10.108\include\linux\leds.h)中定义多色灯平台信息的相关结构体.

```c
//path:linux-3.10.108\include\linux\leds.h

/* 通用LED灯数据结构体 */
struct led_info {
  const char  *name;							/* 灯名称 */
  const char  *default_trigger;					/* 默认触发器名称 */
  int   flags;									/*  */
};

/* LED灯平台数据结构体 */
struct led_platform_data {
  int   num_leds;								/* LED的个数 */
  struct led_info *leds;						/* 通用LED数据结构体 */
};

/* 多色灯结构体 */
struct color_led {
	const char *name;							/* 多色灯名称 */
	const char *default_trigger;				/* 默认触发器名称 */
	unsigned 	gpio[LEDS_COLOR_GPIO_NUM];		/* 多个多色灯的gpio口 */
	enum led_color_e default_color;				/* 默认LED初始灯色 */
	unsigned	active_low : 1;					/* 灯的触发极性 */
	unsigned	retain_state_suspended : 1;		/*  */
	unsigned	default_state : 2;				/* 默认LED初始状态 */
	/* default_state should be one of LEDS_GPIO_DEFSTATE_(ON|OFF|KEEP) */
};

/* 多色灯平台数据结构体 */
struct color_led_platform_data {
	int num_leds;									/* 多色led灯的个数 */
	const struct color_led *leds;					/* 多色灯结构体指针 */
	int	(*gpio_blink_set)(unsigned gpio, int state,	/* LED灯闪烁函数指针 */
					unsigned long *delay_on,
					unsigned long *delay_off);
};
```

​		[dev-leds-color.c](.\平台驱动代码\linux-3.10.108\arch\mips\ralink\dev-leds-color.c)中定义多色灯的平台设备注册函数.

```c
//path:linux-3.10.108\arch\mips\ralink\dev-leds-color.c

/**
 * @brief:ralink平台注册多色灯平台驱动
 * @param  id:常为-1
 * @param  num_leds:多色灯数组大小
 * @param  leds:多色灯数组首地址
 */
void __init ralink_register_leds_color(int id, unsigned num_leds,struct color_led *leds)
{
   struct platform_device *pdev;
   struct color_led_platform_data pdata;
   struct color_led *p;
   int err; 
   /* 创建内存空间并将dev-h20m.c中设置的多色灯相关数据复制到空间中 */      
   p = kmalloc(num_leds * sizeof(*p), GFP_KERNEL);
   if (!p)
   	 return;
   memcpy(p, leds, num_leds * sizeof(*p));
    
   /* 创建平台设备：leds-color */
   pdev = platform_device_alloc("leds-color", id);
   if (!pdev)
   	 goto err_free_leds;

   /* 对多色灯平台数据进行赋值 */
   memset(&pdata, 0, sizeof(pdata));
   pdata.num_leds = num_leds;
   pdata.leds = p;

   /* 向平台设备leds-color添加对应相关数据 */
   err = platform_device_add_data(pdev, &pdata, sizeof(pdata));
   if (err)
   	 goto err_put_pdev;

   /* 将该平台设备加入设备层中 */
   err = platform_device_add(pdev);
   if (err)
   	 goto err_put_pdev;

   return;

err_put_pdev:
	platform_device_put(pdev);

err_free_leds:
	kfree(p);
}   
```

​		[dev-h20m.c](.\平台驱动代码\linux-3.10.108\arch\mips\ralink\dev-h20m.c)中定义开发板中对应的GPIO引脚配置，并调用多色平台驱设备函数进行注册

```c
//path:linux-3.10.108\arch\mips\ralink\dev-h20m.c
#define GPIO_UNDEFINEED                 (-1)
#define GPIO_LED_GREEN_SYS              (14)
#define GPIO_LED_RED_SYS                (13)

static struct color_led sysled_leds_color[] __initdata = {
  {
  		.name		= "sysled",
        .gpio[COLOR_RED]        = GPIO_LED_RED_SYS,
        .gpio[COLOR_YELLOW]     = GPIO_UNDEFINEED,
        .gpio[COLOR_GREEN]      = GPIO_LED_GREEN_SYS,
        .gpio[COLOR_BLUE]       = GPIO_UNDEFINEED,
        .active_low	        = 1,
        .default_trigger        = "timer",
		.default_state          = 0,
		.default_color          = LED_COLOR_GREEN,
  },
};

int __init mt7621_device_init(void){
	u32 t; 
#ifdef CONFIG_NOT_RGOS
    ralink_gpio_init(); /* 'ralink_gpio_init()' has already called by 'ralink_gpio.c' */
#endif
	t = le32_to_cpu(*(volatile u32 *)(RALINK_REG_GPIOMODE));
	t |= (RALINK_GPIOMODE_JTAG);
	*(volatile u32 *)(RALINK_REG_GPIOMODE) = t;    /* set jtag to gpio*/
    
    /* 设置对应gpio口的输入输出方向，false表示输出，true表示输入 */
    ralink_gpio_direction_select(GPIO_LED_GREEN_SYS, false);
    ralink_gpio_direction_select(GPIO_LED_RED_SYS, false);
    
    /* 注册多色灯数据到平台驱动中 */
    ralink_register_leds_color(-1, ARRAY_SIZE(sysled_leds_color), sysled_leds_color);
	return 0;
}

/* 解注册函数 */
int __init device_gpio_unregister(void)
{
    ralink_gpio_reset(GPIO_LED_GREEN_SYS);
	ralink_gpio_reset(GPIO_LED_RED_SYS);
 	return 0;
}

/* 内核初始化完成后自动运行 */
arch_initcall(mt7621_device_init);
```



### 2.5.2. LED-COLOR 平台设备注册（设备树）



```c
//（设备树）path:linux-5.4.194\arch\arm64\boot\dts\mediatek\ruijie_h30m_gsw.dts
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
```



### 2.5.3. LED-COLOR 平台驱动注册

​		linux-3.10.108 内核中 [leds-color.c](.\平台驱动代码\linux-3.10.108\drivers\leds\leds-color.c)  
​		linux-5.4.194 内核中[leds-color.c](.\平台驱动代码\linux-5.4.194\drivers\leds\leds-color.c)

```c
//（非设备树）path:linux-3.10.108\drivers\leds\leds-color.c
//（设备树）path:linux-5.4.194\drivers\leds\leds-color.c
...

/* 用于设备树 */
static const struct of_device_id of_color_leds_match[] = {
	{ .compatible = "color-leds", },
	{},
};    
MODULE_DEVICE_TABLE(of, of_color_leds_match)
    
static struct platform_driver color_led_driver = {
	.probe      = color_led_probe,			/* 设备-驱动匹配后调用，用于设备与驱动绑定 */
	.remove     = color_led_remove,			/* 用于设备与驱动解绑 */
	.shutdown   = color_led_shutdown,		/* 关闭时使设备静止 */	
	.driver     = {							/* device_driver结构体 */
		.name   = "leds-color",				/* 设备驱动名 */
		.owner	= THIS_MODULE,				/* 模块拥有者 */
		.of_match_table = of_match_ptr(of_color_leds_match),/* of匹配表,用于设备树 */
	},
};

/* module_platform_driver（）-用于在模块init/exit中不做任何特殊操作的驱动程序的助手宏。这消除了很多样板。每个模块只能使用此宏一次，调用它将替换module_init（）和module_exit（） */
module_platform_driver(color_led_driver);

MODULE_AUTHOR("Zhang Yilin <zhangyilin@ruijie.com.cn>");
MODULE_DESCRIPTION("COLORFUL LED driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:leds-color");
```



## 2.6. h30m pwm适配



### 2.6.1. pwm设备树

```c
//path:linux-5.4.194/Documentation/devicetree/bindings/pwm/pwm-mediatek.txt
属性简介：
    1. compatible 属性：		匹配驱动
    	格式：compatible = "mediatek,<name>-pwm"
    	举例：compatible = "mediatek,mt7981-pwm";
	2. reg 属性：				控制器寄存器的物理基地址和长度
        reg = <0 0x11006000 0 0x1000>;
    3. pwm-cells 属性：		必须为2
        #pwm-cells = <2>;  
    4. clocks 属性：			PWM参考时钟的显色器和时钟说明符
    5. clock-names属性：		必须包含以下内容，但没有时钟的MT7628除外
      	-“top”：顶级时钟生成器
		-“main”：PWM核心使用的时钟
		-“pwm1-8”：mt2712的八个每PWM时钟
		-“pwm1-6”：mt7622的六个每PWM时钟
		-“pwm1-5”：mt7623的五个每PWM时钟  
    6. pinctrl-names属性：		必须包含“default”。
    7. pinctrl-0属性：pinctrl名称中的每个条目必须存在一个属性
        请参见pinctrl/ppinctrl-bindings.txt
实例：
pwm0: pwm@11006000 {
    compatible = "mediatek,mt7623-pwm";
    reg = <0 0x11006000 0 0x1000>;
    #pwm-cells = <2>;
    clocks = <&topckgen CLK_TOP_PWM_SEL>,
    <&pericfg CLK_PERI_PWM>,
    <&pericfg CLK_PERI_PWM1>,
    <&pericfg CLK_PERI_PWM2>,
    <&pericfg CLK_PERI_PWM3>,
    <&pericfg CLK_PERI_PWM4>,
    <&pericfg CLK_PERI_PWM5>;
    clock-names = "top", "main", "pwm1", "pwm2",
    "pwm3", "pwm4", "pwm5";
    pinctrl-names = "default";
    pinctrl-0 = <&pwm0_pins>;
};        
        
//path:linux-5.4.194/arch/arm64/boot/dts/mediatek/mt7981.dtsi
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


```



### 2.6.2. pwm驱动

```c
//path:linux-5.4.194/drivers/pwm/pwm-mediatek.c

...
    
struct pwm_mediatek_of_data {
	unsigned int num_pwms;
	bool pwm45_fixup;
	int reg_ver;
};

static const struct pwm_mediatek_of_data mt7981_pwm_data = {
  .num_pwms = 3,
  .pwm45_fixup = false,
  .reg_ver = REG_V2,
};

static const struct of_device_id pwm_mediatek_of_match[] = {
	{ .compatible = "mediatek,mt7981-pwm", .data = &mt7981_pwm_data },
	{ },
};
MODULE_DEVICE_TABLE(of, pwm_mediatek_of_match);

static struct platform_driver pwm_mediatek_driver = {
	.probe = pwm_mediatek_probe,
	.remove = pwm_mediatek_remove,
	.driver = {
		.name = "pwm-mediatek",
		.of_match_table = pwm_mediatek_of_match,
	},
};
module_platform_driver(pwm_mediatek_driver);

MODULE_AUTHOR("John Crispin <blogic@openwrt.org>");
MODULE_LICENSE("GPL v2");
```











# 2. 子系统

## 2.1. input子系统

### 2.1.1. 简介：

​		input 子系统就是管理输入的子系统，是Linux内核针对输入设备（本质还是字符设备）而创建的框架，用于专门处理输入事件，如：  
​				①按键/键盘：按键信息。②鼠标/触摸屏：坐标信息。

### 2.1.2. 分层：

（1） 硬件层：具体的硬件设备，如：按键、USB 键盘/鼠标等。

（2）内核层：

​		①**驱动层**：输入设备的具体驱动程序，比如按键驱动程序，向内核层报告输入内容。

​		②**核心层**：承上启下，为驱动层提供输入设备注册和操作接口。通知事件层对输入事件进行处理。

​		③**事件层**：主要和用户空间进行交互。

（3）用户层：所有的输入设备都以文件形式提供给用户应用程序使用。

​	

## 2.2. gpio子系统

### 2.2.1. 简介：

​		gpio 子系统用于初始化 GPIO 并且提供相应的 API 函数，比如设置 GPIO为输入输出，读取 GPIO 的值等。
​		gpio 子系统的主要目的就是方便驱动开发者使用 gpio，驱动开发者在设备树中添加 gpio 相关信息，然后就可以在驱动程序中使用 gpio 子系统提供的 API函数来操作 GPIO， Linux 内核向驱动开发者屏蔽掉了 GPIO 的设置过程，极大的方便了驱动开发者使用 GPIO。



### 2.3.pinctrl子系统

### 2.3.1. 简介：  

​		大多数片上系统(Soc)的引脚(Pin)都是支持复用的，故配置时要考虑复用的设置，此外还要配置Pin的电气特性，比如上下拉、速度、驱动等。传统的配置Pin的方式就是直接操控相应寄存器，但该方式繁琐且容易出错（Pin功能冲突），pinctrl子系统就是为解决该问题引入，主要工作内容：  
​				1.获取设备树中pin信息。
​				2.根据获得到的pin信息来设置pin的**复用功能**。
​				3.根据获得到的pin信息来设置pin的**电气特性**，比如上下拉、速度、驱动能力。  
​		pinctrl 子系统源码目录为 drivers/pinctrl.

















