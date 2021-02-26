//
//  server.c
//  chat
//
//  Created by Oana Vasilescu on 26/02/2021.
//

#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>

void server(int adresa_ip, int port) // adresa ip nu stiu daca e int... cred ca char mai degraba
{
    int descriptor_socket;
    struct sockaddr_in adresa;
    int adress_len = sizeof(adresa);
    if((descriptor_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))<0){
        // s-ar putea ca argumentele sa nu fie ok asa; la ultima chestie unii puneau 0
        perror("la server nu s-a putut face socket");
        exit(0);
        
    }
    // inainte sa trecem mai departe, cred ca aici trb sa bagam in adresa ce primim ca param la f server (adresa ip si nr port);
    if(bind(descriptor_socket,(struct sockaddr *)& adresa, sizeof(adresa)) < 0){
        perror("server: pb cu bind");
        exit(1);
    }
    if(listen(descriptor_socket, 32)<0){// 32 e backlog
        perror("server: pb la listen");
        exit(2);
    }
    if (accept(descriptor_socket,(struct sockaddr *)& adresa, (socklen_t*)&adress_len)<0){
        // aici cred ca e while.. si nu stiu daca e ok cu param.
        perror("server: pb la accept");
        exit(3);
    }
}


