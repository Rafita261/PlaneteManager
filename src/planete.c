#include "../include/planete.h"

Planete creerPlanete(char *nom, int energie, int ressources, int pollution){
    Planete p ;
    p.nom = nom ;
    p.niveau_energie = energie;
    p.niveau_ressource = ressources ;
    p.niveau_pollution = pollution ;
    p.actions = NULL ;
    p.evenements = NULL ;

    return p;
}


void ajouterAction(Planete *p, const char *nomAction){
    Element *e = malloc(sizeof(Element)) ;
    *e = actionToElement(nomAction) ;
    p->actions = e ;
}

void ajouterEvenement(Planete *p, const char *nomEvenement){
    Element *e = malloc(sizeof(Element)) ;
    *e = evenementToElement(nomEvenement) ;
    p->evenements = e ;
}

void afficherPlanete(const Planete p){
    for(int i = 0 ; i < strlen(p.nom) ; i++) printf("#") ;
    printf("############################################\n");
    printf("######### VOTRE PLANETE : %s\t#############\n", p.nom);
    printf("######### Pollutions : %d / %d\t#############\n", p.niveau_pollution, MAX_POLLUTION);
    printf("######### Enerigie : %d   \t#############\n", p.niveau_energie) ;
    printf("######### Ressources : %d   \t#############\n", p.niveau_ressource);
    printf("############################################\n");
}

int verifierPlanete(const Planete p){
    if (p.niveau_pollution > MAX_POLLUTION || p.niveau_energie <= 0){
        printf(ROUGE GRAS "GAME OVER !!!!!!" RESET ) ;
        afficherPlanete(p);
        printf(ROUGE GRAS "GAME OVER !!!!!!" RESET ) ;
        return 1 ; 
    }
    else {
        return 0;
    }
}