# Variabili di configurazione
CC       = gcc
CFLAGS   = -Wall -Wextra -std=c99
SRC_DIR  = src
OBJ_DIR  = obj
BUILD_DIR= build
TARGET   = $(BUILD_DIR)/app

# Recupera tutti i file sorgenti dalla cartella src
SOURCES  = $(wildcard $(SRC_DIR)/*.c)

# Converte i file sorgente in file oggetto nella cartella obj
OBJECTS  = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

# Regola di default per la compilazione
all: $(TARGET)

# Compilazione dell'eseguibile finale
$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(OBJECTS) -o $(TARGET)

# Regola per compilare i file .o in obj
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Pulizia dei file compilati
clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)

.PHONY: all clean