/**
 * @file terminal.c
 * @brief Mode brut du terminal (termios) et décodage des touches.
 *
 * Les flèches envoient des séquences d'échappement : ESC [ A (haut),
 * ESC [ B (bas), ESC [ C (droite), ESC [ D (gauche).
 */
#define _POSIX_C_SOURCE 200809L
#include "terminal.h"
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>

static struct termios ancienMode;
static int modeActif = 0;
static int finEntree = 0;

/** @brief Ecrit une séquence (utilisable dans un gestionnaire de signal). */
static void ecrire(const char* s) {
    ssize_t r = write(STDOUT_FILENO, s, strlen(s));
    (void)r;
}

/**
 * @brief Remet le terminal dans son état d'origine.
 *
 * Réaffiche le curseur et quitte l'écran alternatif. Sans effet si le
 * mode brut n'est pas actif.
 */
void restaurerTerminal(void) {
    if (!modeActif)
        return;
    ecrire("\033[0m\033[?25h\033[?1049l");
    if (isatty(STDIN_FILENO))
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &ancienMode);
    modeActif = 0;
}

/** @brief Restaure le terminal avant de laisser le signal terminer le programme. */
static void surSignal(int sig) {
    restaurerTerminal();
    signal(sig, SIG_DFL);
    raise(sig);
}

/**
 * @brief Passe le terminal en mode brut (touches lues sans attendre Entrée,
 * sans écho) et bascule sur l'écran alternatif.
 *
 * @return 0 en cas de succès, -1 en cas d'erreur.
 */
int activerModeBrut(void) {
    static int nettoyageEnregistre = 0;

    if (modeActif)
        return 0;

    if (isatty(STDIN_FILENO)) {
        if (tcgetattr(STDIN_FILENO, &ancienMode) == -1)
            return -1;
        struct termios brut = ancienMode;
        brut.c_lflag &= ~(ICANON | ECHO);
        brut.c_cc[VMIN] = 1;
        brut.c_cc[VTIME] = 0;
        if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &brut) == -1)
            return -1;
    }
    modeActif = 1;

    if (!nettoyageEnregistre) {
        atexit(restaurerTerminal);
        signal(SIGINT, surSignal);
        signal(SIGTERM, surSignal);
        nettoyageEnregistre = 1;
    }

    ecrire("\033[?1049h\033[?25l\033[2J\033[H");
    return 0;
}

/**
 * @brief Lit un octet sur l'entrée standard.
 * @param delaiMs Délai maximal en ms (-1 : attente infinie).
 * @return L'octet lu, -1 si le délai est écoulé, -2 en fin d'entrée.
 */
static int lireOctet(int delaiMs) {
    if (finEntree) {
        if (delaiMs > 0) {
            struct timeval tv = { delaiMs / 1000, (delaiMs % 1000) * 1000 };
            select(0, NULL, NULL, NULL, &tv);
        }
        return -2;
    }
    if (delaiMs >= 0) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        struct timeval tv = { delaiMs / 1000, (delaiMs % 1000) * 1000 };
        if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) <= 0)
            return -1;
    }
    unsigned char c;
    if (read(STDIN_FILENO, &c, 1) != 1) {
        finEntree = 1;
        return -2;
    }
    return c;
}

/**
 * @brief Attend une touche pendant au plus @p delaiMs millisecondes.
 * @param delaiMs Délai maximal (-1 : attente infinie).
 * @return Une valeur de Touche (TOUCHE_AUCUNE si le délai est écoulé).
 */
int lireToucheDelai(int delaiMs) {
    int c = lireOctet(delaiMs);

    if (c == -1)
        return TOUCHE_AUCUNE;
    if (c == -2)
        return delaiMs >= 0 ? TOUCHE_AUCUNE : TOUCHE_QUITTER;

    if (c == 27) {
        int c2 = lireOctet(50);
        if (c2 != '[' && c2 != 'O')
            return TOUCHE_ECHAP;
        switch (lireOctet(50)) {
            case 'A': return TOUCHE_HAUT;
            case 'B': return TOUCHE_BAS;
            case 'C': return TOUCHE_DROITE;
            case 'D': return TOUCHE_GAUCHE;
            default:  return TOUCHE_AUTRE;
        }
    }
    if (c == '\n' || c == '\r' || c == ' ')
        return TOUCHE_ENTREE;
    if (c == 'q' || c == 'Q')
        return TOUCHE_QUITTER;
    if (c == 'k')
        return TOUCHE_HAUT;
    if (c == 'j')
        return TOUCHE_BAS;
    if (c >= '1' && c <= '9')
        return TOUCHE_CHIFFRE + (c - '1');
    return TOUCHE_AUTRE;
}

/**
 * @brief Attend qu'une touche significative soit pressée.
 * @return Une valeur de Touche (jamais TOUCHE_AUCUNE).
 */
int lireTouche(void) {
    int t;
    do {
        t = lireToucheDelai(-1);
    } while (t == TOUCHE_AUCUNE);
    return t;
}
