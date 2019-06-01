CC := gcc
CFLAGS := -g -Wall `pkg-config --cflags libusb-1.0`
LDLIBS := `pkg-config --libs libusb-1.0`

srcdir := $(dir $(word 1,$(MAKEFILE_LIST)))

DEST := wrdctl
SRC  := main.c operate.c simple_usb.c

OBJS := $(patsubst %.c,%.o,$(SRC))


all: $(DEST)

.PHONY: all distclean clean

$(DEST): $(OBJS)
	$(CC) -o $@ $(CFLAGS) $^ $(LDLIBS)

distclean: clean

clean:
	$(RM) $(OBJS) $(DEST)



### ヘッダ依存関係 自動解決 ###
DEPEND_DIR := _depend
DEPENDS := $(patsubst %.c,$(DEPEND_DIR)/%.d,$(SRC))

$(DEPEND_DIR)/%.d: $(srcdir)/%.c
	@install -d $(dir $@)
	$(CC) -MM -MG -MF $@ $<

.PHONY: cleandep
cleandep:
	$(RM) -r $(DEPEND_DIR)
distclean: cleandep

ifneq "$(findstring clean, $(MAKECMDGOALS))" "clean"
  -include $(DEPENDS)
endif


