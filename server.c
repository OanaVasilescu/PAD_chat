#include <stdio.h>
#include <pthread.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define MAX_CLIENTS 40 //Numarul maxim de clienti ce pot utliza chatroom-ul
#define MAX_LENGTH 1024

#define EXIT_INCORRECT_USAGE_OF_ARGUMENTS 1
#define EXIT_RECV_FUNCTION_SERVER_FAILURE 2
#define EXIT_SERVER_SOCKET_CREATION_FAILURE 3
#define EXIT_BIND_FUNCTION_FAILURE 4
#define EXIT_LISTEN_FUNCTION_FAILURE 5
#define EXIT_ACCEPT_FUNCTION_FAILURE 6
#define EXIT_CLOSE_FUNCTION_FAILURE 7

int socket_client[MAX_CLIENTS];
int contor = 0;


void* functie_client(void* args){
    int socket_cl = ((int*)args)[0];
    printf("Conectat la client %d\n", socket_cl);
    while (1) {
        char data[MAX_LENGTH];
        int read = recv(socket_cl, data, MAX_LENGTH, 0);
        if(read < 0){
            perror("Error! Message can't be received!\n");
            exit(7);
        }
        data[read] = '\0';
        printf("%s\n", data);
        for (int i = 0; i<=contor; i++){
            if(socket_client[i] != socket_cl)
                send(socket_client[i], data, MAX_LENGTH, 0);
        }
    }
}

void server(char adresa_ip[], int port)
{
    int descriptor_socket;
    struct sockaddr_in adresa;
    int adress_len = sizeof(adresa);
    if((descriptor_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))<0){
        perror("Error! Server socket can't be created!\n");
        exit(EXIT_SERVER_SOCKET_CREATION_FAILURE);
        
    }
    memset(&adresa, '\0', sizeof(adresa));
    adresa.sin_family=AF_INET;
    adresa.sin_port=htons(port);
    adresa.sin_addr.s_addr=inet_addr(adresa_ip);
    
    if(bind(descriptor_socket,(struct sockaddr *)& adresa, sizeof(adresa)) < 0){
        perror("Error! Bind function failure!");
        exit(EXIT_BIND_FUNCTION_FAILURE);
    }
    if(listen(descriptor_socket, 32)<0){
        perror("Error! Listen function failure!");
        exit(EXIT_LISTEN_FUNCTION_FAILURE);
    }
    while(1){
        if ((socket_client[contor] = accept(descriptor_socket,(struct sockaddr *)& adresa, (socklen_t*)&adress_len))<0){
            perror("Error! The connection to the client was not successful!\n");
            exit(EXIT_ACCEPT_FUNCTION_FAILURE);
        }
        pthread_t thread_client;
        pthread_create(&thread_client, NULL, functie_client, &socket_client[contor]);
        contor++;
    }
    if(close(descriptor_socket) < 0){
        printf("Error! Socket descriptor can't be closed correctly!\n");
        exit(EXIT_CLOSE_FUNCTION_FAILURE);
    }
}

int main(int argc, const char * argv[]){
    int port;
    char adresaIP[14] = {0};
    if(argc!= 3){
        printf("Usage: %s <IP address> <port number>", argv[0]);
        exit(EXIT_INCORRECT_USAGE_OF_ARGUMENTS);
    }
    strcpy(adresaIP, argv[1]);
    port = atoi(argv[2]);
    server(adresaIP,port);
}
