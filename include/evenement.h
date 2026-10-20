#ifndef _EVENEMENT_H_

#define _EVENEMENT_H_
#include "../include/planete.h"

#define INCENDIE 0
#define TEMPETE 1
#define SECHERESSE 2
#define PLUIE 3

typedef void(*evenement)(Planete*) ;
Element evenementToElement(char* nomEvent) ;

void incendie(Planete* p) ;
void tempete(Planete* p);
void secheresse(Planete* p);
void pluie_abondantes(Planete* p)

const evenement aevenement_functions = {
    incendie,
    tempete,
    secheresse,
    pluie_abontantes
}


#endif