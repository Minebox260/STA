#ifndef _SERVEUR_COMM_H
    #define _SERVEUR_COMM_H

    //////////////////////////////////////////////////
    //  TABLE DES REQUETES                          //
    //  101 : Demande d'enregistrement              //
    //  102 : Rapport de position                   //
    //  103 : Demander autorisation ressource       //
    //  104 : Demander libération ressource         //
    //  105 : Envoi de consigne                     //
    //                                              //
    //  REPONSES ACK                                //
    //  201 : Enregistrement effectué               //
    //  202 : Position acquittée                    //
    //  203 : Autorisation d'accès accordée         //
    //  204 : Ressource libérée                     //
    //  205 : Consigne acquittée                    //
    //                                              //
    // REPONSES NACK                                //
    // 401 : Déjà enregistré                        //
    // 402 : Nombre maximal de voitures sur le      //
    //       serveur atteint                        //
    // 403 : Pas enregistré sur le serveur          //
    // 404 : Impossible d'acquitter la position     //
    // 405 : La ressource est occupée               //
    // 406 : La ressource n'est pas occupée /       //
    //       Impossible de la libérer               //
    // 407 : Cette voiture a déjà réservé           //
    //       la ressource                           //
    // 408 : Impossible d'acquitter la consigne     //
    //                                              //
    // AUTRES                                       //
    // 500 : Requête invalide                       //
    //////////////////////////////////////////////////

    void * receive_data(void * arg);
    void * send_data(char * data, struct sockaddr_in adr_client);
    void * handle_request(char * request, struct sockaddr_in adr_client, char * resp);

#endif