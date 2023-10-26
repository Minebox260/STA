#include "include/Serveur.h"
#include "include/Serveur_communication.h"
#include "include/Serveur_utils.h"

void itoa(int val, char * dest) {
    dest[0] = '\0';
    sprintf(dest, "%d", val);
}

int get_car_id(char * ip_client) {
    int i;

    while (i < MAXVOITURES && cars_list[i] != NULL) {
        if (!strcmp(cars_list[i]->ip, ip_client)) break;
        else i++;
    }
    
    if (i == MAXVOITURES) return -1;
    else return i;
    }