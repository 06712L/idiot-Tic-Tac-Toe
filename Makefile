CC ?= gcc
CFLAGS ?= -Wall -O2 -static
CFLAGS-debug ?= -Wall -O0 -g -static
CCwin ?= x86_64-w64-mingw32-gcc
CFLAGS ?= -Wall -O2 -static
CFLAGS-debug ?= -Wall -O0 -g -static
LIBS =
LIBSwin = -lwinmm
TARGET = idiot-tic-tac-toe
SRCLINUX_DIR = ./src/src-linux
SRCWIN_DIR = ./src/src-win
OBJS_DIR = oiia
ELF_DIR = elf
EXE_DIR = exe
OBJS = ./$(OBJS_DIR)/main.o ./$(OBJS_DIR)/menu.o ./$(OBJS_DIR)/game.o
OBJSwin = ./$(OBJS_DIR)/main-win.o ./$(OBJS_DIR)/menu-win.o ./$(OBJS_DIR)/game-win.o

.PHONY: linux linuxdebug win windebug cleanlinux cleanwin clean
all: linux


linux: $(OBJS)
	@mkdir -p $(ELF_DIR)
	@cp -n -r ./sound ./$(ELF_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o ./$(ELF_DIR)/$(TARGET)-release $(LIBS)


linuxdebug: $(OBJS)
	@mkdir -p $(ELF_DIR)
	@cp -n -r ./sound ./$(ELF_DIR)
	$(CC) $(CFLAGS-debug) $(OBJS) -o ./$(ELF_DIR)/$(TARGET)-debug $(LIBS)


win: $(OBJSwin)
	@mkdir -p $(EXE_DIR)
	@cp -n -r ./sound ./$(EXE_DIR)
	$(CCwin) $(CFLAGS) $(OBJSwin) -o ./$(EXE_DIR)/$(TARGET)-release.exe $(LIBSwin)


windebug: $(OBJSwin)
	@mkdir -p $(EXE_DIR)
	@cp -n -r ./sound ./$(EXE_DIR)
	$(CCwin) $(CFLAGS-debug) $(OBJSwin) -o ./$(EXE_DIR)/$(TARGET)-debug.exe $(LIBSwin)


./$(OBJS_DIR)/%.o: $(SRCLINUX_DIR)/%.c
	@mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


./$(OBJS_DIR)/%-win.o: $(SRCWIN_DIR)/%.c
	@mkdir -p $(OBJS_DIR)
	$(CCwin) $(CFLAGS) -c $< -o $@


cleanlinux:
	rm -rf ./$(OBJS_DIR) ./$(ELF_DIR) ./*.log


cleanwin:
	rm -rf ./$(OBJS_DIR) ./$(EXE_DIR)


clean:
	rm -rf ./$(OBJS_DIR) ./$(ELF_DIR) ./$(EXE_DIR) ./*.log
