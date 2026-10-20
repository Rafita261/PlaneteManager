#include <stdio.h>
#include "../include/element.h"

void afficher_int(void* nb){
    printf("%d", *((int*)nb));
}

int main(void) {

    Element *liste = NULL;

    // Ajout d'éléments au début
    ajouterElement(&liste, "Variable", "int");
    ajouterElement(&liste, "Fonction", "void");

    // Ajout d'un élément à la fin
    ajouterElementTail(&liste, "Constante", "float");

    printf("=== Liste initiale ===\n");
    afficherElements(liste);

    // Suppression d'un élément
    supprimerElement(&liste, "Variable");

    printf("\n=== Liste apres suppression ===\n");
    afficherElements(liste);

    // Libération de la mémoire
    libererListeElements(liste);
    liste = NULL;

    return 0;
}