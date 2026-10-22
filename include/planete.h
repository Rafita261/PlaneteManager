/**
 * @file planete.h
 * @brief Structure Planete et fonctions de gestion associées.
 */
#ifndef _PLANETE_H_
#define _PLANETE_H_

#include "element.h"

#define MAX_POLLUTION 1000   /**< Pollution provoquant le game over */
#define MAX_RESSOURCES 1000  /**< Plafond des ressources naturelles */
#define MAX_ENERGIE 1000     /**< Plafond de l'énergie */

#define TYPE_ACTION "Action"
#define TYPE_EVENEMENT "Evenement"

/**
 * @brief Etat d'une planète et éléments qui lui sont associés.
 */
typedef struct Planete {
    char* nom;               /**< Nom de la planète (copie allouée) */
    int niveau_pollution;    /**< 0 .. MAX_POLLUTION */
    int niveau_energie;      /**< 0 .. MAX_ENERGIE */
    int niveau_ressource;    /**< 0 .. MAX_RESSOURCES */
    Element *actions;        /**< Liste des actions disponibles */
    Element *evenements;     /**< Liste des événements possibles */
} Planete;

Planete creerPlanete(const char *nom, int energie, int ressources, int pollution);
void ajouterAction(Planete *p, const char *nomAction);
void ajouterEvenement(Planete *p, const char *nomEvenement);
void afficherPlanete(const Planete p);
int estGameOver(const Planete p);
int verifierPlanete(const Planete p);
void modifierPlanete(Planete *p, int deltaPollution, int deltaRessources, int deltaEnergie);
void libererPlanete(Planete *p);

#endif
