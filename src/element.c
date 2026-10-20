#include "../include/element.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Crée un nouveau nœud de la liste des éléments.
 *
 * Alloue dynamiquement la mémoire nécessaire à un élément,
 * initialise son nom, son type et son pointeur vers le suivant.
 *
 * @param nom Nom de l'élément.
 * @param type Type de l'élément.
 *
 * @return Un pointeur vers le nouvel élément si l'allocation réussit,
 *         NULL en cas d'échec.
 */
Element* creerNoeud(char* nom, char* type) {
    Element *l = malloc(sizeof(Element));

    if (l == NULL)
        return NULL;

    l->nom = nom;
    l->type = type;
    l->suivant = NULL;

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

    Element *l = creerNoeud((char *)nomElement, (char *)nomType);

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

    Element *l = creerNoeud((char *)nomElement, (char *)nomType);

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

    free(courant);
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
 * Parcourt la liste et libère chaque nœud.
 *
 * @param liste Pointeur vers la tête de la liste.
 */
void libererListeElements(Element *liste) {
    Element *courant = liste;

    while (courant != NULL) {
        Element *suivant = courant->suivant;
        free(courant);
        courant = suivant;
    }
}