/*
*@file action.h
*@brief Ce fichier contient les déclarations des fonctions pour gérer les actions choisis par le joueur
*
*/

#ifndef _ACTION_H_

#define _ACTION_H_
#include "../include/planete.h"

#define PLANTER_ARBRE "Planter un arbre"
#define TRANSITION_ENERGETIQUE "Transition énergétique"
#define RECHARGER_ENERGIE "Recharger l'énergie"
#define REDUCTION_DECHETS "Réduction des déchets"

typedef void (*effet)(Planete* planete);

void planterArbre(Planete* planete);
void transitionEnergetique(Planete* planete);
void reductionDechets(Planete* planete);
void rechargerEnergie(Planete* planete);

effet trouverAction(const char* nomAction);
int energieRequise(const char* nomAction);

#endif