#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_LENGTH 1024

#define EXIT_INCORRECT_USAGE_OF_ARGUMENTS 1
#define EXIT_RECV_FUNCTION_CLIENT_FAILURE 2
#define EXIT_SOCKET_FUNCTION_FAILURE 3
#define EXIT_CONNECT_FUNCTION_FAILURE 4
#define EXIT_CLOSE_FUNCTION_FAILURE 5

void trim_name(char *arr, int length){
    int i;
    for(i=0;i<length;i++){
        if(arr[i]=='\n'){
        arr[i]='\0';
        break;
        }
     }
}

void* functie_mesaj(void* args){
    int socket_cl = *(int*)args;
    while (1) {
        char data[MAX_LENGTH];
        int read = recv(socket_cl, data, MAX_LENGTH, 0);
        if(read < 0){
            perror("Error! Message can't be received!\n");
            exit(EXIT_RECV_FUNCTION_CLIENT_FAILURE);
        }
        data[read] = '\0';
        printf("%s", data);
    }
}
void client(char adresa[], int port){
    int clientSocket;
    struct sockaddr_in serverAdrr;
    char name[10];
    
    if ((clientSocket=socket(PF_INET, SOCK_STREAM, 0))<0){
        printf("Error! The client socket can't be created!\n");
        exit(EXIT_SOCKET_FUNCTION_FAILURE);
    }
    
    memset(&serverAdrr, '\0', sizeof(serverAdrr));
    serverAdrr.sin_family=AF_INET;
    serverAdrr.sin_port=htons(port);
    serverAdrr.sin_addr.s_addr=inet_addr(adresa);
    
    if((connect(clientSocket, (struct sockaddr*)&serverAdrr, sizeof(serverAdrr))) <0){
        printf("Error! Client can't connect to the server!\n");
        exit(EXIT_CONNECT_FUNCTION_FAILURE);
    }
    else{
        puts("Conectat la server\n");
        printf("Enter your name:");
        fgets(name,32,stdin);
        printf("\n");
        trim_name(name,strlen(name));
        pthread_t thread_mesaj;
        pthread_create(&thread_mesaj, NULL, functie_mesaj, &clientSocket);
        while (1) {
            char input[MAX_LENGTH]={};
            char buffer[MAX_LENGTH + 32]={};
            fgets(input, MAX_LENGTH, stdin);
            printf("\n");
            sprintf(buffer,"%s: %s\n", name, input);
            send(clientSocket, buffer, strlen(buffer), 0);
            
        }
    }
    if (close(clientSocket) <0){
        printf("Error! Client Socket can't be closed correctly!");
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
    client(adresaIP,port);
    return 0;
}
