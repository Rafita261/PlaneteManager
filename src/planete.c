#include "../include/planete.h"

Planete* creerPlanete(char* nom, int niveau_pollution, int niveau_energie, int niveau_ressource) {
    Planete* planete = (Planete*)malloc(sizeof(Planete));
    if (planete == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour la planète.\n");
        exit(EXIT_FAILURE);
    }

    planete->nom = strdup(nom);
    planete->niveau_pollution = niveau_pollution;
    planete->niveau_energie = niveau_energie;
    planete->niveau_ressource = niveau_ressource;
    planete->actions = NULL;
    planete->evenements = NULL;

    return planete;
}

void ajouterAction(Planete* planete, const char* nomAction) {
    ajouterElement(&(planete->actions), nomAction, "Action");
}

void ajouterEvenement(Planete* planete, const char* nomEvenement) {
    ajouterElement(&(planete->evenements), nomEvenement, "Evenement");
}

void afficherPlanete(const Planete planete) {
    printf("Nom de la planète: %s\n", planete.nom);
    printf("Niveau de pollution: %d\n", planete.niveau_pollution);
    printf("Niveau d'énergie: %d\n", planete.niveau_energie);
    printf("Niveau de ressources: %d\n", planete.niveau_ressource);

    printf("\nActions:\n");
    afficherElements(planete.actions);

    printf("\nÉvénements:\n");
    afficherElements(planete.evenements);
}

int estGameOver(const Planete planete) {
    return (planete.niveau_pollution >= MAX_POLLUTION || planete.niveau_energie <= 0 || planete.niveau_ressource <= 0);
}

int verifierPlanete(const Planete planete) {
    if (planete.niveau_pollution < 0 || planete.niveau_energie < 0 || planete.niveau_ressource < 0) {
        return 0;
    }
    return 1; 
}

void modifierPlanete(Planete* planete, int deltaPollution, int deltaEnergie, int deltaRessource) {
    planete->niveau_pollution += deltaPollution;
    planete->niveau_energie += deltaEnergie;
    planete->niveau_ressource += deltaRessource;

    if (planete->niveau_pollution < 0) planete->niveau_pollution = 0;
    if (planete->niveau_energie < 0) planete->niveau_energie = 0;
    if (planete->niveau_ressource < 0) planete->niveau_ressource = 0;
}

void libererPlanete(Planete* planete) {
    if (planete) {
        free(planete->nom);
        libererListeElements(planete->actions);
        libererListeElements(planete->evenements);
        free(planete);
    }
}