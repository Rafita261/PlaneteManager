/**
* @file evenement.h
* @brief Ce fichier contient les déclarations des fonctions pour gérer les événements qui peuvent survenir sur la planète
*/


#ifndef _EVENEMENT_H_

#define _EVENEMENT_H_
#include "../include/action.h"

#define INCENDIE "Incendie"
#define TEMPETE "Tempête"
#define SECHERESSE "Sécheresse"
#define PLUIE "Pluie abondante"
#define RAS "Rien à signaler"

void incendie(Planete* planete);
void tempete(Planete* planete);
void secheresse(Planete* planete);
void pluies_abondantes(Planete* planete);
void ras(Planete* planete);

effet trouverEvenement(const char* nomEvenement);

#endif