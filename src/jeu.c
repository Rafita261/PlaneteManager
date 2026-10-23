/**
* @file jeu.c
* @brief Ce fichier contient les déclarations des fonctions pour gérer le jeu
*/

#include "../include/jeu.h"

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
    // Créer une planète initiale
    Planete* planete = creerPlanete("Terre", 200, 500, 300);

    // Boucle de jeu
    while (!estGameOver(*planete)) {
        // Sélectionner une action et un événement aléatoires
        Element* action = selectionnerElementAleatoire(planete->actions);
        Element* evenement = selectionnerElementAleatoire(planete->evenements);

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