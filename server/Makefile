# Compiler configuration
CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99 -I$(SRC_DIR) -I$(LIB_DIR)
LDFLAGS =
LIBS    =

# Project directories
SRC_DIR   = src
LIB_DIR   = libs
OBJ_DIR   = obj
BUILD_DIR = bin

# Source files and corresponding objects
SOURCES      = $(wildcard $(SRC_DIR)/*.c)
LIB_SOURCES  = $(wildcard $(LIB_DIR)/*.c)
OBJECTS      = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
LIB_OBJECTS  = $(patsubst $(LIB_DIR)/%.c, $(OBJ_DIR)/%.o, $(LIB_SOURCES))
ALL_OBJECTS  = $(OBJECTS) $(LIB_OBJECTS)

TARGET   = $(BUILD_DIR)/app

# Default rule
all: $(TARGET)

# Build final executable
$(TARGET): $(ALL_OBJECTS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(ALL_OBJECTS) -o $(TARGET) $(LDFLAGS) $(LIBS)

# Compile source files from src folder
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile source files from libs folder
$(OBJ_DIR)/%.o: $(LIB_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)

.PHONY: all clean
