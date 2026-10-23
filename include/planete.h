#ifndef _PLANETE_H_

#define _PLANETE_H_
#define MAX_POLLUTION 1000
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/element.h"

typedef struct Planete{
    char* nom ;
    int niveau_pollution ;
    int niveau_energie ;
    int niveau_ressource ;

    Element* actions ;
    Element* evenements ;
} Planete;

Planete* creerPlanete(char* nom, int niveau_pollution, int niveau_energie, int niveau_ressource);

void ajouterAction(Planete* planete, const char* nomAction);

void ajouterEvenement(Planete* planete, const char* nomEvenement);

void afficherPlanete(const Planete planete);

int estGameOver(const Planete planete);

int verifierPlanete(const Planete planete);

void modifierPlanete(Planete* planete, int deltaPollution, int deltaEnergie, int deltaRessource);

void libererPlanete(Planete* planete);

#endif