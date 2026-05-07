CC ?= gcc
CCWIN ?= x86_64-w64-mingw32-gcc
DEBUG ?= 0
CFLAGS := -Wall -std=gnu99 -static
ifeq ($(DEBUG),1)
CFLAGS += -O0 -g
OBJS_DIR := oiia-debug
VERSION := debug
else
CFLAGS += -O2 -s
OBJS_DIR := oiia
VERSION := release
endif
LIBS =
LIBSWIN = -lwinmm
TARGET = idiot-tic-tac-toe
SRCLINUX_DIR = ./src/src-linux
SRCWIN_DIR = ./src/src-win
ELF_DIR = elf
EXE_DIR = exe
OBJS = ./$(OBJS_DIR)/main.o ./$(OBJS_DIR)/menu.o ./$(OBJS_DIR)/game.o
OBJSWIN = ./$(OBJS_DIR)/main-win.o ./$(OBJS_DIR)/menu-win.o ./$(OBJS_DIR)/game-win.o

.PHONY: linux win cleanlinux cleanwin clean
all: linux


linux: $(OBJS)
	@mkdir -p $(ELF_DIR)
	@cp -n -r ./sound ./$(ELF_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o ./$(ELF_DIR)/$(TARGET)-$(VERSION) $(LIBS)


win: $(OBJSWIN)
	@mkdir -p $(EXE_DIR)
	@cp -n -r ./sound ./$(EXE_DIR)
	$(CCWIN) $(CFLAGS) $(OBJSWIN) -o ./$(EXE_DIR)/$(TARGET)-$(VERSION).exe $(LIBSWIN)


./$(OBJS_DIR)/%.o: $(SRCLINUX_DIR)/%.c
	@mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


./$(OBJS_DIR)/%-win.o: $(SRCWIN_DIR)/%.c
	@mkdir -p $(OBJS_DIR)
	$(CCWIN) $(CFLAGS) -c $< -o $@


cleanlinux:
	rm -rf $(OBJS) ./$(ELF_DIR) ./*.log


cleanwin:
	rm -rf $(OBJSWIN) ./$(EXE_DIR)


clean:
	rm -rf ./$(OBJS_DIR) ./$(ELF_DIR) ./$(EXE_DIR) ./*.log
