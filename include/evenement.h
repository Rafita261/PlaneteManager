/**
 * @file evenement.h
 * @brief Evénements climatiques et leurs effets sur une Planete.
 */
#ifndef _EVENEMENT_H_
#define _EVENEMENT_H_

#include "action.h"  /* type effet */

#define INCENDIE "Incendie"
#define TEMPETE "Tempête"
#define SECHERESSE "Sécheresse"
#define PLUIE "Pluies abondantes"
#define RAS "RAS"

void incendie(Planete* p);
void tempete(Planete* p);
void secheresse(Planete* p);
void pluies_abondantes(Planete* p);
void ras(Planete* p);

effet trouverEvenement(const char* nomEvenement);

#endif
