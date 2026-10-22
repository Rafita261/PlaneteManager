/**
 * @file action.h
 * @brief Actions écologiques et leurs effets sur une Planete.
 */
#ifndef _ACTION_H_
#define _ACTION_H_

#include "planete.h"

#define PLANTER_ARBRE "Planter des arbres"
#define TRANSITION_ENERGETIQUE "Transition énergétique"
#define REDUCTION_DECHETS "Réduction des déchets"
#define RECHARGER_ENERGIE "Recharger l'énergie"

/** @brief Pointeur vers une fonction d'effet sur une planète. */
typedef void (*effet)(Planete*);

void planter_arbre(Planete* p);
void transition_energetique(Planete* p);
void reduction_dechets(Planete* p);
void recharger_energie(Planete* p);

effet trouverAction(const char* nomAction);
int energieRequise(const char* nomAction);

#endif
