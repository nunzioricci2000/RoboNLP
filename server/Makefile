CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99 -I$(SRC_DIR) -I$(LIB_DIR)
LDFLAGS =
LIBS    =

SRC_DIR   = src
LIB_DIR   = libs
OBJ_DIR   = obj
BUILD_DIR = bin

SOURCES      = $(wildcard $(SRC_DIR)/*.c)
LIB_SOURCES  = $(wildcard $(LIB_DIR)/*.c)
OBJECTS      = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
LIB_OBJECTS  = $(patsubst $(LIB_DIR)/%.c, $(OBJ_DIR)/%.o, $(LIB_SOURCES))
ALL_OBJECTS  = $(OBJECTS) $(LIB_OBJECTS)

TARGET   = $(BUILD_DIR)/app

all: $(TARGET)

$(TARGET): $(ALL_OBJECTS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(ALL_OBJECTS) -o $(TARGET) $(LDFLAGS) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(LIB_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)

.PHONY: all clean
