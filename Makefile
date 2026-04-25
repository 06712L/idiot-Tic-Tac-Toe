CC ?= gcc
CFLAGS = -Wall -O2 -static
CFLAGS-debug = -Wall -O0 -g -static
TARGET = I-TTT
SRCLINUX_DIR = ./src/src-linux
OBJS_DIR = oiia
ELF_DIR = elf
OBJS = ./$(OBJS_DIR)/main.o ./$(OBJS_DIR)/menu.o ./$(OBJS_DIR)/game.o
.PHONY: linux linuxdebug cleanlinux clean
all: linux


linux: $(OBJS)
	@mkdir -p $(ELF_DIR)
	@cp -n -r ./sound ./$(ELF_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o ./$(ELF_DIR)/$(TARGET)-release


linuxdebug: $(OBJS)
	@mkdir -p $(ELF_DIR)
	@cp -n -r ./sound ./$(ELF_DIR)
	$(CC) $(CFLAGS-debug) ./$(OBJS_DIR)/$(OBJS) -o ./$(ELF_DIR)/$(TARGET)-debug


./$(OBJS_DIR)/%.o: $(SRCLINUX_DIR)/%.c
	@mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


cleanlinux:
	rm -rf ./$(OBJS_DIR) ./$(ELF_DIR)


clean:
	rm -rf ./$(OBJS_DIR) ./$(ELF_DIR)
