#ifndef _SERVEUR_H
    #define _SERVEUR_H
    #define _GNU_SOURCE
    

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <sys/types.h>          
    #include <sys/socket.h>
    #include <sys/wait.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <pthread.h>
    
    #define MAXOCTETS 150
    #define MAXVOITURES 2

    #define CHECK_ERROR(val1,val2,msg)   if (val1==val2) \
                                        { perror(msg); \
                                            exit(EXIT_FAILURE); }

    void * user_menu(void * t_data);
    void handle_input(char * menu_input);

    extern int sd; //socket de dialogue

    typedef struct car {
        struct sockaddr_in addr;
        int pos_x;
        int pos_y;
    } car;

    extern struct car * cars_list[MAXVOITURES];

#endif