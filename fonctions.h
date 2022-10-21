#ifndef FONCTIONS_H
#define FONCTIONS_H

#include"structures.h"

void afficher_clients(client_t * liste_client, int taille);
void afficher_caisses(caisse_t * caisse);
int get_nbligne(char * filename);
caisse_t* initialiser_caisses(char* caisses, caisse_t* liste_caisses);
client_t* initialiser_clients(char* clients, client_t* liste_clients);
void liberer_clients(client_t * liste_clients, int taille);
void liberer_caisses(caisse_t * liste_caisses);
int temps_rayons(int i, int j, int i2, int j2);
int temps_magasin(client_t client);
void arrivee_caisse_client(client_t client, caisse_t* caisses, int* caisse, int* temps);
void afficher_evenement(evenement_t evenement);






#endif
