/**
 * @file carte.h
 * @brief Carte de la planète en pixel art et tableau de bord.
 */
#ifndef _CARTE_H_
#define _CARTE_H_

#include "planete.h"

/** @brief Ce qu'il faut afficher à l'écran. */
typedef struct {
    const Planete* planete;  /**< Planète à dessiner */
    int tour;                /**< Tour courant (0 : pas de partie en cours) */
    int tourMax;             /**< Nombre de tours pour gagner */
    const char* evenement;   /**< Evénement du tour (NULL : aucun) */
    const char* action;      /**< Dernière action jouée (NULL : aucune) */
    const char* message;     /**< Message libre (peut contenir des couleurs) */
} EtatAffichage;

void afficherTableau(const EtatAffichage* e);
void enteteTableau(const void* etat);

#endif
