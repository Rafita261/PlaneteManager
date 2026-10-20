CC = gcc
CFLAGS = -Wall -Werror
AR = ar
ARFLAGS = rcs

LIBS_DIR = libs
OBJ_DIR = obj
SOURCE = src
HEADER = include

LIB = $(LIBS_DIR)/libelement.a
OBJ = $(OBJ_DIR)/element.o

# Programme principal
program: $(SOURCE)/main.c $(LIB)
	$(CC) $(CFLAGS) $(SOURCE)/main.c -I$(HEADER) -L$(LIBS_DIR) -lelement -o program

# Création de l'objet element
$(OBJ_DIR)/element.o: $(HEADER)/element.h $(SOURCE)/element.c
	$(CC) $(CFLAGS) -I$(HEADER) -c $(SOURCE)/element.c -o $@

# Création de la bibliothèque statique
$(LIB): $(OBJ)
	$(AR) $(ARFLAGS) $(LIB) $(OBJ)

# Nettoyage des fichiers générés
clean:
	rm -f $(OBJ_DIR)/*.o $(LIB) program
