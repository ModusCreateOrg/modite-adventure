BUILD_DIR = build.ldk
SOURCE_DIR = src/LDK

IPK_DEST_DIR = _modus_test


PROG = $(BUILD_DIR)/modus_test.elf
SRCS = $(wildcard src/LDK/*.cpp)
OBJS = $(SRCS:.cpp=.o)

CHAINPREFIX := /opt/mipsel-linux-uclibc
CROSS_COMPILE := $(CHAINPREFIX)/usr/bin/mipsel-linux-

CXX := $(CROSS_COMPILE)g++
CC  := $(CROSS_COMPILE)cc
STRIP := $(CROSS_COMPILE)strip
RC  := $(CROSS_COMPILE)windres

SYSROOT := $(shell $(CC) --print-sysroot)
SDL_CFLAGS := $(shell $(SYSROOT)/usr/bin/sdl-config --cflags)
SDL_LIBS := $(shell $(SYSROOT)/usr/bin/sdl-config --libs)

# CXXFLAGS = -Wall -Ofast -mno-abicalls -mplt $(SDL_CFLAGS) -DDINGUX
CXXFLAGS = -Wall -Ofast -mplt $(SDL_CFLAGS) -DDINGUX
LDFLAGS = $(SDL_LIBS) -lSDL_mixer -lSDL_image -lSDL_gfx -flto -s


$(PROG): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(PROG)

ipk: $(PROG)
	@rm -rf /tmp/.modus_test-ipk/
	@mkdir -p /tmp/.modus_test-ipk/root/home/retrofw/games/$(IPK_DEST_DIR) /tmp/.modus_test-ipk/root/home/retrofw/apps/gmenu2x/sections/games
	@cp -r $(BUILD_DIR)/modus_test.elf $(SOURCE_DIR)/modus_test.png /tmp/.modus_test-ipk/root/home/retrofw/games/$(IPK_DEST_DIR)
	@cp $(SOURCE_DIR)/modus_test.lnk /tmp/.modus_test-ipk/root/home/retrofw/apps/gmenu2x/sections/games
	@sed "s/^Version:.*/Version: $$(date +%Y%m%d)/" $(SOURCE_DIR)/control > /tmp/.modus_test-ipk/control
	@cp $(SOURCE_DIR)/conffiles /tmp/.modus_test-ipk/
	@tar --owner=0 --group=0 -czvf /tmp/.modus_test-ipk/control.tar.gz -C /tmp/.modus_test-ipk/ control conffiles
	@tar --owner=0 --group=0 -czvf /tmp/.modus_test-ipk/data.tar.gz -C /tmp/.modus_test-ipk/root/ .
	@echo 2.0 > /tmp/.modus_test-ipk/debian-binary
	@ar r build.ldk/modus_test.ipk /tmp/.modus_test-ipk/control.tar.gz /tmp/.modus_test-ipk/data.tar.gz /tmp/.modus_test-ipk/debian-binary

clean:
	rm -f $(PROG) src/LDK/*.o

.PHONY: clean
