ifndef CFLAGS
CFLAGS = -O2 -g -I ../src
endif
APP = std_test
BUILD_DIR = ./build
OBJSDIR = ./build/obj
ELF_FILE = $(BUILD_DIR)/$(APP).elf

DB_INCLUDE = -I./
DB_INCLUDE += -I$(STAGING_DIR)/usr/include

DB_CFLAGS += $(DB_INCLUDE) -g
LIB= -L$(STAGING_DIR)/usr/lib -lblobmsg_json -luci -lubox -lubus -ldebug -ljson-c

VPATH += utils:ubus:cfg:.
SRCDIRS += utils ubus cfg .

SRCFILES = $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c))
SRCFILENAMES = $(notdir $(SRCFILES))
OBJS = $(patsubst %.c, %.o,$(SRCFILENAMES))

build: make-dir $(OBJSDIR) $(OBJS) $(TARDIR)
	$(CC) $(LDFLAGS) -o $(ELF_FILE) $(OBJSDIR)/*.o $(LINKFLAGS) $(LIB)

$(OBJS): %.o:%.c
	$(CC) -c $(CFLAGS) $(DB_CFLAGS) $< -o $(OBJSDIR)/$@

.PHONY: make-dir
make-dir:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(OBJSDIR)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
