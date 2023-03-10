#ifndef __MAIN_DEBUG_H__
#define __MAIN_DEBUG_H__
#include <libdebug/libdebug.h>

extern dbg_md_t g_main_id;

/* debug */
#define MAIN_LOG(fmt, arg...) do { \
    dbg_logfile(g_main_id, fmt, ##arg);\
} while (0)

#define MAIN_DEBUG(fmt, arg...) do { \
    dbg_printf(g_main_id, DBG_LV_DEBUG, fmt, ##arg);\
} while (0)

#define MAIN_WARNING(fmt, arg...) do { \
    dbg_printf(g_main_id, DBG_LV_WARNING, "WARNING in %s [%d]: "fmt, __FILE__, __LINE__, ##arg);\
} while (0)

#define MAIN_ERROR(fmt, arg...) do { \
    dbg_printf(g_main_id, DBG_LV_ERROR, "ERROR in %s [%d]: "fmt, __FILE__, __LINE__, ##arg);\
} while (0)



#endif /* __MAIN_DEBUG_H__ */