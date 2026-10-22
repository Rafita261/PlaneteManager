# Planet-Manager — INFO505 Programmation C

```plaintext
- RATSIMBAZAFY Fitahiana Christalin
- RAKOTOSANDRATANA Iorenantsoa Yohanna

L3 INFO - Groupe 2B
```

## Description

Jeu de gestion d'une planète dans le terminal : à chaque tour un événement
climatique aléatoire survient, puis le joueur choisit une action écologique.
La planète est perdue si la pollution atteint 1000 ou si les ressources
tombent à 0 ; elle est sauvée si elle tient 10 tours.

La planète est dessinée en **pixel art** (couleurs 24 bits) : océans, forêts,
savanes, déserts, glaciers, nuages, smog et incendies évoluent selon la
pollution, les ressources et l'événement du tour.

## Commandes

| Touche | Effet |
|:--|:--|
| ↑ / ↓ (ou ← / →) | Déplacer la sélection |
| Entrée / Espace | Valider |
| 1 à 9 | Choisir directement l'option |
| q / Echap | Quitter (avec confirmation en cours de partie) |

Chaque action affiche son coût en énergie et son effet prévu
(vert = bon pour la planète, rouge = mauvais). Une action trop chère
apparaît en gris et ne peut pas être choisie.

## Structure du projet

```plaintext
include/                 src/
├── element.h            ├── element.c    Liste chaînée (→ libs/libElements.a)
├── planete.h            ├── planete.c    Structure Planete
├── action.h             ├── action.c     Actions + coûts (pointeurs de fonctions)
├── evenement.h          ├── evenement.c  Evénements climatiques
├── jeu.h                ├── jeu.c        Tours de jeu, stratégies de score
├── terminal.h           ├── terminal.c   Mode brut du terminal, lecture des flèches
├── ui.h                 ├── ui.c         Menus colorés
├── carte.h              ├── carte.c      Pixel art de la planète + tableau de bord
└── color.h              └── main.c       Menu principal et tests unitaires
```

## Compilation et exécution

```bash
make                 # compile la bibliothèque libElements.a puis le programme
make MODE=debug      # version debug (-g, -DDEBUG)
make doc             # documentation Doxygen dans doc/
make clean

./program            # menu principal (jouer, démos, tests)
./program --test     # tests unitaires uniquement
./program --auto     # partie automatique en mode texte
```

Le terminal doit gérer les couleurs 24 bits (terminal GNOME, Konsole,
VS Code...) et faire au moins 110 colonnes sur 32 lignes.
