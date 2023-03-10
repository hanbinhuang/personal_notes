#ifndef __CFG_DEBUG_H__
#define __CFG_DEBUG_H__
#include <libdebug/libdebug.h>

extern dbg_md_t g_cfg_id;

#define CFG_LOG(fmt, arg...) do { \
    dbg_logfile(g_cfg_id, fmt, ##arg);\
} while (0)

#define CFG_DEBUG(fmt, arg...) do { \
    dbg_printf(g_cfg_id, DBG_LV_DEBUG, fmt, ##arg);\
} while (0)

#define CFG_WARNING(fmt, arg...) do { \
    dbg_printf(g_cfg_id, DBG_LV_WARNING, "WARNING in %s [%d]: "fmt, __FILE__, __LINE__, ##arg);\
} while (0)

#define CFG_ERROR(fmt, arg...) do { \
    dbg_printf(g_cfg_id, DBG_LV_ERROR, "ERROR in %s [%d]: "fmt, __FILE__, __LINE__, ##arg);\
} while (0)

#endif /* __CFG_DEBUG_H__ */