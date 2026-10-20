#ifndef _ELEMENT_H_
#define _ELEMENT_H_

typedef struct Element {
    char* nom;
    char* type;

    struct Element* suivant;
} Element;

Element* creerNoeud(char* nom, char* type) ;
void ajouterElement(Element **liste, const char *nomElement, const char *nomType);
void ajouterElementTail(Element **liste, const char *nomElement, const char *nomType);
void supprimerElement(Element **liste, const char *nomElement);
void afficherElements(const Element *liste);
void libererListeElements(Element *liste);

#endif