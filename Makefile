CC = gcc
CFLAGS = -Wall -Werror
AR = ar
ARFLAGS = rcs

LIBS_DIR = libs
OBJ_DIR = obj
SOURCE = src
HEADER = include

LIB = $(LIBS_DIR)/libelement.a
SOURCES = $(wildcard $(SOURCE)/*.c)
OBJECTS = $(patsubst $(SOURCE)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
LIB_OBJ = $(OBJ_DIR)/element.o
PROGRAM_OBJ = $(filter-out $(LIB_OBJ),$(OBJECTS))

# Programme principal
program: $(PROGRAM_OBJ) $(LIB)
	$(CC) $(CFLAGS) -I$(HEADER) $^ -o $@

# Création des objets
$(OBJ_DIR)/%.o: $(SOURCE)/%.c
	$(CC) $(CFLAGS) -I$(HEADER) -c $< -o $@

# Création de la bibliothèque statique
$(LIB): $(LIB_OBJ)
	$(AR) $(ARFLAGS) $@ $^

# Nettoyage des fichiers générés
clean:
	rm -f $(OBJ_DIR)/*.o $(LIB) program
