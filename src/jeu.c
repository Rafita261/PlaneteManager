/**
* @file jeu.c
* @brief Ce fichier contient les déclarations des fonctions pour gérer le jeu
*/

#include "../include/jeu.h"

static int testerFonctionsPlanete(void){
    Planete* planete = creerPlanete("Test", 10, 20, 30);
    if (planete == NULL) {
        fprintf(stderr, "Échec du test: création de la planète.\n");
        return 0;
    }

    int succes = 0;
    if (planete->niveau_pollution != 10 || planete->niveau_energie != 20 ||
        planete->niveau_ressource != 30 || planete->actions != NULL ||
        planete->evenements != NULL || !verifierPlanete(*planete) ||
        estGameOver(*planete)) {
        fprintf(stderr, "Échec du test: initialisation de la planète.\n");
        goto nettoyage;
    }

    ajouterAction(planete, PLANTER_ARBRE);
    ajouterAction(planete, TRANSITION_ENERGETIQUE);
    ajouterAction(planete, RECHARGER_ENERGIE);
    ajouterAction(planete, REDUCTION_DECHETS);
    ajouterEvenement(planete, INCENDIE);
    ajouterEvenement(planete, TEMPETE);
    ajouterEvenement(planete, SECHERESSE);
    ajouterEvenement(planete, PLUIE);
    ajouterEvenement(planete, RAS);
    if (planete->actions == NULL || planete->evenements == NULL) {
        fprintf(stderr, "Échec du test: ajout d'une action ou d'un événement.\n");
        goto nettoyage;
    }

    modifierPlanete(planete, -15, -25, -35);
    if (planete->niveau_pollution != 0 || planete->niveau_energie != 0 ||
        planete->niveau_ressource != 0 || !verifierPlanete(*planete) ||
        !estGameOver(*planete)) {
        fprintf(stderr, "Échec du test: bornes minimales et game over.\n");
        goto nettoyage;
    }

    modifierPlanete(planete, 0, 1, 1);
    if (estGameOver(*planete)) {
        fprintf(stderr, "Échec du test: état actif de la planète.\n");
        goto nettoyage;
    }

    modifierPlanete(planete, MAX_POLLUTION, 0, 0);
    if (!estGameOver(*planete)) {
        fprintf(stderr, "Échec du test: seuil maximal de pollution.\n");
        goto nettoyage;
    }

    succes = 1;

nettoyage:
    libererPlanete(planete);
    if (succes) {
        printf("Test des fonctions planète réussi.\n");
    }
    return succes;
}

Element* selectionnerElementAleatoire(Element* liste){
    if (liste == NULL) {
        return NULL;
    }

    // Compter le nombre d'éléments dans la liste
    int count = 0;
    Element* current = liste;
    while (current != NULL) {
        count++;
        current = current->suivant;
    }

    // Générer un index aléatoire
    int randomIndex = rand() % count;

    // Parcourir la liste jusqu'à l'index aléatoire
    current = liste;
    for (int i = 0; i < randomIndex; i++) {
        current = current->suivant;
    }

    return current;
}

int calculScoreV1(const Planete planete){
    // Score basé sur la pollution, l'énergie et les ressources
    int score = 0;
    score += (MAX_POLLUTION - planete.niveau_pollution); // Moins de pollution = meilleur score
    score += planete.niveau_energie; // Plus d'énergie = meilleur score
    score += planete.niveau_ressource; // Plus de ressources = meilleur score

    return score;
}
void tourDeJeu(Planete* planete, Element* action, Element* evenement){
    if (planete == NULL || action == NULL || evenement == NULL) {
        fprintf(stderr, "Erreur: planete, action ou evenement est NULL.\n");
        return;
    }

    // Appliquer l'effet de l'action
    effet effetAction = trouverAction(action->nom);
    if (effetAction != NULL) {
        effetAction(planete);
    } else {
        fprintf(stderr, "Erreur: Action '%s' non trouvée.\n", action->nom);
    }

    // Appliquer l'effet de l'événement
    effet effetEvenement = trouverEvenement(evenement->nom);
    if (effetEvenement != NULL) {
        effetEvenement(planete);
    } else {
        fprintf(stderr, "Erreur: Evenement '%s' non trouvé.\n", evenement->nom);
    }
}
void jeuComplet(){
    if (!testerFonctionsPlanete()) {
        return;
    }

    // Créer une planète initiale
    Planete* planete = creerPlanete("Terre", 200, 500, 300);
    if (planete == NULL) {
        fprintf(stderr, "Erreur: impossible de créer la planète.\n");
        return;
    }

    ajouterAction(planete, PLANTER_ARBRE);
    ajouterAction(planete, TRANSITION_ENERGETIQUE);
    ajouterAction(planete, RECHARGER_ENERGIE);
    ajouterAction(planete, REDUCTION_DECHETS);
    ajouterEvenement(planete, INCENDIE);
    ajouterEvenement(planete, TEMPETE);
    ajouterEvenement(planete, SECHERESSE);
    ajouterEvenement(planete, PLUIE);
    ajouterEvenement(planete, RAS);

    /* Vérifier les données nécessaires avant de démarrer la partie. */
    if (planete->actions == NULL || planete->evenements == NULL) {
        fprintf(stderr, "Test impossible: ");
        if (planete->actions == NULL && planete->evenements == NULL) {
            fprintf(stderr, "aucune action ni aucun événement n'est chargé.\n");
        } else if (planete->actions == NULL) {
            fprintf(stderr, "aucune action n'est chargée.\n");
        } else {
            fprintf(stderr, "aucun événement n'est chargé.\n");
        }
        fprintf(stderr, "Vérifiez l'initialisation des listes avant de lancer le jeu.\n");
        libererPlanete(planete);
        return;
    }

    // Boucle de jeu
    while (!estGameOver(*planete)) {
        // Sélectionner une action et un événement aléatoires
        Element* action = selectionnerElementAleatoire(planete->actions);
        Element* evenement = selectionnerElementAleatoire(planete->evenements);

        if (action == NULL || evenement == NULL) {
            fprintf(stderr, "Test interrompu: une liste d'actions ou d'événements est vide.\n");
            libererPlanete(planete);
            return;
        }

        // Afficher l'état actuel de la planète
        afficherPlanete(*planete);

        // Afficher l'action et l'événement choisis
        printf("\nAction choisie: %s\n", action->nom);
        printf("Événement survenu: %s\n", evenement->nom);

        // Effectuer le tour de jeu
        tourDeJeu(planete, action, evenement);

        // Vérifier si la planète est dans un état valide
        if (!verifierPlanete(*planete)) {
            fprintf(stderr, "Erreur: La planète est dans un état invalide.\n");
            break;
        }
    }

    // Afficher le score final
    int scoreFinal = calculScoreV1(*planete);
    printf("\n=== Game Over ===\n");
    printf("Score final: %d\n", scoreFinal);

    // Libérer la mémoire allouée pour la planète
    libererPlanete(planete);
}