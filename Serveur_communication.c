#include "Serveur.h"
#include "Serveur_communication.h"
#include "Serveur_utils.h"

void * receive_data(void * arg) {

    struct sockaddr_in adr_client; //adresse du client
    socklen_t longadr;
    int nbcar;
    char ip_client[MAXOCTETS];
    char buff_recv[MAXOCTETS+1]; // Buffer de réception d'octets
    char resp[MAXOCTETS+1];

    longadr=sizeof(adr_client);

    while (1) {
        nbcar=recvfrom(sd, buff_recv,MAXOCTETS+1,0,(struct sockaddr *) &adr_client,&longadr);
        CHECK_ERROR(nbcar,0,"\nSERVEUR - Problème lors de la réception\n");
        printf("Received request : %s", buff_recv);
        //handle_request(buff_recv, adr_client, resp);

        //send_data(resp, adr_client);
    }
}

void * send_data(char data[MAXOCTETS+1], struct sockaddr_in adr_client) {
    int nbcar;
    nbcar=sendto(sd, data,strlen(data) + 1,0,(const struct sockaddr *) &adr_client,sizeof(adr_client));
    CHECK_ERROR(nbcar,0,"\nErreur lors de l'émission des données");
}

void * handle_request(char request[MAXOCTETS + 1], struct sockaddr_in adr_client, char * resp) {
    int code;
    int resp_code = 000;
    int car_id;
    char *ptr = strtok(request, ":");
    char buffer[5];
    char ip_client[MAXOCTETS+1];

    strcpy(ip_client, inet_ntoa(adr_client.sin_addr));

    // Analyse du code de requête
    code = atoi(ptr);
    if (code == 101) { // Demande enregistrement
        int i = 0;
        while (i < MAXVOITURES && cars_list[i] != NULL) { //On cherche le prochain emplacement de voiture libre
            if (!strcmp(ip_client, cars_list[i]->ip)) {
                resp_code = 401;
                break;
            }
            else i++;
        }
        if (i = MAXVOITURES) {
            resp_code = 402;
        } else if (resp_code != 401) {
            cars_list[i] = (struct car *)malloc(sizeof(car));
            
            strcpy(cars_list[i]->ip, ip_client);
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
                    itoa(resp_code, resp);
                    break;

                case 103: // Demande d'accès à une ressource
                    itoa(resp_code, resp);
                    break;

                case 104: // Demande de libération  d'une ressource
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

