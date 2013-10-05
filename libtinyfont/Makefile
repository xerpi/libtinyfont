TARGET_LIB = libtinyfont.a
OBJS = tinyfont.o font_data.o

CFLAGS = -G0 -Wall -O2
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)


PSPSDK=$(shell psp-config --pspsdk-path)
PSPDEV=$(shell psp-config --pspdev-path)
include $(PSPSDK)/lib/build.mak

all:
	@echo "Type 'make install' to install the library!"

install: $(TARGET_LIB)
	cp $(TARGET_LIB) $(PSPDEV)/psp/lib
	cp tinyfont.h $(PSPDEV)/psp/include
	@echo "libtinyfont installed!"
	
