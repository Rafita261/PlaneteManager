/**
 * @file planete.c
 * @brief Implémentation de la gestion d'une Planete.
 */
#include "planete.h"
#include "color.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Borne une valeur dans l'intervalle [min, max].
 */
static int borner(int v, int min, int max) {
    if (v < min) return min;
    if (v > max) return max;
    return v;
}

/**
 * @brief Crée une planète sans actions ni événements.
 *
 * @param nom Nom de la planète (copié).
 * @param energie Niveau d'énergie initial.
 * @param ressources Niveau de ressources initial.
 * @param pollution Niveau de pollution initial.
 * @return La planète initialisée (nom à NULL si l'allocation échoue).
 */
Planete creerPlanete(const char *nom, int energie, int ressources, int pollution) {
    Planete p;

    p.nom = NULL;
    if (nom != NULL) {
        p.nom = malloc(strlen(nom) + 1);
        if (p.nom != NULL)
            strcpy(p.nom, nom);
    }
    p.niveau_energie = borner(energie, 0, MAX_ENERGIE);
    p.niveau_ressource = borner(ressources, 0, MAX_RESSOURCES);
    p.niveau_pollution = borner(pollution, 0, MAX_POLLUTION);
    p.actions = NULL;
    p.evenements = NULL;

    return p;
}

/**
 * @brief Ajoute une action (en fin de liste) à la planète.
 * @param p Planète à modifier.
 * @param nomAction Nom de l'action.
 */
void ajouterAction(Planete *p, const char *nomAction) {
    if (p == NULL)
        return;
    ajouterElementTail(&p->actions, nomAction, TYPE_ACTION);
}

/**
 * @brief Ajoute un événement (en fin de liste) à la planète.
 * @param p Planète à modifier.
 * @param nomEvenement Nom de l'événement.
 */
void ajouterEvenement(Planete *p, const char *nomEvenement) {
    if (p == NULL)
        return;
    ajouterElementTail(&p->evenements, nomEvenement, TYPE_EVENEMENT);
}

/**
 * @brief Affiche les indicateurs, actions et événements d'une planète.
 * @param p Planète à afficher.
 */
void afficherPlanete(const Planete p) {
    printf("############################################\n");
    printf("  PLANETE     : %s\n", p.nom != NULL ? p.nom : "(sans nom)");
    printf("  Pollution   : %d / %d\n", p.niveau_pollution, MAX_POLLUTION);
    printf("  Energie     : %d / %d\n", p.niveau_energie, MAX_ENERGIE);
    printf("  Ressources  : %d / %d\n", p.niveau_ressource, MAX_RESSOURCES);
    printf("############################################\n");
    printf("Actions :\n");
    afficherElements(p.actions);
    printf("Evenements :\n");
    afficherElements(p.evenements);
}

/**
 * @brief Indique, sans rien afficher, si la planète est en game over.
 * @param p Planète à tester.
 * @return 1 en cas de game over, 0 sinon.
 */
int estGameOver(const Planete p) {
    return p.niveau_pollution >= MAX_POLLUTION || p.niveau_ressource <= 0 || p.niveau_energie <= 0;
}

/**
 * @brief Vérifie si la planète est en game over.
 *
 * Défaite si les ressources tombent à 0 ou si la pollution atteint
 * MAX_POLLUTION.
 *
 * @param p Planète à vérifier.
 * @return 1 en cas de game over, 0 sinon.
 */
int verifierPlanete(const Planete p) {
    if (estGameOver(p)) {
        printf(ROUGE GRAS "GAME OVER !!!!!!" RESET "\n");
        if (p.niveau_pollution >= MAX_POLLUTION)
            printf(ROUGE "La pollution a atteint %d.\n" RESET, MAX_POLLUTION);
        if (p.niveau_ressource <= 0)
            printf(ROUGE "Les ressources sont epuisees.\n" RESET);
        if (p.niveau_energie <= 0)
            printf(ROUGE "L'energies est epuisee.\n" RESET);
        return 1;
    }
    return 0;
}

/**
 * @brief Applique des variations aux indicateurs de la planète.
 *
 * Les valeurs obtenues sont bornées entre 0 et leur maximum.
 *
 * @param p Planète à modifier.
 * @param deltaPollution Variation de la pollution.
 * @param deltaRessources Variation des ressources.
 * @param deltaEnergie Variation de l'énergie.
 */
void modifierPlanete(Planete *p, int deltaPollution, int deltaRessources, int deltaEnergie) {
    if (p == NULL)
        return;
#ifdef DEBUG
    printf("[DEBUG] %s : pollution %+d, ressources %+d, energie %+d\n",
           p->nom, deltaPollution, deltaRessources, deltaEnergie);
#endif
    p->niveau_pollution = borner(p->niveau_pollution + deltaPollution, 0, MAX_POLLUTION);
    p->niveau_ressource = borner(p->niveau_ressource + deltaRessources, 0, MAX_RESSOURCES);
    p->niveau_energie = borner(p->niveau_energie + deltaEnergie, 0, MAX_ENERGIE);
}

/**
 * @brief Libère toute la mémoire associée à la planète.
 * @param p Planète à libérer.
 */
void libererPlanete(Planete *p) {
    if (p == NULL)
        return;
    free(p->nom);
    libererListeElements(p->actions);
    libererListeElements(p->evenements);
    p->nom = NULL;
    p->actions = NULL;
    p->evenements = NULL;
}
