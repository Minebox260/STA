/******************************************************************************/
/* APPLICATION : Serveur Mini Projet Voiture Autonome STA 2023                */
/* Date : 19/10/2023                                                          */
/* Version : 1.0                                                              */
/* Compilation : make Client.exe                                 */
/* usage : ./Client.exe <ip serveur> <port serveur>                                          */
/******************************************************************************/

#include "include/Client.h"

int main(int argc, char *argv[]) {
    if (argc != 3) printf("CLIENT | Utilisation : ./Client.exe <ip serveur> <port serveur> \n");
    else {
        struct sockaddr_in client_adr, server_adr; //adresse du serveur
        char recv_buff[MAXOCTETS+1];
        int erreur; // Gestion des erreurs
        char * ptr;
        int code;
        char commande[MAXOCTETS+1];
	    sd=socket(AF_INET, SOCK_DGRAM, 0); //Utilisation du lecteur udp
	    CHECK_ERROR(sd, -1, "Erreur socket non cree !!! \n");
	    printf("N° de la socket : %d \n", sd);

        printf("Client - Initialisation\n");

        
        server_adr.sin_family=AF_INET;
        server_adr.sin_port=htons(atoi(argv[2]));
        server_adr.sin_addr.s_addr=inet_addr(argv[1]);
        
        client_adr.sin_family=AF_INET;
        client_adr.sin_port=htons(0);
        client_adr.sin_addr.s_addr=inet_addr("0.0.0.0");
    // Préparation de l'adresse
    
        // Affectation de l'adresse au socket
        erreur=bind(sd,(const struct sockaddr *) &client_adr,sizeof(client_adr));
        CHECK_ERROR(erreur,-1, "SERVEUR - Erreur lors du bind du socket d'écoute\n");
        
        while(1) {
            printf("Enregistrement auprès du serveur...\n");
            sendto(sd, "101", strlen("101")+1, 0, (const struct sockaddr *)&server_adr, sizeof(server_adr));
            recvfrom(sd, recv_buff, MAXOCTETS+1, 0, NULL, NULL);
            printf("Réponse reçu: %s\n", recv_buff);
            ptr = strtok(recv_buff, ":");
            code = atoi(ptr);
            if (code==201 || code==401) break;
            sleep(1);
        }
        printf("Enregistrement effectué !\n");
        while (1) {
            printf("En attente d'une commande...\n");
            recvfrom(sd, recv_buff, MAXOCTETS+1, 0, NULL, NULL);
            printf("Réponse reçue: %s\n", recv_buff);
            ptr = strtok(recv_buff, ":");
            code = atoi(ptr);
            if (code==105) break;
        }
        ptr = strtok(NULL, ":");
        strcpy(commande, ptr);
        printf("Commande : %s\n", commande);
        printf("Appuyez pour quitter > \n");
        fgets(recv_buff, sizeof(recv_buff), stdin);
                
        /*pthread_create(&tid[0], NULL, receive_data, NULL);
        pthread_create(&tid[1], NULL, user_menu, NULL);

        pthread_join(tid[0], NULL);
        pthread_join(tid[1], NULL);*/
        
        close(sd);

    }
    exit(EXIT_SUCCESS);
}
