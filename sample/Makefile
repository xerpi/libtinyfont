TARGET = tinyfont_sample
OBJS = main.o vram.o

INCDIR =
CFLAGS = -G0 -Wall -O2
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =
LIBS= -ltinyfont -lpspdisplay -lpspgum -lpspgu -lm

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = tinyfont sample by xerpi

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak


copy: EBOOT.PBP
	mkdir -p /media/$(USER)/disk/PSP/GAME/$(notdir $(CURDIR))
	cp EBOOT.PBP /media/$(USER)/disk/PSP/GAME/$(notdir $(CURDIR))
	sync
