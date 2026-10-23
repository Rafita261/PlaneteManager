/** @brief Structure représentant le jeu */

#ifndef _JEU_H_

#define _JEU_H_
#include "../include/planete.h"
#include "../include/action.h"
#include "../include/evenement.h"
#include "../include/element.h"
#include <stdio.h>
#include <stdlib.h>

Element* selectionnerElementAleatoire(Element* liste);

int calculScoreV1(const Planete planete);
void tourDeJeu(Planete* planete, Element* action, Element* evenement);
void jeuComplet();

#endif