/**
 * @file terminal.h
 * @brief Gestion du terminal : mode brut, lecture des touches (flèches).
 */
#ifndef _TERMINAL_H_
#define _TERMINAL_H_

/** @brief Touches reconnues par lireTouche(). */
typedef enum {
    TOUCHE_AUCUNE = 0,  /**< Délai écoulé sans appui */
    TOUCHE_HAUT,
    TOUCHE_BAS,
    TOUCHE_GAUCHE,
    TOUCHE_DROITE,
    TOUCHE_ENTREE,      /**< Entrée ou Espace */
    TOUCHE_ECHAP,
    TOUCHE_QUITTER,     /**< 'q' ou fin de l'entrée standard */
    TOUCHE_AUTRE,
    TOUCHE_CHIFFRE = 100 /**< '1' renvoie TOUCHE_CHIFFRE + 0, '2' TOUCHE_CHIFFRE + 1... */
} Touche;

int activerModeBrut(void);
void restaurerTerminal(void);
int lireTouche(void);
int lireToucheDelai(int delaiMs);

#endif
