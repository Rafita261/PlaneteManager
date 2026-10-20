# Rendu TP 2 - POGRAMMATION C II

```plaintext

- RATSIMBAZAFY Fitahiana Christalin

- RAKOTOSANDRATANA Iorenantsoa Yohanna

L3 INFO - Groupe 2B

```

## Description

On a créé deux listes chaînées différentes  :

- Liste générique (Liste) : stocke des valeurs de types quelconques grâce à void * et à une copie des données en mémoire.
- Liste d'éléments (Element) : stocke des éléments possédant un nom, un type et un pointeur vers l'élément suivant. (Selon le TP)


## Structure du projet

```plaintext
projet/
├── include/
│   ├── linked_list.h
│   └── element.h
├── src/
│   ├── linked_list.c
│   ├── element.c
│   └── main.c
├── obj/
├── libs/
├── Makefile
└── README.md

```

## Fonctionnalités

**Liste générique (linked_list)**

|Fonction|Description|
|:-------------|:-----------:|
|create_node()|Crée un nœud contenant une copie d'une valeur.|
|add_first()|Ajoute un élément au début.|
|add_last()|Ajoute un élément à la fin.|
|remove_first()|Supprime le premier élément.|
|remove_last()|Supprime le dernier élément.|
|affiche_list()|Affiche la liste avec une fonction de rappel.|
|free_node()|Libère la mémoire de la valeur d'un nœud.|

**Liste d'éléments (element)**

|Fonction|Description|
|:-------------|:-----------:|
|creerNoeud()|Crée un nouvel élément.|
|ajouterElement()|Ajoute un élément au début.|
|ajouterElementTail()|Ajoute un élément à la fin.|
|supprimerElement()|Supprime un élément à partir de son nom.|
|afficherElements()|Affiche les noms et les types des éléments.|
|libererListeElements()|Libère tous les nœuds de la liste.|


**Compilation et exécution**

**Makefile**

Le Makefile automatise la compilation du projet et la création d'une bibliothèque statique nommée liblinked_list.a.

Les fichiers sources sont d'abord compilés en fichiers objets (.o), puis regroupés dans la bibliothèque à l'aide de la commande ar. Cette bibliothèque contient les implémentations des listes chaînées et permet de les réutiliser dans d'autres programmes sans avoir à recompiler directement tous les fichiers sources.

Lors de la compilation de l'exécutable, le compilateur GCC utilise cette bibliothèque pour résoudre les fonctions appelées par le programme principal.

**Commandes disponibles**

make : compile le projet et génère l'exécutable program.
make clean : supprime les fichiers objets, la bibliothèque statique et l'exécutable généré.

**Compilation et linkage**

```bash
make
```

**Exécuter le programme :**

```bash
./program
```

**Nettoyer les fichiers générés :**

```bash
make clean
```