/**
 * @file main.c
 * @brief Programme principal : menu (jeu interactif, démos, tests).
 *
 * Options : --test (tests unitaires), --auto (partie automatique en texte).
 */
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "element.h"
#include "planete.h"
#include "action.h"
#include "jeu.h"
#include "carte.h"
#include "ui.h"
#include "terminal.h"
#include <stdlib.h>
#include <time.h>

/**
 * @brief Tests unitaires de la liste chaînée d'Elements.
 */
void test_liste_chainee(void) {
    Element *liste = NULL;

    printf("=== Test liste chainee ===\n");

    ajouterElement(&liste, "Variable", "int");
    ajouterElement(&liste, "Fonction", "void");
    ajouterElementTail(&liste, "Constante", "float");

    afficherElements(liste);
    assert(tailleListe(liste) == 3);
    assert(strcmp(liste->nom, "Fonction") == 0);
    assert(strcmp(rechercherElementIndice(liste, 2)->nom, "Constante") == 0);
    assert(rechercherElementIndice(liste, 3) == NULL);
    assert(rechercherElementNom(liste, "Variable") != NULL);
    assert(tirageAleatoireElement(liste) != NULL);

    supprimerElement(&liste, "Variable");
    supprimerElement(&liste, "Inexistant");
    printf("\n--- Apres suppression ---\n");
    afficherElements(liste);
    assert(tailleListe(liste) == 2);
    assert(rechercherElementNom(liste, "Variable") == NULL);

    supprimerElement(&liste, "Fonction");
    supprimerElement(&liste, "Constante");
    assert(liste == NULL);
    assert(tirageAleatoireElement(liste) == NULL);

    libererListeElements(liste);
    printf("Test liste chainee : OK\n\n");
}

/**
 * @brief Tests de création, modification et affichage d'une planète.
 */
void test_planete(void) {
    printf("=== Test planete ===\n");

    Planete p = creerPlanete("Test", 100, 200, 50);
    ajouterAction(&p, PLANTER_ARBRE);
    ajouterEvenement(&p, "Incendie");
    afficherPlanete(p);

    assert(tailleListe(p.actions) == 1);
    assert(tailleListe(p.evenements) == 1);
    assert(verifierPlanete(p) == 0);

    planter_arbre(&p);
    assert(p.niveau_pollution == 10 && p.niveau_ressource == 220 && p.niveau_energie == 70);

    modifierPlanete(&p, -500, 0, 0);
    assert(p.niveau_pollution == 0);  /* borné à 0 */

    modifierPlanete(&p, MAX_POLLUTION, 0, 0);
    assert(verifierPlanete(p) == 1);

    libererPlanete(&p);
    printf("Test planete : OK\n\n");
}

/** @brief Lance les deux séries de tests unitaires. */
static void lancerTests(void) {
    test_liste_chainee();
    test_planete();
    printf("Tous les tests sont passés.\n");
}

/** @brief Menu principal piloté avec les flèches. */
static int menuPrincipal(void) {
    Planete apercu;
    if (!initialiserPlanete(&apercu))
        return 1;

    EtatAffichage etat = { &apercu, 0, NB_TOURS_VICTOIRE, NULL, NULL,
                           "Protégez la planète pendant 10 tours !" };
    OptionMenu menu[] = {
        { "Jouer", "Choisissez vous-même vos actions", 80, 200, 90, 1, NULL },
        { "Démo (stratégie V1)", "L'ordinateur joue : ressources - pollution", 60, 190, 225, 1, NULL },
        { "Démo (stratégie V2)", "L'ordinateur joue : stratégie prudente", 205, 120, 235, 1, NULL },
        { "Tests unitaires", "Vérifie la liste chaînée et la planète", 235, 195, 60, 1, NULL },
        { "Quitter", NULL, 230, 70, 60, 1, NULL },
    };
    int nb = (int)(sizeof(menu) / sizeof(menu[0]));
    int choix = 0;

    if (activerModeBrut() != 0) {
        fprintf(stderr, "Erreur : impossible de configurer le terminal\n");
        libererPlanete(&apercu);
        return 1;
    }

    for (;;) {
        choix = menuChoix(menu, nb, choix, enteteTableau, &etat, "Menu principal :");
        if (choix == 0)
            jeuInteractif();
        else if (choix == 1)
            jeuDemo(calculScoreV1);
        else if (choix == 2)
            jeuDemo(calculScoreV2);
        else if (choix == 3) {
            restaurerTerminal();
            lancerTests();
            printf("\nAppuyez sur Entrée pour revenir au menu...");
            fflush(stdout);
            getchar();
            activerModeBrut();
        } else
            break;
    }

    restaurerTerminal();
    libererPlanete(&apercu);
    printf("Au revoir !\n");
    return 0;
}

int main(int argc, char** argv) {
    srand((unsigned)time(NULL));

    if (argc > 1 && strcmp(argv[1], "--test") == 0) {
        lancerTests();
        return 0;
    }
    if (argc > 1 && strcmp(argv[1], "--auto") == 0) {
        jeuComplet(calculScoreV2);
        return 0;
    }

    /* Tampon large : chaque image est envoyée d'un bloc, sans scintillement */
    setvbuf(stdout, NULL, _IOFBF, 1 << 16);
    return menuPrincipal();
}
