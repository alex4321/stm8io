CC=sdcc
CFLAGS=-mstm8
INCLUDEPATH=lib/stm8/inc/
DEFINES= STM8S003
OUTPUT_DIR=build/
all:	compile flash

clean:	
	rm -rf $(OUTPUT_DIR)/*

compile:	create_build_directory compile_uart compile_io compile_main link
create_build_directory:	
	mkdir -p $(OUTPUT_DIR)
compile_uart:	src/uart.c
	$(CC) -c $(CFLAGS) -I $(INCLUDEPATH) -D $(DEFINES) -o $(OUTPUT_DIR) src/uart.c
compile_io:	src/io.c
	$(CC) -c $(CFLAGS) -I $(INCLUDEPATH) -D $(DEFINES) -o $(OUTPUT_DIR) src/io.c
compile_main:	src/io.c
	$(CC) -c $(CFLAGS) -I $(INCLUDEPATH) -D $(DEFINES) -o $(OUTPUT_DIR) src/main.c
link:	build/main.rel build/uart.rel  build/io.rel
	$(CC) $(CFLAGS) -I $(INCLUDEPATH) -D $(DEFINES) -o $(OUTPUT_DIR)/main.ihx build/uart.rel build/io.rel build/main.rel

flash_windows:	build/main.ihx
	packihx $(OUTPUT_DIR)/main.ihx > $(OUTPUT_DIR)/main.hex
	stvp_cmdline -BoardName=ST-LINK -Device=STM8S003F3 -Port=USB -ProgMode=SWIM \
	-verbose -no_loop -no_log \
	-FileProg=$(OUTPUT_DIR)/main.hex

flash_unix:	build/main.ihx
	stm8flash -c stlinkv2 -p stm8s003 -w $(OUTPUT_DIR)/$(OUTPUT_FILE).ihx
	
ifeq ($(OS),Windows_NT)
flash:	flash_windows
else
flash:	flash_unix
endif