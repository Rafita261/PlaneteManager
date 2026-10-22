/**
 * @file carte.c
 * @brief Rendu "pixel art" de la planète dans le terminal.
 *
 * Chaque caractère '▀' affiche deux pixels carrés : celui du haut avec la
 * couleur du texte, celui du bas avec la couleur de fond (couleurs 24 bits).
 * Le relief est généré par un bruit procédural projeté sur une sphère ;
 * les couleurs dépendent de la pollution, des ressources et de l'événement.
 */
#include "carte.h"
#include "evenement.h"
#include "ui.h"
#include "color.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define CARTE_L 40              /**< Largeur en pixels */
#define CARTE_H 40              /**< Hauteur en pixels (paire) */
#define NB_LIGNES (CARTE_H / 2) /**< Lignes de terminal occupées */
#define LARGEUR_BARRE 20
#define TAILLE_LIGNE 640

/** @brief Couleur RVB. */
typedef struct { int r, g, b; } Couleur;

static const Couleur ESPACE        = {   6,   6,  16 };
static const Couleur ETOILE        = { 230, 230, 255 };
static const Couleur OCEAN_PROFOND = {  15,  45, 125 };
static const Couleur OCEAN_COTIER  = {  40, 125, 210 };
static const Couleur OCEAN_POLLUE  = {  60,  62,  45 };
static const Couleur PLAGE         = { 225, 210, 150 };
static const Couleur FORET         = {  40, 135,  50 };
static const Couleur FORET_DENSE   = {  18,  85,  32 };
static const Couleur SAVANE        = { 150, 170,  70 };
static const Couleur DESERT        = { 215, 178, 100 };
static const Couleur MONTAGNE      = { 115, 100,  90 };
static const Couleur NEIGE         = { 245, 248, 252 };
static const Couleur GLACE         = { 215, 235, 255 };
static const Couleur SMOG          = { 125, 108,  78 };
static const Couleur NUAGE         = { 245, 245, 245 };
static const Couleur FEU           = { 255,  80,  20 };
static const Couleur FEU_CLAIR     = { 255, 205,  40 };
static const Couleur ATMO_PURE     = {  90, 150, 255 };
static const Couleur ATMO_POLLUEE  = { 170, 130,  70 };

/** @brief Paramètres du rendu calculés une fois par image. */
typedef struct {
    double pollution;   /**< 0..1 */
    double ressources;  /**< 0..1 */
    double rotation;    /**< Angle de rotation de la planète */
    int tour;
    int incendie, tempete, secheresse, pluie;
} ContexteRendu;

static int borne255(int v) { return v < 0 ? 0 : (v > 255 ? 255 : v); }

static Couleur rgb(int r, int g, int b) {
    Couleur c = { borne255(r), borne255(g), borne255(b) };
    return c;
}

/** @brief Mélange linéaire de @p a vers @p b (t entre 0 et 1). */
static Couleur melanger(Couleur a, Couleur b, double t) {
    if (t < 0) t = 0;
    if (t > 1) t = 1;
    return rgb((int)(a.r + (b.r - a.r) * t),
               (int)(a.g + (b.g - a.g) * t),
               (int)(a.b + (b.b - a.b) * t));
}

/** @brief Multiplie la luminosité d'une couleur. */
static Couleur eclairer(Couleur c, double f) {
    return rgb((int)(c.r * f), (int)(c.g * f), (int)(c.b * f));
}

/** @brief Valeur pseudo-aléatoire reproductible dans [0, 1]. */
static double hash3(int x, int y, int z, int graine) {
    unsigned int h = (unsigned)x * 374761393u + (unsigned)y * 668265263u
                   + (unsigned)z * 2147483647u + (unsigned)graine * 144665u;
    h = (h ^ (h >> 13)) * 1274126177u;
    h ^= h >> 16;
    return (h & 0xFFFFu) / 65535.0;
}

static double lisser(double t) { return t * t * (3 - 2 * t); }
static double lerp(double a, double b, double t) { return a + (b - a) * t; }

/** @brief Bruit de valeur 3D interpolé (continu). */
static double bruit3(double x, double y, double z, int graine) {
    int xi = (int)floor(x), yi = (int)floor(y), zi = (int)floor(z);
    double tx = lisser(x - xi), ty = lisser(y - yi), tz = lisser(z - zi);
    double c[2][2][2];

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
                c[i][j][k] = hash3(xi + i, yi + j, zi + k, graine);

    return lerp(lerp(lerp(c[0][0][0], c[1][0][0], tx), lerp(c[0][1][0], c[1][1][0], tx), ty),
                lerp(lerp(c[0][0][1], c[1][0][1], tx), lerp(c[0][1][1], c[1][1][1], tx), ty),
                tz);
}

/** @brief Somme de plusieurs octaves de bruit (relief plus naturel). */
static double fbm3(double x, double y, double z, int graine) {
    double somme = 0, amplitude = 0.5, total = 0;
    for (int o = 0; o < 4; o++) {
        somme += amplitude * bruit3(x, y, z, graine + o * 31);
        total += amplitude;
        x *= 2; y *= 2; z *= 2;
        amplitude *= 0.5;
    }
    return somme / total;
}

/** @brief Etire une valeur autour de 0.5 pour augmenter le contraste. */
static double contraste(double v) {
    v = (v - 0.5) * 2.2 + 0.5;
    return v < 0 ? 0 : (v > 1 ? 1 : v);
}

/** @brief Calcule la couleur d'un pixel de la carte. */
static Couleur pixel(const ContexteRendu* k, int px, int py) {
    double cx = (CARTE_L - 1) / 2.0, cy = (CARTE_H - 1) / 2.0;
    double rayon = CARTE_L / 2.0 - 2.0;
    double dx = (px - cx) / rayon, dy = (py - cy) / rayon;
    double d2 = dx * dx + dy * dy;
    Couleur atmo = melanger(ATMO_PURE, ATMO_POLLUEE, k->pollution);

    /* Espace, étoiles et halo de l'atmosphère */
    if (d2 > 1.0) {
        double d = sqrt(d2);
        if (d < 1.15)
            return melanger(ESPACE, atmo, (1.15 - d) / 0.15 * 0.55);
        if (hash3(px, py, 0, 77) > 0.975)
            return eclairer(ETOILE, 0.35 + 0.65 * hash3(px, py, k->tour, 78));
        return ESPACE;
    }

    /* Point de la sphère, tourné selon le tour de jeu */
    double dz = sqrt(1.0 - d2);
    double sx = dx * cos(k->rotation) + dz * sin(k->rotation);
    double sz = -dx * sin(k->rotation) + dz * cos(k->rotation);
    double sy = dy;

    double altitude = contraste(fbm3(sx * 1.5 + 11, sy * 1.5 + 11, sz * 1.5 + 11, 1));
    double humidite = contraste(fbm3(sx * 2.3 + 23, sy * 2.3 + 23, sz * 2.3 + 23, 2));
    double latitude = fabs(sy);
    double mer = 0.48 - (k->secheresse ? 0.07 : 0.0) + (k->pluie ? 0.03 : 0.0);
    double res = k->ressources - (k->secheresse ? 0.25 : 0.0) + (k->pluie ? 0.1 : 0.0);
    double glace = 0.70 + 0.28 * k->pollution + (bruit3(sx * 5, sy * 5, sz * 5, 3) - 0.5) * 0.2;
    int emissif = 0;
    Couleur c;

    if (latitude > glace) {
        c = GLACE;  /* les calottes fondent quand la pollution monte */
    } else if (altitude < mer) {
        Couleur profond = melanger(OCEAN_PROFOND, OCEAN_POLLUE, k->pollution * 0.7);
        Couleur cotier = melanger(OCEAN_COTIER, OCEAN_POLLUE, k->pollution * 0.7);
        c = melanger(profond, cotier, altitude / mer);
    } else {
        double a = (altitude - mer) / (1.0 - mer);
        double seuil = 0.85 - res;  /* moins de ressources => plus de désert */

        if (a < 0.05)                     c = PLAGE;
        else if (a > 0.9)                 c = NEIGE;
        else if (a > 0.6)                 c = MONTAGNE;
        else if (humidite < seuil - 0.1)  c = DESERT;
        else if (humidite < seuil + 0.05) c = SAVANE;
        else c = melanger(FORET, FORET_DENSE, (humidite - seuil) * 2);

        c = melanger(c, SMOG, k->pollution * 0.3);

        if (k->incendie && a < 0.6 && bruit3(sx * 6 + 5, sy * 6, sz * 6, 9) > 0.6) {
            c = melanger(FEU, FEU_CLAIR, hash3(px, py, k->tour, 10));
            emissif = 1;
        }
    }

    /* Nuages : plus nombreux et plus sombres en cas de tempête ou de pluie */
    double n = fbm3(sx * 3.0 + 40, sy * 4.0 + 40, sz * 3.0 + 40, 4);
    double seuilNuage = k->tempete ? 0.47 : (k->pluie ? 0.5 : 0.58);
    if (n > seuilNuage) {
        Couleur nuage = k->tempete ? rgb(175, 175, 195) : (k->pluie ? rgb(195, 210, 235) : NUAGE);
        double opacite = 0.25 + (n - seuilNuage) * 5;
        c = melanger(c, nuage, opacite > 0.85 ? 0.85 : opacite);
        if (opacite > 0.5)
            emissif = 0;
    }

    /* Smog : couvre la planète à mesure que la pollution monte */
    if (contraste(fbm3(sx * 3 + 60, sy * 3 + 60, sz * 3 + 60, 5)) > 1.0 - k->pollution * 0.8)
        c = melanger(c, SMOG, 0.6);

    /* Eclairage venant du haut à gauche */
    double lumiere = -0.55 * dx - 0.45 * dy + 0.70 * dz;
    lumiere = 0.2 + 0.95 * (lumiere > 0 ? lumiere : 0);
    if (emissif && lumiere < 0.95)
        lumiere = 0.95;
    c = eclairer(c, lumiere);

    if (d2 > 0.8)
        c = melanger(c, atmo, (d2 - 0.8) / 0.2 * 0.35);
    return c;
}

/** @brief Petit carré de couleur suivi d'un nom, pour la légende. */
static void pastille(char* dst, size_t n, Couleur c, const char* nom) {
    int pad = 10 - largeurAffichee(nom);
    ajouterTexte(dst, n, "\033[38;2;%d;%d;%dm██" RESET " %s%*s",
                 c.r, c.g, c.b, nom, pad > 0 ? pad : 0, "");
}

/** @brief Jauge colorée : "Libellé ██████░░░░ 320/1000". */
static void barre(char* dst, size_t n, const char* libelle, int val, int max, Couleur c) {
    int pleins = max > 0 ? val * LARGEUR_BARRE / max : 0;
    if (val > 0 && pleins == 0)
        pleins = 1;

    ajouterTexte(dst, n, "%s \033[38;2;%d;%d;%dm", libelle, c.r, c.g, c.b);
    for (int i = 0; i < LARGEUR_BARRE; i++) {
        if (i == pleins)
            ajouterTexte(dst, n, "\033[38;2;55;55;65m");
        ajouterTexte(dst, n, i < pleins ? "█" : "░");
    }
    ajouterTexte(dst, n, RESET " %4d/%d", val, max);
}

/** @brief Couleur et description d'un événement climatique. */
static Couleur infoEvenement(const char* nom, const char** description) {
    if (strcmp(nom, INCENDIE) == 0) {
        *description = "Les forêts brûlent : pollution ↑ ressources ↓";
        return rgb(235, 75, 30);
    }
    if (strcmp(nom, TEMPETE) == 0) {
        *description = "Vents violents : pollution ↑ ressources ↓";
        return rgb(170, 170, 215);
    }
    if (strcmp(nom, SECHERESSE) == 0) {
        *description = "Les sols s'assèchent : ressources ↓↓";
        return rgb(225, 175, 60);
    }
    if (strcmp(nom, PLUIE) == 0) {
        *description = "La nature se régénère : pollution ↓ ressources ↑";
        return rgb(80, 160, 255);
    }
    *description = "Rien à signaler";
    return rgb(150, 150, 150);
}

/** @brief Remplit le panneau d'informations affiché à droite de la carte. */
static void remplirPanneau(const EtatAffichage* e, char panneau[NB_LIGNES][TAILLE_LIGNE]) {
    const Planete* p = e->planete;
    double pol = (double)p->niveau_pollution / MAX_POLLUTION;
    double res = (double)p->niveau_ressource / MAX_RESSOURCES;
    Couleur vert = rgb(80, 200, 90), rouge = rgb(230, 60, 50);

    ajouterTexte(panneau[0], TAILLE_LIGNE, GRAS CYAN "P L A N E T  -  M A N A G E R" RESET);
    ajouterTexte(panneau[1], TAILLE_LIGNE, DIM "──────────────────────────────────────────" RESET);
    ajouterTexte(panneau[2], TAILLE_LIGNE, "Planète " GRAS "%s" RESET, p->nom != NULL ? p->nom : "?");
    if (e->tour > 0)
        ajouterTexte(panneau[2], TAILLE_LIGNE, "      Tour " GRAS "%d" RESET " / %d", e->tour, e->tourMax);

    barre(panneau[4], TAILLE_LIGNE, "Pollution ", p->niveau_pollution, MAX_POLLUTION, melanger(vert, rouge, pol));
    barre(panneau[5], TAILLE_LIGNE, "Ressources", p->niveau_ressource, MAX_RESSOURCES, melanger(rouge, vert, res));
    barre(panneau[6], TAILLE_LIGNE, "Énergie   ", p->niveau_energie, MAX_ENERGIE, rgb(240, 200, 60));

    if (e->evenement != NULL) {
        const char* description;
        Couleur c = infoEvenement(e->evenement, &description);
        ajouterTexte(panneau[8], TAILLE_LIGNE,
                     "Événement  \033[48;2;%d;%d;%dm\033[38;2;10;10;10m" GRAS " %s " RESET,
                     c.r, c.g, c.b, e->evenement);
        ajouterTexte(panneau[9], TAILLE_LIGNE, DIM "           %s" RESET, description);
    }
    if (e->action != NULL)
        ajouterTexte(panneau[10], TAILLE_LIGNE, "Dernière action  " GRAS "%s" RESET, e->action);

    if (pol >= 0.75)
        ajouterTexte(panneau[12], TAILLE_LIGNE, ROUGE GRAS "/!\\ Pollution critique  " RESET);
    if (res <= 0.2)
        ajouterTexte(panneau[12], TAILLE_LIGNE, ROUGE GRAS "/!\\ Ressources faibles  " RESET);
    if (p->niveau_energie < 30)
        ajouterTexte(panneau[12], TAILLE_LIGNE, JAUNE GRAS "/!\\ Énergie faible" RESET);

    if (e->message != NULL)
        ajouterTexte(panneau[13], TAILLE_LIGNE, "%s", e->message);

    ajouterTexte(panneau[15], TAILLE_LIGNE, DIM "Légende" RESET);
    pastille(panneau[16], TAILLE_LIGNE, OCEAN_COTIER, "Océan");
    pastille(panneau[16], TAILLE_LIGNE, FORET, "Forêt");
    pastille(panneau[16], TAILLE_LIGNE, SAVANE, "Savane");
    pastille(panneau[17], TAILLE_LIGNE, DESERT, "Désert");
    pastille(panneau[17], TAILLE_LIGNE, GLACE, "Glace");
    pastille(panneau[17], TAILLE_LIGNE, MONTAGNE, "Montagne");
    pastille(panneau[18], TAILLE_LIGNE, SMOG, "Smog");
    pastille(panneau[18], TAILLE_LIGNE, FEU, "Feu");
    pastille(panneau[18], TAILLE_LIGNE, NUAGE, "Nuages");
}

/**
 * @brief Dessine la carte en pixel art et le tableau de bord.
 *
 * L'appelant place le curseur (par exemple en haut de l'écran) avant l'appel.
 *
 * @param e Etat à afficher.
 */
void afficherTableau(const EtatAffichage* e) {
    static Couleur pixels[CARTE_H][CARTE_L];
    static char panneau[NB_LIGNES][TAILLE_LIGNE];
    const Planete* p = e->planete;
    ContexteRendu k;

    k.pollution = (double)p->niveau_pollution / MAX_POLLUTION;
    k.ressources = (double)p->niveau_ressource / MAX_RESSOURCES;
    k.rotation = e->tour * 0.45;
    k.tour = e->tour;
    k.incendie = e->evenement != NULL && strcmp(e->evenement, INCENDIE) == 0;
    k.tempete = e->evenement != NULL && strcmp(e->evenement, TEMPETE) == 0;
    k.secheresse = e->evenement != NULL && strcmp(e->evenement, SECHERESSE) == 0;
    k.pluie = e->evenement != NULL && strcmp(e->evenement, PLUIE) == 0;

    for (int y = 0; y < CARTE_H; y++)
        for (int x = 0; x < CARTE_L; x++)
            pixels[y][x] = pixel(&k, x, y);

    memset(panneau, 0, sizeof(panneau));
    remplirPanneau(e, panneau);

    for (int l = 0; l < NB_LIGNES; l++) {
        printf(" ");
        for (int x = 0; x < CARTE_L; x++) {
            Couleur h = pixels[2 * l][x], b = pixels[2 * l + 1][x];
            printf("\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm▀",
                   h.r, h.g, h.b, b.r, b.g, b.b);
        }
        printf(RESET "   %s" RESET "\033[K\n", panneau[l]);
    }
}

/**
 * @brief Adaptateur pour menuChoix() : dessine un EtatAffichage.
 * @param etat Pointeur vers un EtatAffichage.
 */
void enteteTableau(const void* etat) {
    afficherTableau((const EtatAffichage*)etat);
}
