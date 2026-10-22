/**
 * @file element.c
 * @brief Implémentation de la liste chaînée d'Elements.
 */
#include "element.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Duplique une chaîne (équivalent portable de strdup).
 * @return La copie, ou NULL si l'allocation échoue.
 */
static char* copierChaine(const char* s) {
    if (s == NULL)
        return NULL;
    char* copie = malloc(strlen(s) + 1);
    if (copie != NULL)
        strcpy(copie, s);
    return copie;
}

/**
 * @brief Libère un noeud et les chaînes qu'il possède.
 */
static void libererNoeud(Element* e) {
    free(e->nom);
    free(e->type);
    free(e);
}

/**
 * @brief Crée un nouveau nœud de la liste des éléments.
 *
 * Alloue dynamiquement la mémoire nécessaire à un élément,
 * copie son nom et son type et initialise son pointeur vers le suivant.
 *
 * @param nom Nom de l'élément.
 * @param type Type de l'élément.
 *
 * @return Un pointeur vers le nouvel élément si l'allocation réussit,
 *         NULL en cas d'échec.
 */
Element* creerNoeud(const char* nom, const char* type) {
    if (nom == NULL || type == NULL)
        return NULL;

    Element *l = malloc(sizeof(Element));

    if (l == NULL)
        return NULL;

    l->nom = copierChaine(nom);
    l->type = copierChaine(type);
    l->suivant = NULL;

    if (l->nom == NULL || l->type == NULL) {
        libererNoeud(l);
        return NULL;
    }

    return l;
}

/**
 * @brief Ajoute un élément au début de la liste.
 *
 * Le nouvel élément devient la tête de la liste.
 *
 * @param liste Adresse du pointeur vers la tête de la liste.
 * @param nomElement Nom de l'élément à ajouter.
 * @param nomType Type de l'élément à ajouter.
 */
void ajouterElement(Element **liste,
                    const char *nomElement,
                    const char *nomType) {
    if (liste == NULL)
        return;

    Element *l = creerNoeud(nomElement, nomType);

    if (l == NULL)
        return;

    l->suivant = *liste;
    *liste = l;
}

/**
 * @brief Ajoute un élément à la fin de la liste.
 *
 * Si la liste est vide, le nouvel élément devient sa tête.
 * Sinon, l'élément est inséré après le dernier nœud.
 *
 * @param liste Adresse du pointeur vers la tête de la liste.
 * @param nomElement Nom de l'élément à ajouter.
 * @param nomType Type de l'élément à ajouter.
 */
void ajouterElementTail(Element **liste,
                        const char *nomElement,
                        const char *nomType) {
    if (liste == NULL)
        return;

    Element *l = creerNoeud(nomElement, nomType);

    if (l == NULL)
        return;

    if (*liste == NULL) {
        *liste = l;
        return;
    }

    Element *courant = *liste;

    while (courant->suivant != NULL) {
        courant = courant->suivant;
    }

    courant->suivant = l;
}

/**
 * @brief Supprime le premier élément correspondant au nom donné.
 *
 * Recherche le premier nœud dont le nom correspond à
 * @p nomElement, puis le retire de la liste et libère sa mémoire.
 *
 * @param liste Adresse du pointeur vers la tête de la liste.
 * @param nomElement Nom de l'élément à supprimer.
 */
void supprimerElement(Element **liste, const char *nomElement) {
    if (liste == NULL || *liste == NULL || nomElement == NULL)
        return;

    Element *courant = *liste;
    Element *precedent = NULL;

    while (courant != NULL &&
           strcmp(courant->nom, nomElement) != 0) {
        precedent = courant;
        courant = courant->suivant;
    }

    if (courant == NULL)
        return;

    if (precedent == NULL) {
        *liste = courant->suivant;
    } else {
        precedent->suivant = courant->suivant;
    }

    libererNoeud(courant);
}

/**
 * @brief Affiche tous les éléments de la liste.
 *
 * Chaque élément est affiché avec son nom et son type.
 *
 * @param liste Pointeur vers la tête de la liste.
 */
void afficherElements(const Element *liste) {
    const Element *courant = liste;

    if (courant == NULL)
        printf("(liste vide)\n");

    while (courant != NULL) {
        printf("Nom : %s | Type : %s\n",
               courant->nom,
               courant->type);

        courant = courant->suivant;
    }
}

/**
 * @brief Libère la mémoire occupée par toute la liste.
 *
 * Parcourt la liste et libère chaque nœud ainsi que ses chaînes.
 *
 * @param liste Pointeur vers la tête de la liste.
 */
void libererListeElements(Element *liste) {
    Element *courant = liste;

    while (courant != NULL) {
        Element *suivant = courant->suivant;
        libererNoeud(courant);
        courant = suivant;
    }
}

/**
 * @brief Compte le nombre d'éléments de la liste.
 * @param liste Pointeur vers la tête de la liste.
 * @return Le nombre d'éléments (0 si la liste est vide).
 */
int tailleListe(const Element *liste) {
    int n = 0;
    while (liste != NULL) {
        n++;
        liste = liste->suivant;
    }
    return n;
}

/**
 * @brief Recherche le premier élément portant le nom donné.
 * @param liste Pointeur vers la tête de la liste.
 * @param nomElement Nom recherché.
 * @return L'élément trouvé, ou NULL s'il n'existe pas.
 */
Element* rechercherElementNom(Element *liste, const char *nomElement) {
    if (nomElement == NULL)
        return NULL;

    while (liste != NULL) {
        if (strcmp(liste->nom, nomElement) == 0)
            return liste;
        liste = liste->suivant;
    }
    return NULL;
}

/**
 * @brief Retourne l'élément situé à l'indice donné (0 = tête).
 * @param liste Pointeur vers la tête de la liste.
 * @param indice Position de l'élément.
 * @return L'élément trouvé, ou NULL si l'indice est hors limites.
 */
Element* rechercherElementIndice(Element *liste, int indice) {
    if (indice < 0)
        return NULL;

    while (liste != NULL && indice > 0) {
        liste = liste->suivant;
        indice--;
    }
    return liste;
}

/**
 * @brief Tire un élément au hasard dans la liste.
 *
 * Le générateur doit avoir été initialisé avec srand() au préalable.
 *
 * @param liste Pointeur vers la tête de la liste.
 * @return Un élément aléatoire, ou NULL si la liste est vide.
 */
Element* tirageAleatoireElement(Element *liste) {
    int n = tailleListe(liste);
    if (n == 0)
        return NULL;
    return rechercherElementIndice(liste, rand() % n);
}
