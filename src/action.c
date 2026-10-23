/* @file action.c
 * @brief Ce fichier contient les définitions des fonctions pour gérer les actions choisis par le joueur et Effets des actions sur la planète
 */

 #include "../include/action.h"
 #include <string.h>


 /** @brief Association nom d'action -> fonction d'effet */
typedef struct {
    const char* nomAction;
    effet fonctionEffet;
    int coutEnergie;
} DescAction;

static const DescAction actions[] = {
    {PLANTER_ARBRE, planterArbre, 30},
    {TRANSITION_ENERGETIQUE, transitionEnergetique, 100},
    {RECHARGER_ENERGIE, rechargerEnergie, 0},
    {REDUCTION_DECHETS, reductionDechets, 15}
};

#define NB_ACTIONS ((int)(sizeof(actions) / sizeof(actions[0])))

/** @brief Réduit la pollution et augmente un peu de ressources  */
void planterArbre(Planete* planete) {
    modifierPlanete(planete, -40, +20, -30);
}

/** @brief Réduit fortement la pollution et coute beaucoup d'énergie */
void transitionEnergetique(Planete* planete) {
    modifierPlanete(planete, -100, -50, -100);
}

/** @brief Regagne de l'energie au prix de pollution et de ressources */
void rechargerEnergie(Planete* planete) {
    modifierPlanete(planete, +20, -20, +100);
}

/** @brief Réduit un peu la pollution et regagne un peu d'énergie */
void reductionDechets(Planete* planete) {
    modifierPlanete(planete, -30, +10, -20);
}

/**
* @brief Trouve la fonction d'effet associée à une action donnée
* @return La fonction d'effet correspondante, ou NULL si l'action n'est pas trouvée
*/
effet trouverAction(const char* nomAction) {
    for (int i = 0; i < NB_ACTIONS; i++) {
        if (strcmp(actions[i].nomAction, nomAction) == 0) {
            return actions[i].fonctionEffet;
        }
    }
    return NULL; // Action non trouvée
}


/** @brief retourne l'énergie requise pour une action donnée */
int energieRequise(const char* nomAction) {
    for (int i = 0; i < NB_ACTIONS; i++) {
        if (strcmp(actions[i].nomAction, nomAction) == 0) {
            return actions[i].coutEnergie;
        }
    }
    return -1; // Action non trouvée
}