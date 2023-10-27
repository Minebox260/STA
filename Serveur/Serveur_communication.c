#include "include/Serveur.h"
#include "include/Serveur_communication.h"
#include "include/Serveur_utils.h"

void * receive_data(void * arg) {

    struct sockaddr_in adr_client; //adresse du client
    socklen_t longadr;
    int nbcar;
    char ip_client[MAXOCTETS];
    char buff_recv[MAXOCTETS+1]; // Buffer de réception d'octets
    char resp[MAXOCTETS+1];

    longadr=sizeof(adr_client);

    while (1) {
        memset(&adr_client,0,sizeof(struct sockaddr_in));
        nbcar=recvfrom(sd, buff_recv,MAXOCTETS+1,0,(struct sockaddr *) &adr_client,&longadr);
        CHECK_ERROR(nbcar,0,"\nSERVEUR - Problème lors de la réception\n");
        handle_request(buff_recv, adr_client, resp);
        send_data(resp, adr_client);
    }
}

void * send_data(char data[MAXOCTETS+1], struct sockaddr_in adr_client) {
    int nbcar;
    // TODO : Renvoi si timeout sans réponse (Remplacer handle_request par thread pour gérer possibles autres requêtes en simultané)
    nbcar=sendto(sd, data,strlen(data) + 1,0,(const struct sockaddr *) &adr_client,sizeof(adr_client));
    CHECK_ERROR(nbcar,0,"\nErreur lors de l'émission des données");
}

void * handle_request(char request[MAXOCTETS + 1], struct sockaddr_in adr_client, char * resp) {
    int code;
    int resp_code = 0;
    int car_id;
    char *ptr = strtok(request, ":");
    char buffer[5];
    char ip_client[MAXOCTETS+1];
    int i;
    int id_ressource;

    strcpy(ip_client, inet_ntoa(adr_client.sin_addr));

    // Analyse du code de requête
    code = atoi(ptr);
    if (code == 101) { // Demande enregistrement
        i = 0;
        while (i < MAXVOITURES && cars_list[i] != NULL) { // On cherche le prochain emplacement de voiture libre
            if (!strcmp(ip_client, inet_ntoa(cars_list[i]->addr.sin_addr))) { // Si l'ip existe déjà
                resp_code = 401;
                break;
            }
            else i++;
        }
        if (i == MAXVOITURES) { // Si plus de place
            resp_code = 402;
        } else if (resp_code != 401) { // Si on a trouvé un emplacement libre
            cars_list[i] = (struct car *)malloc(sizeof(car));
            memcpy(&cars_list[i]->addr,(struct sockaddr*)&adr_client, sizeof(struct sockaddr_in)); // On copie l'adresse
            cars_list[i]->pos_x = 0;
            cars_list[i]->pos_y = 0;
            resp_code = 201;
        }
        itoa(resp_code, resp);
    } 
    else { // Autres demandes : la voiture doit être enregistrée
        if (car_id = get_car_id(ip_client) == -1) { // On vérifie si la voiture est enregistrée est on récupère l'ID
            resp_code = 403;
            itoa(resp_code, resp);
        } else {
            switch(code) {
                case 102: // Réception de rapport de position
                    resp_code = 202;

                    ptr = strtok(NULL, ":"); // On parse la pos X
                    if (strlen(ptr)==0) resp_code = 404;
                    else {
                        cars_list[car_id]->pos_x = atoi(ptr);
                    
                        ptr = strtok(NULL, ":"); // On parse la pos Y
                        if (strlen(ptr)==0) resp_code = 404;
                        else cars_list[car_id]->pos_y = atoi(ptr);
                    }

                    itoa(resp_code, resp);
                    break;

                case 103: // Demande d'accès à une ressource
                    ptr = strtok(NULL, ":");
                    if (strlen(ptr)==0) resp_code = 500;
                    else {
                        id_ressource = atoi(ptr);
                        if (id_ressource < 0 || id_ressource >= NBRESSOURCES) resp_code = 500;
                        else {
                            if (ressources_list[id_ressource]->reserved) {
                                if (ressources_list[id_ressource]->car_id == car_id) resp_code = 407;
                                else resp_code = 405;
                            } else {
                                resp_code = 203;
                                ressources_list[id_ressource]->reserved = 1;
                                ressources_list[id_ressource]->car_id = car_id;
                            }
                        }
                    }
                    itoa(resp_code, resp);
                    break;

                case 104: // Demande de libération  d'une ressource
                    ptr = strtok(NULL, ":");
                    if (strlen(ptr)==0) resp_code = 500;
                    else {
                        id_ressource = atoi(ptr);
                        if (id_ressource < 0 || id_ressource >= NBRESSOURCES) resp_code = 500;
                        else {
                            if (ressources_list[id_ressource]->reserved && ressources_list[id_ressource]->car_id == car_id) {
                                resp_code = 204;
                                ressources_list[id_ressource]->reserved = 0;
                                ressources_list[id_ressource]->car_id = -1;
                            } else resp_code = 406;
                        }
                    }
                    itoa(resp_code, resp);
                    break;
                default:
                    resp_code = 500;
                    itoa(resp_code, resp);
                    break;
            }
        }
    }
}

