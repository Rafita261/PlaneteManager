/**
* @file evenement.c
* @brief Ce fichier contient les définitions des fonctions pour gérer les événements qui peuvent survenir sur la planète et Effets des événements sur la planète
*/


#include "../include/evenement.h"
#include <string.h>

/** @brief Association nom d'événement */
typedef struct {
    const char* nomEvenement;
    effet fonctionEffet;
} DescEvenement;

static const DescEvenement evenements[] = {
    {INCENDIE, incendie},
    {TEMPETE, tempete},
    {SECHERESSE, secheresse},
    {PLUIE, pluies_abondantes},
    {RAS, ras}
};

#define NB_EVENEMENTS ((int)(sizeof(evenements) / sizeof(evenements[0])))

/** @brief Augmente la pollution et reduit la ressource */
void incendie(Planete* planete) {
    modifierPlanete(planete, +50, -20, -30);
}

/** @brief Augmente la pollution et reduit l'energie */
void tempete(Planete* planete) {
    modifierPlanete(planete, +30, -30, -10);
}

/** @brief Augmente la pollution et reduit la ressource */
void secheresse(Planete* planete) {
    modifierPlanete(planete, +40, -10, -40);
}

/** @brief Augmente la pollution et reduit l'energie */
void pluies_abondantes(Planete* planete) {
    modifierPlanete(planete, -20, -10, +20);
}

/** @brief Ne fait rien */
void ras(Planete* planete) {
    (void)planete; // Aucun effet sur la planète
}

/**
* @brief Retrouve la fonction d'effet associée à un événement donné
* @return La fonction d'effet correspondante, ou NULL si l'événement n'est pas trouvé
*/
effet trouverEvenement(const char* nomEvenement) {
    for (int i = 0; i < NB_EVENEMENTS; i++) {
        if (strcmp(evenements[i].nomEvenement, nomEvenement) == 0) {
            return evenements[i].fonctionEffet;
        }
    }
    return NULL; // Événement non trouvé
}