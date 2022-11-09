##
# i
#
# @file
# @version 0.1

# tool macros
CC := gcc
CCFLAGS := -Wall -Werror -O2
DBGFLAGS := -g -O0

OBJ_DIR := $(shell pwd)/../obj
BUILD_DIR := $(shell pwd)/../bin

TARGET := server client

all: $(TARGET)

server: $(OBJ_DIR)/server.o
	$(shell mkdir -p $(BUILD_DIR))
	$(shell chmod -R 777 $(BUILD_DIR))
	$(CC) $(CCFLAGS) $(DBGFLAGS) -o $(BUILD_DIR)/server $(OBJ_DIR)/server.o

client: $(OBJ_DIR)/client.o
	$(shell mkdir -p $(BUILD_DIR))
	$(shell chmod -R 777 $(BUILD_DIR))
	$(CC) $(CCFLAGS) $(DBGFLAGS) -o $(BUILD_DIR)/client $(OBJ_DIR)/client.o

$(OBJ_DIR)/%.o: %.c
	$(shell mkdir -p $(OBJ_DIR))
	$(shell chmod -R 777 $(OBJ_DIR))
	$(CC) $(CCFLAGS) $(DBGFLAGS) -c -o $@ $<

# phony targets
clean:
	$(shell rm -rf $(OBJ_DIR) $(BUILD_DIR) *.o)

.PHONY: all clean

# end
