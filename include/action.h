#ifndef _ACTION_H_

#define _ACTION_H_

#include "../include/planete.h"
#include "../include/element.h"

#define PLANTER_ARBRE 0
#define TRANSITION_ECOLOGIQUE 1
#define REDUCTION_DECHETS 2

typedef void(*action)(Planete*) ;

Element actionToElement(char*) ;

void planter_arbre(Planete* p) ;
void transition_ecologique(Planete* p) ;
void reduction_dechets(Planete* p) ;

const action action_functions = {
    planter_arbre,
    transition_ecologique,
    reduction_dechets
}

#endif