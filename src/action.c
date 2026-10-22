/**
 * @file action.c
 * @brief Effets des actions écologiques.
 */
#include "action.h"
#include <string.h>

/** @brief Association nom d'action / effet / coût en énergie. */
typedef struct {
    const char* nom;
    effet fonction;
    int cout;
} DescAction;

static const DescAction actions[] = {
    { PLANTER_ARBRE,          planter_arbre,          30 },
    { TRANSITION_ENERGETIQUE, transition_energetique, 120 },
    { REDUCTION_DECHETS,      reduction_dechets,      15 },
    { RECHARGER_ENERGIE,      recharger_energie,      0 },
};

#define NB_ACTIONS ((int)(sizeof(actions) / sizeof(actions[0])))

/** @brief Réduit la pollution et augmente un peu les ressources. */
void planter_arbre(Planete* p) {
    modifierPlanete(p, -40, +20, -30);
}

/** @brief Réduit fortement la pollution, coûte beaucoup d'énergie. */
void transition_energetique(Planete* p) {
    modifierPlanete(p, -120, 0, -120);
}

/** @brief Réduit modérément la pollution, faible coût en énergie. */
void reduction_dechets(Planete* p) {
    modifierPlanete(p, -25, 0, -15);
}

/** @brief Regagne de l'énergie au prix d'un peu de pollution et de ressources. */
void recharger_energie(Planete* p) {
    modifierPlanete(p, +20, -20, +100);
}

/**
 * @brief Retrouve la fonction d'effet associée à un nom d'action.
 * @return La fonction, ou NULL si l'action est inconnue.
 */
effet trouverAction(const char* nomAction) {
    for (int i = 0; nomAction != NULL && i < NB_ACTIONS; i++)
        if (strcmp(actions[i].nom, nomAction) == 0)
            return actions[i].fonction;
    return NULL;
}

/**
 * @brief Retourne l'énergie nécessaire pour réaliser une action.
 * @return Le coût, ou -1 si l'action est inconnue.
 */
int energieRequise(const char* nomAction) {
    for (int i = 0; nomAction != NULL && i < NB_ACTIONS; i++)
        if (strcmp(actions[i].nom, nomAction) == 0)
            return actions[i].cout;
    return -1;
}
