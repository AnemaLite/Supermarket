#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct{
    int x;
    int y;
}article_t;

typedef struct{
    char* client;
    int debut;
    int fin;
}occupation_t;

typedef struct{
    char* id;
    //int sortie;
    int entree;
    int nb_art;
    article_t *list_art;
}client_t;

typedef struct{
    int numero;
    int debut;
    int fin;
    occupation_t* occupation;
    int nb;
}caisse_t; 

typedef struct{
    client_t client;
    int caisse;
    int temps;
}arrivee_client_t;

typedef struct{
    int temps;
    char* evenement;
    char* client;
    int numero_caisse;
}evenement_t;

#endif
