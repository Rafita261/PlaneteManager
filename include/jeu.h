/**
 * @file jeu.h
 * @brief Logique d'un tour de jeu et d'une partie complète.
 */
#ifndef _JEU_H_
#define _JEU_H_

#include "planete.h"

#define NB_TOURS_VICTOIRE 20  /**< Tours à survivre pour gagner */

/**
 * @brief Stratégie de calcul du score d'une planète.
 *
 * Plus le score est élevé, meilleur est l'état de la planète.
 * La stratégie sert à choisir l'action à jouer en mode automatique.
 */
typedef int (*calculScore)(const Planete*);

int calculScoreV1(const Planete* p);
int calculScoreV2(const Planete* p);

int initialiserPlanete(Planete* p);
Element* selectionnerElementAleatoire(Element *liste);
Element* debutTour(Planete* p);
int appliquerAction(Planete* p, const Element* action);
Element* choisirAction(const Planete* p, calculScore strategie);

int tourDeJeu(Planete *p, int numeroTour, calculScore strategie);
void jeuComplet(calculScore strategie);
void jeuDemo(calculScore strategie);
void jeuInteractif(void);

#endif
