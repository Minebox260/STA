/******************************************************************************/
/* APPLICATION : Serveur Mini Projet Voiture Autonome STA 2023                */
/* Date : 19/10/2023                                                          */
/* Version : 1.0                                                              */
/* Compilation : gcc Serveur.c -o Serveur                                     */
/* usage : ./Serveur <port serveur>                                           */
/******************************************************************************/

#include "Serveur.h"
#include "Serveur_utils.h"
#include "Serveur_communication.h"

int main(int argc, char *argv[]) {
    if (argc != 2) printf("SERVEUR | Utilisation : ./serveur <port serveur> \n");
    else {
        struct sockaddr_in server_adr; //adresse du serveur

        int erreur; // Gestion des erreurs

        initscr();
        cbreak();
        noecho();
        scrollok(stdscr, TRUE);
        nodelay(stdscr, TRUE);

        printf("SERVEUR - Initialisation\n");

        pthread_t tid[2];
        printf("SERVEUR - N° du socket dialogue : %d \n", sd);
        
        // Préparation de l'adresse
        server_adr.sin_family=AF_INET;
        server_adr.sin_port=htons(atoi(argv[1]));
        server_adr.sin_addr.s_addr=inet_addr("0.0.0.0");

        // Affectation de l'adresse au socket
        erreur=bind(sd,(const struct sockaddr *) &server_adr,sizeof(server_adr));
        CHECK_ERROR(erreur,-1, "SERVEUR - Erreur lors du bind du socket d'écoute\n");
        
        pthread_create(&tid[0], NULL, receive_data, NULL);
        pthread_create(&tid[1], NULL, user_menu, NULL);

        pthread_join(tid[0], NULL);
        pthread_join(tid[1], NULL);
        
        close(sd);

    }
    exit(EXIT_SUCCESS);
}

void * user_menu(void * t_data) {
    int i;
    char key_input;
    char menu_input[MAXOCTETS+1];
    int menu_input_index = 0;
    
    while (1) {
        system("clear");
        printf("\033[0;37m\
                /////////////////////////////////\n\
                //                             //\n\
                //  SERVEUR VOITURES AUTONOMES //\n\
                //                             //\n\
                /////////////////////////////////\n\n\n");
        
        printf("//      VOITURES CONNECTES     //\n\n");
        for (i = 0; i < MAXVOITURES; i++) {
           if (cars[i] != NULL) printf("[\033[0;32m%d\033[0;37m] | IP : %s | X : %d | Y : %d\n", i, cars[i]->ip, cars[i]->pos_x, cars[i]->pos_y);
           else printf("[\033[0;31m-\033[0;37m] Non connectée"); 
        }
        printf("\n\n");
        printf("//             MENU            //\n\n");
        printf("SENDTO : Envoyer une commande de déplacement à une voiture\n");
        printf("QUIT : Eteindre le serveur\n\n");
        printf("SERVEUR > %s", menu_input);

        usleep(1000);
        
        key_input = getch();
        if (key_input != ERR) {
            if(key_input == 13) {
                handle_input(menu_input);
                memset(menu_input, 0, MAXOCTETS+1);
                menu_input_index = 0;
            } else {
                menu_input[menu_input_index] = key_input;
                menu_input_index++;
            }
        }
    }
}

void handle_input(char * menu_input) {
    int car_id;
    char buffer[MAXOCTETS-3];
    char send[MAXOCTETS+1];
    struct sockaddr_in send_ip;
    if (!strcmp(menu_input, "SENDTO")) {
        printf("\nQuelle voiture souhaitez vous commander ?\nSERVEUR > ");
        fgets(buffer, MAXOCTETS-3, stdin);
        car_id = atoi(buffer);
        if (car_id > 0 && car_id < MAXVOITURES && cars[car_id] != NULL) {
            printf("\nQuelle commande souhaitez vous envoyer ?\nSERVEUR > ");
            fgets(buffer, MAXOCTETS-3, stdin);
            strcat(send, "105:");
            strcat(send, buffer);
            inet_aton(cars[car_id]->ip, &send_ip.sin_addr);
            send_data(send, send_ip);
        } else printf("\nID Invalide");
    } else if (!strcmp(menu_input, "QUIT")) exit(EXIT_SUCCESS);
}

