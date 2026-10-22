/**
 * @file ui.c
 * @brief Menus colorés pilotés au clavier.
 */
#include "ui.h"
#include "terminal.h"
#include "color.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define LARGEUR_LIBELLE 26

/**
 * @brief Ajoute du texte formaté à la fin de @p dst (sans débordement).
 */
void ajouterTexte(char* dst, size_t taille, const char* format, ...) {
    size_t lg = strlen(dst);
    if (lg >= taille)
        return;
    va_list args;
    va_start(args, format);
    vsnprintf(dst + lg, taille - lg, format, args);
    va_end(args);
}

/**
 * @brief Nombre de colonnes occupées par une chaîne UTF-8 sans séquence
 * de couleur (les octets de continuation 10xxxxxx ne comptent pas).
 */
int largeurAffichee(const char* s) {
    int n = 0;
    for (; *s; s++)
        if (((unsigned char)*s & 0xC0) != 0x80)
            n++;
    return n;
}

/** @brief Affiche une option, en surbrillance si elle est sélectionnée. */
static void dessinerOption(const OptionMenu* o, int selectionnee, int indice) {
    int r = o->actif ? o->r : 110;
    int g = o->actif ? o->g : 110;
    int b = o->actif ? o->b : 110;
    int pad = LARGEUR_LIBELLE - largeurAffichee(o->libelle);

    if (selectionnee)
        printf("  \033[48;2;%d;%d;%dm\033[38;2;10;10;10m" GRAS " ▶ %d. %s%*s" RESET,
               r, g, b, indice + 1, o->libelle, pad > 0 ? pad : 0, "");
    else
        printf("  \033[38;2;%d;%d;%dm   %d. %s%*s" RESET,
               r, g, b, indice + 1, o->libelle, pad > 0 ? pad : 0, "");

    if (o->detail != NULL)
        printf("  %s%s" RESET, o->actif ? "" : DIM, o->detail);
    printf("\033[K\n");
}

/**
 * @brief Affiche un menu et laisse le joueur choisir avec les flèches.
 *
 * Haut/Bas (ou Gauche/Droite) déplacent la sélection, Entrée valide,
 * un chiffre valide directement l'option correspondante, q/Echap quitte.
 *
 * @param options Tableau des options.
 * @param n Nombre d'options.
 * @param initial Option sélectionnée au départ.
 * @param entete Fonction dessinant le haut de l'écran (peut être NULL).
 * @param contexte Paramètre transmis à @p entete.
 * @param titre Titre affiché au-dessus des options.
 * @return L'indice de l'option choisie, ou -1 si le joueur quitte.
 */
int menuChoix(const OptionMenu* options, int n, int initial,
              DessinEntete entete, const void* contexte, const char* titre) {
    if (n <= 0)
        return -1;

    int sel = (initial >= 0 && initial < n) ? initial : 0;
    const char* message = NULL;

    for (;;) {
        printf("\033[H");
        if (entete != NULL)
            entete(contexte);
        printf("\033[K\n  " GRAS "%s" RESET "\033[K\n", titre);
        for (int i = 0; i < n; i++)
            dessinerOption(&options[i], i == sel, i);
        printf("\033[K\n  " DIM "↑/↓ choisir   Entrée valider   1-%d raccourci   q quitter" RESET "\033[K\n", n);
        if (message != NULL)
            printf("  " ROUGE GRAS "%s" RESET "\033[K\n", message);
        printf("\033[J");
        fflush(stdout);

        int t = lireTouche();
        int choix = -1;
        message = NULL;

        if (t == TOUCHE_HAUT || t == TOUCHE_GAUCHE)
            sel = (sel - 1 + n) % n;
        else if (t == TOUCHE_BAS || t == TOUCHE_DROITE)
            sel = (sel + 1) % n;
        else if (t == TOUCHE_ENTREE)
            choix = sel;
        else if (t == TOUCHE_QUITTER || t == TOUCHE_ECHAP)
            return -1;
        else if (t >= TOUCHE_CHIFFRE && t - TOUCHE_CHIFFRE < n)
            choix = sel = t - TOUCHE_CHIFFRE;

        if (choix >= 0) {
            if (options[choix].actif)
                return choix;
            message = options[choix].raison != NULL ? options[choix].raison
                                                    : "Option indisponible.";
        }
    }
}
