OSNAME:=$(shell uname -s)

CPPFLAGS=-I/usr/local/lib -Wno-write-strings
ifeq ($(OSNAME),Darwin)
	CPPFLAGS += -lhidapi
else
	CPPFLAGS += -I/usr/local/include/hidapi -lhidapi-libusb
endif

zeal60: zeal60.cpp keycode.cpp keycode.h config.h
	g++ $(CPPFLAGS) zeal60.cpp keycode.cpp -o zeal60

zeal65: zeal65.cpp keycode.cpp keycode.h config.h
	g++ $(CPPFLAGS) zeal65.cpp keycode.cpp -o zeal65

clean:
	$(RM) zeal60 zeal65

all: zeal60 zeal65