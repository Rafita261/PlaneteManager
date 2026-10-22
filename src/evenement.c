/**
 * @file evenement.c
 * @brief Effets des événements climatiques.
 */
#include "evenement.h"
#include <string.h>

/** @brief Association nom d'événement / effet. */
typedef struct {
    const char* nom;
    effet fonction;
} DescEvenement;

static const DescEvenement evenements[] = {
    { INCENDIE,   incendie },
    { TEMPETE,    tempete },
    { SECHERESSE, secheresse },
    { PLUIE,      pluies_abondantes },
    { RAS,        ras },
};

#define NB_EVENEMENTS ((int)(sizeof(evenements) / sizeof(evenements[0])))

/** @brief Augmente la pollution et réduit les ressources. */
void incendie(Planete* p) {
    modifierPlanete(p, +80, -60, 0);
}

/** @brief Augmente la pollution et réduit modérément les ressources. */
void tempete(Planete* p) {
    modifierPlanete(p, +40, -30, 0);
}

/** @brief Réduit drastiquement les ressources. */
void secheresse(Planete* p) {
    modifierPlanete(p, 0, -120, 0);
}

/** @brief Réduit la pollution et augmente les ressources. */
void pluies_abondantes(Planete* p) {
    modifierPlanete(p, -30, +60, 0);
}

/** @brief Rien à signaler. */
void ras(Planete* p) {
    (void)p;
}

/**
 * @brief Retrouve la fonction d'effet associée à un nom d'événement.
 * @return La fonction, ou NULL si l'événement est inconnu.
 */
effet trouverEvenement(const char* nomEvenement) {
    for (int i = 0; nomEvenement != NULL && i < NB_EVENEMENTS; i++)
        if (strcmp(evenements[i].nom, nomEvenement) == 0)
            return evenements[i].fonction;
    return NULL;
}
