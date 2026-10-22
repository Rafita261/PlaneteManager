CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99
AR = ar
ARFLAGS = rcs

LIBS_DIR = libs
OBJ_DIR = obj
SOURCE = src
HEADER = include

# make MODE=debug pour compiler avec les symboles de debug
MODE ?= release
ifeq ($(MODE),debug)
    CFLAGS += -g -O0 -DDEBUG
else
    CFLAGS += -O2
endif

LIB = $(LIBS_DIR)/libElements.a
LIB_OBJ = $(OBJ_DIR)/element.o
GAME_OBJ = $(OBJ_DIR)/planete.o $(OBJ_DIR)/action.o $(OBJ_DIR)/evenement.o $(OBJ_DIR)/jeu.o \
           $(OBJ_DIR)/terminal.o $(OBJ_DIR)/ui.o $(OBJ_DIR)/carte.o $(OBJ_DIR)/main.o

.PHONY: all clean doc

all: program

# Programme principal
program: $(GAME_OBJ) $(LIB)
	$(CC) $(CFLAGS) $(GAME_OBJ) -L$(LIBS_DIR) -lElements -lm -o $@

# Compilation générique des objets (dépendances sur tous les headers)
$(OBJ_DIR)/%.o: $(SOURCE)/%.c $(wildcard $(HEADER)/*.h) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(HEADER) -c $< -o $@

# Création de la bibliothèque statique
$(LIB): $(LIB_OBJ) | $(LIBS_DIR)
	$(AR) $(ARFLAGS) $@ $^

$(OBJ_DIR) $(LIBS_DIR):
	mkdir -p $@

doc:
	doxygen Doxyfile

# Nettoyage des fichiers générés
clean:
	rm -f $(OBJ_DIR)/*.o $(LIB) program
