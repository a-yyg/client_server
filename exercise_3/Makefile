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

TARGET := server3 client3

all: $(TARGET)

server3: $(OBJ_DIR)/server3.o
	$(shell mkdir -p $(BUILD_DIR))
	$(shell chmod -R 777 $(BUILD_DIR))
	$(CC) $(CCFLAGS) $(DBGFLAGS) -o $(BUILD_DIR)/server3 $(OBJ_DIR)/server3.o

client3: $(OBJ_DIR)/client3.o
	$(shell mkdir -p $(BUILD_DIR))
	$(shell chmod -R 777 $(BUILD_DIR))
	$(CC) $(CCFLAGS) $(DBGFLAGS) -o $(BUILD_DIR)/client3 $(OBJ_DIR)/client3.o

$(OBJ_DIR)/%.o: %.c
	$(shell mkdir -p $(OBJ_DIR))
	$(shell chmod -R 777 $(OBJ_DIR))
	$(CC) $(CCFLAGS) $(DBGFLAGS) -c -o $@ $<

# phony targets
clean:
	$(shell rm -rf $(OBJ_DIR) $(BUILD_DIR) *.o)

.PHONY: all clean

# end
