#include"fonctions.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>


void afficher_clients(client_t * liste_clients, int taille){
    for(int i = 0; i < taille; i++){
        printf("%s %d %d\n",liste_clients[i].id, liste_clients[i].entree,liste_clients[i].nb_art);
        for(int j = 0; j < liste_clients[i].nb_art; j++){
            printf("%d%d ",liste_clients[i].list_art[j].x,liste_clients[i].list_art[j].y);
        }
        printf("\n");
    }
}

void afficher_caisses(caisse_t * liste_caisses){
    for(int i = 0; i < 7; i++){
        printf("numero : %d\ndebut : %d\nfin : %d\n",liste_caisses[i].numero,liste_caisses[i].debut,liste_caisses[i].fin);
    }
}

caisse_t* initialiser_caisses(char* caisses, caisse_t* liste_caisses){
    //caisses is the file name
    FILE* fichier = NULL;
    fichier = fopen(caisses, "r");
    char read[500] = "";
    int i = 0;
    while(fgets(read, 500, fichier) != NULL){
        caisse_t caisse;
        char* id = strtok(read, " ");
        memmove(id, id+1, strlen(id));        
        caisse.numero = atoi(id);
        char* debut = strtok(NULL, " ");
        caisse.debut = atoi(debut);
        char* fin = strtok(NULL, " ");
        caisse.fin = atoi(fin);
        liste_caisses[i] = caisse;
        liste_caisses[i].nb = 0;
        memset(read, 0, 500);
        i++;

    }
    fclose(fichier);
    return liste_caisses;
}

client_t* initialiser_clients(char* clients, client_t* liste_clients){
    FILE* fichier = NULL;
    fichier = fopen(clients, "r");
    char read[500] = "";
    int j = 0;
    while(fgets(read, 500, fichier) != NULL){
        client_t client;
        char *tmp = strtok(read, " ");
        client.id = malloc(sizeof(char)*15);
        strcpy(client.id,tmp);
        client.entree = atoi(strtok(NULL, " "));
        client.nb_art = atoi(strtok(NULL, " "));
        client.list_art = malloc(sizeof(article_t)*client.nb_art);
        for(int i = 0; i < client.nb_art; i++){
            article_t article;
            char* art;
            art = strtok(NULL, " ");
            article.x = art[1]-48;
            article.y = art[2]-48;
            client.list_art[i] = article; 
        }
        liste_clients[j] = client;
        memset(read, 0, 500);
        j++;
    }
    fclose(fichier);
    return liste_clients;
}

void liberer_clients(client_t * liste_clients, int taille){
    for(int i = 0; i < taille; i++){
        free(liste_clients[i].id);
        free(liste_clients[i].list_art);
    }
    free(liste_clients);
}
void liberer_caisses(caisse_t * liste_caisses){
    for(int i = 0; i < 7; i++){
        free(liste_caisses[i].occupation);
    }
    free(liste_caisses);
}

int temps_magasin(client_t client){
    int temps = client.nb_art*15;
    temps += temps_rayons(0,0,client.list_art[0].x,client.list_art[0].y);
    for(int i = 0; i < client.nb_art-1; i++){
        temps += temps_rayons(client.list_art[i].x,client.list_art[i].y,client.list_art[i+1].x,client.list_art[i+1].y);
    }
    return temps;
}

int temps_rayons(int i, int j, int i2, int j2){
    int haut = (5- j)*10+5+7*abs(i2-i) + 5 + (5-j2)*10;
    int bas = (j-1)*10 + 5 + 8*abs(i-i2) + 5 + (j2-1)*10;
    if((i == i2) && (j == j2))
        return 0;
    if(i == 0 && j == 0){
        return 5 + temps_rayons(0, 1, i2, j2);
    }
    if(i == i2){
        return abs(j-j2)*10;
    }
    else{
        if(i == 0){
            return haut; 
        }else if(i2 == 0){
            return haut;
        }
        return haut < bas  ? haut : bas;
    }
}

void arrivee_caisse_client(client_t client, caisse_t* caisses, int* caisse, int* temps){
    *temps = client.entree + temps_magasin(client) + (client.list_art[client.nb_art-1].y-1)*10+5;
    int res = 0;
    int min = 8;
    int index;
    for(int i = 0; i < 7; i++){
        if(*temps >= caisses[i].debut && *temps <= caisses[i].fin){
            if(abs(caisses[i].numero-client.list_art[client.nb_art-1].x) < min){
                min = abs(caisses[i].numero-client.list_art[client.nb_art-1].x);
                res = caisses[i].numero;
                index = i;
            }
            if(abs(caisses[i].numero-client.list_art[client.nb_art-1].x) == min && (i < index )){
                index = i;
                res = caisses[i].numero;
                index = i;
            }
        }
    }
    *temps += 8*min;
    *caisse = res;

}

int get_nbligne(char * filename){
    FILE * fichier = fopen(filename,"r") ;
    int c;
    int nLignes = 0;
    int c2 = '\0';
    while((c=fgetc(fichier)) != EOF){
        if(c=='\n')
            nLignes++;
        c2 = c;
    }
    if(c2 != '\n')
        nLignes++;
    fclose(fichier);
    return nLignes;
};

void afficher_evenement(evenement_t evenement){
    if(strcmp("ENTREE",evenement.evenement) == 0){
        printf("%d  ENTREE  %s\n",evenement.temps,evenement.client);
    }
    if(strcmp("FERMETURE",evenement.evenement) == 0){
        printf("%d  FERMETURE  C%d\n",evenement.temps,evenement.numero_caisse);
    }
    if(strcmp("CAISSE",evenement.evenement) == 0){
        printf("%d  CAISSE  %s  C%d\n",evenement.temps,evenement.client,evenement.numero_caisse);
    }
    if(strcmp("OUVERTURE",evenement.evenement) == 0){
        printf("%d  OUVERTURE  C%d\n",evenement.temps,evenement.numero_caisse);
    }
    if(strcmp("SORTIE",evenement.evenement) == 0){
        printf("%d  SORTIE  %s\n",evenement.temps,evenement.client);
    }
    if(strcmp("SERVICE",evenement.evenement) == 0){
        printf("%d  SERVICE  %s  C%d\n",evenement.temps,evenement.client,evenement.numero_caisse);
    }
}