OSNAME:=$(shell uname -s)

CPPFLAGS=-I/usr/local/lib -Wno-write-strings

# Order of LDFLAGS matters! Remember, linking happens after compiling.
LDFLAGS=

ifeq ($(OSNAME),Darwin)
	CPPFLAGS += -lhidapi
else
	CPPFLAGS += -I/usr/local/include/hidapi
	LDFLAGS += -lhidapi-libusb
endif

zeal60: zeal60.cpp keycode.cpp keycode.h config.h
	g++ $(CPPFLAGS) zeal60.cpp keycode.cpp -o zeal60 $(LDFLAGS)

zeal65: zeal60.cpp keycode.cpp keycode.h config.h
	g++ $(CPPFLAGS) -DZEAL65 zeal60.cpp keycode.cpp -o zeal65 $(LDFLAGS)

clean:
	$(RM) zeal60 zeal65

all: zeal60 zeal65
