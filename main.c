#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/stat.h>
#include "fonctions.h"

int cmp_clients(const void * ptr1, const void * ptr2){
    client_t premier = *(const client_t*) ptr1;
    client_t second = *(const client_t*) ptr2;
    return (int) (premier.entree - second.entree); 
}

int cmp_caisses(const void * ptr1, const void * ptr2){ // tri croissant sur l'heure d'ouverture 
    caisse_t premier = *(const caisse_t*) ptr1;
    caisse_t second = *(const caisse_t*) ptr2;
    return (int) (premier.debut - second.debut);
}


int cmp_caisses_2(const void * ptr1, const void * ptr2){ // tri croissant sur l'heure de fermeture
    caisse_t premier = *(const caisse_t*) ptr1;
    caisse_t second = *(const caisse_t*) ptr2;
    return (int) (premier.fin - second.fin); 
}

int cmp_arrivee_client(const void * ptr1, const void * ptr2){ // tri sur l'heure d'arrivee du client
    arrivee_client_t premier = *(const arrivee_client_t*) ptr1;
    arrivee_client_t second = *(const arrivee_client_t*) ptr2;
    return (int) (premier.temps - second.temps); 
}

int cmp_evenement(const void * ptr1, const void * ptr2){ // tri sur l'heure de l'evenement
    evenement_t premier = *(const evenement_t*) ptr1;
    evenement_t second = *(const evenement_t*) ptr2;
    return (int) (premier.temps - second.temps);
}

int main(int argc, char* argv[]){
    int l = get_nbligne(argv[1]);
    evenement_t *evenements = malloc(sizeof(evenement_t)*500);
    int c =0;

    client_t *clients = malloc(sizeof(client_t)*l);
    clients = initialiser_clients(argv[1],clients);

    caisse_t* caisses = malloc(sizeof(caisse_t)*7);
    caisses = initialiser_caisses(argv[2],caisses);
    for(int i = 0; i < 7; i++){
        caisses[i].occupation = malloc(sizeof(occupation_t)*l);
    }
    qsort(clients, l, sizeof(client_t), cmp_clients);

    for(int i = 0; i < l; i++){
        evenements[i+c].evenement = "ENTREE";
        evenements[i+c].temps = clients[i].entree;
        evenements[i+c].client = malloc(sizeof(char)*15);
        strcpy(evenements[i+c].client,clients[i].id);
    }
    c += l;
    /*  ouverture_caisses = qsort
    fermetures_caisses = qsort */

    for(int i = 0; i < 7; i++){
        evenements[i+c].evenement = "FERMETURE";
        evenements[i+c].temps = caisses[i].fin;
        evenements[i+c].client = malloc(sizeof(char)*15);
        evenements[i+c].numero_caisse = caisses[i].numero;
    }
    c +=7;
    qsort(caisses, 7, sizeof(caisse_t), cmp_caisses);
    for(int i = 0; i < 7; i++){
        evenements[i+c].evenement = "OUVERTURE";
        evenements[i+c].temps = caisses[i].debut;
        evenements[i+c].client = malloc(sizeof(char)*15);
        evenements[i+c].numero_caisse = caisses[i].numero;
    }
    c +=7;
    
    int temps = 0, res = 0;
    arrivee_client_t* arrivee_client = malloc(sizeof(arrivee_client_t)*l);
    for(int i = 0; i < l; i++){
        arrivee_caisse_client(clients[i], caisses, &res, &temps);
        arrivee_client_t tmp;
        tmp.client = clients[i];
        tmp.caisse = res;
        tmp.temps = temps;
        arrivee_client[i] = tmp;

        evenements[i+c].evenement = "CAISSE";
        evenements[i+c].temps = temps;
        evenements[i+c].numero_caisse = res;
        evenements[i+c].client = malloc(sizeof(char)*15);
        strcpy(evenements[i+c].client,clients[i].id);
    }
    c +=l;
    qsort(arrivee_client, l, sizeof(arrivee_client_t), cmp_arrivee_client);

    for(int i = 0; i < l; i++){
        for(int j = 0; j < 7; j++){
            if(caisses[j].numero == arrivee_client[i].caisse){
                occupation_t occupation;
                occupation.client = arrivee_client[i].client.id;
                if(caisses[j].nb == 0){
                    occupation.debut = arrivee_client[i].temps;
                    occupation.fin = occupation.debut + 5*arrivee_client[i].client.nb_art;
                    caisses[j].occupation[caisses[j].nb] = occupation;
                }
                else{
                    int min = (caisses[j].occupation[caisses[j].nb-1].fin > arrivee_client[i].temps) ? caisses[j].occupation[caisses[j].nb-1].fin : arrivee_client[i].temps;
                    occupation.debut = min;
                    occupation.fin = occupation.debut + 5*arrivee_client[i].client.nb_art;
                    caisses[j].occupation[caisses[j].nb] = occupation;
                }
                caisses[j].nb++;
                break;
            }
        }
    }
    //Récupération des évenements SERVICE et SORTIE
    int k = 0;
    for(int i = 0; i < 7; i++){
        for(int j = 0; j < caisses[i].nb; j++){
            //printf("Caisse %d : %d %d %s\n", i+1, caisses[i].occupation[j].debut, caisses[i].occupation[j].fin, caisses[i].occupation[j].client);
            evenements[k+c].evenement = "SERVICE";
            evenements[k+c].temps = caisses[i].occupation[j].debut;
            evenements[k+c].numero_caisse = i+1;
            evenements[k+c].client = malloc(sizeof(char)*15);
            strcpy(evenements[k+c].client,caisses[i].occupation[j].client);
            
            evenements[k+c+l].evenement = "SORTIE";
            evenements[k+c+l].temps = caisses[i].occupation[j].fin;
            evenements[k+c+l].client = malloc(sizeof(char)*15);
            strcpy(evenements[k+c+l].client,caisses[i].occupation[j].client);
            k++;
        }
    }
    c += 2*k;
    
    qsort(evenements,c,sizeof(evenement_t),cmp_evenement);
    for(int i = 0; i < c; i++){
        afficher_evenement(evenements[i]);
        free(evenements[i].client);
    }
    free(evenements);

        // Affichage infos caisses
    int marche = 0;
    for(int i = 0; i < 7; i++){
        for(int j = 0; j < caisses[i].nb; j++){
            marche += caisses[i].occupation[j].fin - caisses[i].occupation[j].debut;
        }
        printf("C%d %d %.2f%%\n",i+1,marche,(float)marche/(caisses[i].fin - caisses[i].debut)*100);
        marche = 0;
    }

    printf("\n");
    for(int i = 0; i < l; i++){
        for(int j = 0; j < 7; j++){
            for(int k = 0; k < caisses[j].nb; k++){
                if(strcmp(clients[i].id, caisses[j].occupation[k].client) == 0){
                    for(int m = 0; m < k; m++){
                        if(caisses[j].occupation[m].fin < caisses[j].occupation[k].debut){
                            for(int n = 0; n < l; n++){
                                if(strcmp(clients[i].id, arrivee_client[n].client.id) == 0){
                                    printf("%s %d %d %d\n", clients[i].id, caisses[j].occupation[k].fin - clients[i].entree, m, caisses[j].occupation[k].debut - arrivee_client[n].temps);
                                    goto s;
                                }
                            }
                        }
                    }
                }
            }
        }
        s :;
    }

    liberer_clients(clients,l);
    for(int i = 0; i < 7; i++){
        free(caisses[i].occupation);
    }
    free(caisses);
    free(arrivee_client);
    return 0;
}

