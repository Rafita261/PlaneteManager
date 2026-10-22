/**
 * @file ui.h
 * @brief Menus colorés pilotés au clavier (flèches + Entrée).
 */
#ifndef _UI_H_
#define _UI_H_

#include <stddef.h>

/** @brief Une ligne d'un menu. */
typedef struct {
    const char* libelle;  /**< Texte principal */
    const char* detail;   /**< Texte secondaire (peut contenir des couleurs) */
    int r, g, b;          /**< Couleur de l'option */
    int actif;            /**< 0 si l'option ne peut pas être choisie */
    const char* raison;   /**< Message affiché si on valide une option inactive */
} OptionMenu;

/** @brief Fonction dessinant le haut de l'écran au-dessus du menu. */
typedef void (*DessinEntete)(const void* contexte);

int menuChoix(const OptionMenu* options, int n, int initial,
              DessinEntete entete, const void* contexte, const char* titre);
void ajouterTexte(char* dst, size_t taille, const char* format, ...);
int largeurAffichee(const char* s);

#endif
