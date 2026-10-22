/**
 * @file element.h
 * @brief Liste chaînée d'Elements (actions ou événements climatiques).
 */
#ifndef _ELEMENT_H_
#define _ELEMENT_H_

/**
 * @brief Noeud de la liste chaînée.
 *
 * Le nom et le type sont des copies allouées dynamiquement :
 * la liste est propriétaire de ses chaînes.
 */
typedef struct Element {
    char* nom;                /**< Nom de l'élément (ex : "Incendie") */
    char* type;               /**< Type de l'élément (ex : "Evenement") */
    struct Element* suivant;  /**< Elément suivant, NULL en fin de liste */
} Element;

Element* creerNoeud(const char* nom, const char* type);
void ajouterElement(Element **liste, const char *nomElement, const char *nomType);
void ajouterElementTail(Element **liste, const char *nomElement, const char *nomType);
void supprimerElement(Element **liste, const char *nomElement);
void afficherElements(const Element *liste);
void libererListeElements(Element *liste);

int tailleListe(const Element *liste);
Element* rechercherElementNom(Element *liste, const char *nomElement);
Element* rechercherElementIndice(Element *liste, int indice);
Element* tirageAleatoireElement(Element *liste);

#endif
