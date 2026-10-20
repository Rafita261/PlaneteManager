#ifndef _PLANETE_H_

#define _PLANETE_H_

#define MAX_POLLUTION 1000
#include "../include/element.h"
#include "../include/evenement.h"
#include "../include/action.h"
#include "../include/color.h"
#include <stdio.h>
#include <string.h>

typedef struct Planete{
    char* nom ;
    int niveau_pollution ;
    int niveau_energie ;
    int niveau_ressource ;
    Element *actions ;
    Element *evenements ;
} Planete;

Planete creerPlanete(char *nom, int energie, int ressources, int pollution);
void ajouterAction(Planete *p, const char *nomAction);
void ajouterEvenement(Planete *p, const char *nomEvenement);
void afficherPlanete(const Planete p);
int verifierPlanete(const Planete p);
void modifierPlanete(Planete *p);

#endif