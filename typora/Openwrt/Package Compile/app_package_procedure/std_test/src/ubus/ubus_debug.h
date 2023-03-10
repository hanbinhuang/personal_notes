#ifndef __UBUS_DEBUG_H__
#define __UBUS_DEBUG_H__
#include <libdebug/libdebug.h>

extern dbg_md_t g_ubus_id;

#define UBUS_LOG(fmt, arg...) do { \
    dbg_logfile(g_ubus_id, fmt, ##arg);\
} while (0)

#define UBUS_DEBUG(fmt, arg...) do { \
    dbg_printf(g_ubus_id, DBG_LV_DEBUG, fmt, ##arg);\
} while (0)

#define UBUS_WARNING(fmt, arg...) do { \
    dbg_printf(g_ubus_id, DBG_LV_WARNING, "WARNING in %s [%d]: "fmt, __FILE__, __LINE__, ##arg);\
} while (0)

#define UBUS_ERROR(fmt, arg...) do { \
    dbg_printf(g_ubus_id, DBG_LV_ERROR, "ERROR in %s [%d]: "fmt, __FILE__, __LINE__, ##arg);\
} while (0)

#endif /* __UBUS_DEBUG_H__ */