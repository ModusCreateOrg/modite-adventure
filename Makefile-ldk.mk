BUILD_DIR = build.ldk
SOURCE_DIR = src/LDK


TARGET = ${BUILD_DIR}/modus.dge
SRCS = $(wildcard ${SOURCE_DIR}/*.cpp)
OBJS = $(SRCS:.cpp=.o)



CHAINPREFIX := ~/ldk/mipsel-linux-uclibc
CROSS_COMPILE := $(CHAINPREFIX)/usr/bin/mipsel-linux

CXX := $(CROSS_COMPILE)-g++
CC  := $(CROSS_COMPILE)-cc
STRIP := $(CROSS_COMPILE)-strip
RC  := $(CROSS_COMPILE)-windres

SYSROOT := $(shell $(CC) --print-sysroot)
SDL_CFLAGS := $(shell $(SYSROOT)/usr/bin/sdl-config --cflags)
SDL_LIBS := $(shell $(SYSROOT)/usr/bin/sdl-config --libs)


CFLAGS =  -std=gnu++03 $(INCLUDE) -DRC_OPTIMIZED -DGCW_ZERO -D__LINUX__ -D__DINGUX__ -DFOREVER_16_BIT  $(SDL_CFLAGS)
CFLAGS += -O3 -fdata-sections -ffunction-sections -mips32 -march=mips32 -mno-mips16 -fomit-frame-pointer -fno-builtin
CFLAGS += -fno-common -Wno-write-strings -Wno-sign-compare -ffast-math -ftree-vectorize
CFLAGS += -funswitch-loops -fno-strict-aliasing
CFLAGS += -DMIPS_XBURST -DFAST_LSB_WORD_ACCESS
CFLAGS += -fprofile-use -fprofile-dir=./profile -DNO_ROM_BROWSER

# CXXFLAGS = -Wall -Ofast -mno-abicalls -mplt $(SDL_CFLAGS) -DDINGUX
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti -fno-math-errno -fno-threadsafe-statics
LDFLAGS = $(SDL_LIBS) -lSDL_mixer -lSDL_image -lSDL_gfx -flto -s



## TODO: Turn into legit shell script

DEPLOY_TMP = /tmp/.modus-ipk/root/home/retrofw/test

.PHONY : all
all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CMD)$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@


ipk: $(PROG)
	@rm -rf $(DEPLOY_TMP)
	@mkdir -p $(DEPLOY_TMP)
	@mkdir -p $(DEPLOY_TMP)/games
	@cp -r build.ldk/modus.dge src/LDK/modus.png $(DEPLOY_TMP)
	@cp $(SOURCE_DIR)/modus.lnk $(DEPLOY_TMP)
	@sed "s/^Version:.*/Version: $$(date +%Y%m%d)/" $(SOURCE_DIR)/control > /tmp/.modus-ipk/control
	@cp $(SOURCE_DIR)/conffiles /tmp/.modus-ipk/
	@tar --owner=0 --group=0 -czvf /tmp/.modus-ipk/control.tar.gz -C /tmp/.modus-ipk/ control conffiles
	@tar --owner=0 --group=0 -czvf /tmp/.modus-ipk/data.tar.gz -C /tmp/.modus-ipk/root/ .
	@echo 2.0 > /tmp/.modus-ipk/debian-binary
	@ar r ${BUILD_DIR}/modus.ipk /tmp/.modus-ipk/control.tar.gz /tmp/.modus-ipk/data.tar.gz /tmp/.modus-ipk/debian-binary

clean:
	rm -f $(PROG) ${SOURCE_DIR}/*.o

.PHONY: clean
