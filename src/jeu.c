/**
 * @file jeu.c
 * @brief Implémentation des tours de jeu (texte, démo graphique, interactif).
 */
#include "jeu.h"
#include "action.h"
#include "evenement.h"
#include "carte.h"
#include "ui.h"
#include "terminal.h"
#include "color.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OPTIONS 16
#define TAILLE_DETAIL 256
#define DELAI_DEMO_MS 1300

/**
 * @brief Score V1 : équilibre simple ressources / pollution.
 */
int calculScoreV1(const Planete* p) {
    return p->niveau_ressource - p->niveau_pollution;
}

/**
 * @brief Score V2 : prudente, pénalise fortement la pollution et
 * valorise la réserve d'énergie.
 */
int calculScoreV2(const Planete* p) {
    return 2 * p->niveau_ressource - 3 * p->niveau_pollution + p->niveau_energie / 2;
}

/**
 * @brief Crée la planète de départ avec ses actions et événements.
 * @param p Planète à initialiser.
 * @return 1 en cas de succès, 0 si l'allocation échoue.
 */
int initialiserPlanete(Planete* p) {
    *p = creerPlanete("Terre", 300, 500, 300);
    if (p->nom == NULL) {
        fprintf(stderr, "Erreur : allocation de la planete impossible\n");
        return 0;
    }

    ajouterAction(p, PLANTER_ARBRE);
    ajouterAction(p, TRANSITION_ENERGETIQUE);
    ajouterAction(p, REDUCTION_DECHETS);
    ajouterAction(p, RECHARGER_ENERGIE);

    ajouterEvenement(p, INCENDIE);
    ajouterEvenement(p, TEMPETE);
    ajouterEvenement(p, SECHERESSE);
    ajouterEvenement(p, PLUIE);
    ajouterEvenement(p, RAS);
    return 1;
}

/**
 * @brief Sélectionne un élément aléatoire dans une liste chaînée.
 * @return L'élément tiré, ou NULL si la liste est vide.
 */
Element* selectionnerElementAleatoire(Element *liste) {
    return tirageAleatoireElement(liste);
}

/**
 * @brief Début de tour : usure naturelle puis événement aléatoire.
 * @param p Planète à faire évoluer.
 * @return L'événement appliqué (NULL si aucun).
 */
Element* debutTour(Planete* p) {
    /* Activité de la population : pollue, consomme, produit de l'énergie */
    modifierPlanete(p, +20, -15, +25);

    Element* ev = selectionnerElementAleatoire(p->evenements);
    effet f = ev != NULL ? trouverEvenement(ev->nom) : NULL;
    if (f != NULL)
        f(p);
    return ev;
}

/**
 * @brief Applique une action si la planète a assez d'énergie.
 * @return 1 si l'action a été appliquée, 0 sinon.
 */
int appliquerAction(Planete* p, const Element* action) {
    if (action == NULL)
        return 0;
    effet f = trouverAction(action->nom);
    int cout = energieRequise(action->nom);
    if (f == NULL || cout < 0 || cout > p->niveau_energie)
        return 0;
    f(p);
    return 1;
}

/**
 * @brief Choisit l'action réalisable qui maximise le score.
 *
 * Chaque action est simulée sur une copie des indicateurs de la planète.
 *
 * @param p Planète courante.
 * @param strategie Fonction de calcul du score.
 * @return L'action retenue, ou NULL si aucune n'est réalisable.
 */
Element* choisirAction(const Planete* p, calculScore strategie) {
    Element* meilleure = NULL;
    int meilleurScore = 0;

    for (Element* a = p->actions; a != NULL; a = a->suivant) {
        effet f = trouverAction(a->nom);
        int cout = energieRequise(a->nom);
        if (f == NULL || cout < 0 || cout > p->niveau_energie)
            continue;

        Planete simulation = *p;  /* copie : seuls les entiers sont modifiés */
        f(&simulation);
        int score = strategie(&simulation);

        if (meilleure == NULL || score > meilleurScore) {
            meilleure = a;
            meilleurScore = score;
        }
    }
    return meilleure;
}

/**
 * @brief Simule un tour en mode texte : événement puis meilleure action.
 *
 * @param p Planète à faire évoluer.
 * @param numeroTour Numéro du tour (pour l'affichage).
 * @param strategie Fonction de calcul du score utilisée pour choisir l'action.
 * @return 1 si la planète est en game over à la fin du tour, 0 sinon.
 */
int tourDeJeu(Planete *p, int numeroTour, calculScore strategie) {
    printf(GRAS "\n----- Tour %d -----\n" RESET, numeroTour);

    Element* ev = debutTour(p);
    if (ev != NULL)
        printf(JAUNE "Evenement : %s\n" RESET, ev->nom);

    Element* act = choisirAction(p, strategie);
    if (appliquerAction(p, act))
        printf(VERT "Action    : %s\n" RESET, act->nom);
    else
        printf(VERT "Action    : aucune (energie insuffisante)\n" RESET);

    printf("Pollution %d | Ressources %d | Energie %d | Score %d\n",
           p->niveau_pollution, p->niveau_ressource, p->niveau_energie,
           strategie(p));

    return verifierPlanete(*p);
}

/**
 * @brief Joue une partie complète automatique en mode texte.
 * @param strategie Fonction de calcul du score.
 */
void jeuComplet(calculScore strategie) {
    Planete p;
    if (!initialiserPlanete(&p))
        return;

    printf(GRAS "=== Etat initial ===\n" RESET);
    afficherPlanete(p);

    int gameOver = 0;
    int tour;
    for (tour = 1; tour <= NB_TOURS_VICTOIRE && !gameOver; tour++)
        gameOver = tourDeJeu(&p, tour, strategie);

    printf(GRAS "\n=== Resultat final ===\n" RESET);
    if (gameOver)
        printf(ROUGE GRAS "Defaite au tour %d.\n" RESET, tour - 1);
    else
        printf(VERT GRAS "Victoire : la planete a tenu %d tours !\n" RESET, NB_TOURS_VICTOIRE);
    afficherPlanete(p);

    libererPlanete(&p);
}

/** @brief Raison lisible d'un game over. */
static const char* raisonGameOver(const Planete* p) {
    if (p->niveau_pollution >= MAX_POLLUTION)
        return "la pollution a tout envahi";
    return "les ressources sont épuisées";
}

/** @brief Ecrit le message de fin de partie dans @p msg. */
static void messageFin(char* msg, size_t n, const Planete* p, int gameOver, int abandon, int tour) {
    if (abandon)
        snprintf(msg, n, JAUNE GRAS "Partie abandonnée au tour %d." RESET, tour);
    else if (gameOver)
        snprintf(msg, n, ROUGE GRAS "GAME OVER au tour %d : %s !" RESET, tour, raisonGameOver(p));
    else
        snprintf(msg, n, VERT GRAS "VICTOIRE ! La planète a tenu %d tours." RESET, NB_TOURS_VICTOIRE);
}

/** @brief Affiche l'écran final et attend une touche. */
static void ecranFinal(const EtatAffichage* etat) {
    printf("\033[H");
    afficherTableau(etat);
    printf("\033[K\n  " DIM "Appuyez sur une touche pour revenir au menu..." RESET "\033[J");
    fflush(stdout);
    lireTouche();
}

/**
 * @brief Partie automatique avec la carte en pixel art (q pour arrêter).
 * @param strategie Fonction de calcul du score.
 */
void jeuDemo(calculScore strategie) {
    Planete p;
    if (!initialiserPlanete(&p))
        return;

    char msg[256];
    EtatAffichage etat = { &p, 0, NB_TOURS_VICTOIRE, NULL, NULL,
                           DIM "Démo automatique — q pour arrêter" RESET };
    int gameOver = 0, abandon = 0, tour = 0;

    while (tour < NB_TOURS_VICTOIRE && !gameOver && !abandon) {
        tour++;
        Element* ev = debutTour(&p);
        etat.tour = tour;
        etat.evenement = ev != NULL ? ev->nom : NULL;

        gameOver = estGameOver(p);
        if (!gameOver) {
            Element* act = choisirAction(&p, strategie);
            etat.action = appliquerAction(&p, act) ? act->nom : "aucune";
            gameOver = estGameOver(p);
        }

        printf("\033[H");
        afficherTableau(&etat);
        printf("\033[J");
        fflush(stdout);

        int t = lireToucheDelai(DELAI_DEMO_MS);
        abandon = (t == TOUCHE_QUITTER || t == TOUCHE_ECHAP);
    }

    messageFin(msg, sizeof(msg), &p, gameOver, abandon, tour);
    etat.message = msg;
    ecranFinal(&etat);
    libererPlanete(&p);
}

/** @brief Couleur associée à une action dans le menu. */
static void couleurAction(const char* nom, OptionMenu* o) {
    int r = 200, g = 200, b = 200;
    if (strcmp(nom, PLANTER_ARBRE) == 0)               { r = 80;  g = 200; b = 90;  }
    else if (strcmp(nom, TRANSITION_ENERGETIQUE) == 0) { r = 60;  g = 190; b = 225; }
    else if (strcmp(nom, REDUCTION_DECHETS) == 0)      { r = 235; g = 195; b = 60;  }
    else if (strcmp(nom, RECHARGER_ENERGIE) == 0)      { r = 205; g = 120; b = 235; }
    o->r = r; o->g = g; o->b = b;
}

/** @brief Ajoute une variation colorée (vert = bon, rouge = mauvais). */
static void ajouterDelta(char* dst, size_t n, const char* nom, int delta, int bonSiNegatif) {
    const char* couleur = DIM;
    if (delta != 0)
        couleur = ((delta < 0) == bonSiNegatif) ? VERT : ROUGE;
    ajouterTexte(dst, n, "%s%s %+4d" RESET "   ", couleur, nom, delta);
}

/**
 * @brief Construit les options du menu d'actions avec l'aperçu de leurs effets.
 * @return Le nombre d'options créées.
 */
static int preparerOptions(const Planete* p, OptionMenu* options,
                           char details[][TAILLE_DETAIL], Element** refs) {
    int n = 0;

    for (Element* a = p->actions; a != NULL && n < MAX_OPTIONS; a = a->suivant) {
        effet f = trouverAction(a->nom);
        int cout = energieRequise(a->nom);
        if (f == NULL || cout < 0)
            continue;

        Planete simulation = *p;
        f(&simulation);

        details[n][0] = '\0';
        ajouterTexte(details[n], TAILLE_DETAIL, JAUNE "coût %3d" RESET "   ", cout);
        ajouterDelta(details[n], TAILLE_DETAIL, "Pollution",
                     simulation.niveau_pollution - p->niveau_pollution, 1);
        ajouterDelta(details[n], TAILLE_DETAIL, "Ressources",
                     simulation.niveau_ressource - p->niveau_ressource, 0);
        ajouterDelta(details[n], TAILLE_DETAIL, "Énergie",
                     simulation.niveau_energie - p->niveau_energie, 0);

        options[n].libelle = a->nom;
        options[n].detail = details[n];
        options[n].actif = cout <= p->niveau_energie;
        options[n].raison = "Énergie insuffisante pour cette action !";
        couleurAction(a->nom, &options[n]);
        refs[n] = a;
        n++;
    }
    return n;
}

/** @brief Demande confirmation avant d'abandonner. @return 1 pour abandonner. */
static int confirmerAbandon(const EtatAffichage* etat) {
    OptionMenu oui_non[] = {
        { "Continuer la partie", NULL, 80, 200, 90, 1, NULL },
        { "Abandonner", NULL, 230, 70, 60, 1, NULL },
    };
    return menuChoix(oui_non, 2, 0, enteteTableau, etat, "Voulez-vous vraiment abandonner ?") == 1;
}

/**
 * @brief Partie interactive : le joueur choisit son action avec les flèches.
 *
 * Chaque tour : usure + événement aléatoire, puis choix de l'action dans un
 * menu coloré qui affiche le coût et l'effet prévu de chaque action.
 */
void jeuInteractif(void) {
    Planete p;
    if (!initialiserPlanete(&p))
        return;

    OptionMenu options[MAX_OPTIONS];
    char details[MAX_OPTIONS][TAILLE_DETAIL];
    Element* refs[MAX_OPTIONS];
    char msg[256];
    EtatAffichage etat = { &p, 0, NB_TOURS_VICTOIRE, NULL, NULL, NULL };
    int selection = 0, gameOver = 0, abandon = 0, tour = 0;

    while (tour < NB_TOURS_VICTOIRE && !gameOver && !abandon) {
        tour++;
        Element* ev = debutTour(&p);
        etat.tour = tour;
        etat.evenement = ev != NULL ? ev->nom : NULL;

        if (estGameOver(p)) {
            gameOver = 1;
            break;
        }

        int n = preparerOptions(&p, options, details, refs);
        int choix;
        for (;;) {
            choix = menuChoix(options, n, selection, enteteTableau, &etat,
                              "Choisissez votre action pour ce tour :");
            if (choix >= 0 || confirmerAbandon(&etat))
                break;
        }
        if (choix < 0) {
            abandon = 1;
            break;
        }

        selection = choix;
        appliquerAction(&p, refs[choix]);
        etat.action = refs[choix]->nom;
        gameOver = estGameOver(p);
    }

    messageFin(msg, sizeof(msg), &p, gameOver, abandon, tour);
    etat.message = msg;
    ecranFinal(&etat);
    libererPlanete(&p);
}
