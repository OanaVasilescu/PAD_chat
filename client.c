#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void client(char adresa[], int port){
    int clientSocket;
    struct sockaddr_in serverAdrr;
    if ((clientSocket=socket(PF_INET, SOCK_STREAM, 0))<0){
        perror("eroare socket");
        exit(2);
    }
    memset(&serverAdrr, '\0', sizeof(serverAdrr)); //Alocam un bloc de memorie doar pentru structura asta;
    serverAdrr.sin_family=AF_INET;
    serverAdrr.sin_port=htons(port);
    serverAdrr.sin_addr.s_addr=inet_addr(adresa);
    if((connect(clientSocket, (struct sockaddr*)&serverAdrr, sizeof(serverAdrr))) <0){
        perror("client: pb la connect");
    }
    else{
        puts("conectat la server\n");
        while (1) {
            printf("mesaj:");
            char input[1024];
            fgets(input, 1024, stdin);
            send(clientSocket, input, 1024, 0);
            printf("\n");
            
            
        }
    }
    //close(clientSocket);
}

int main(int argc, const char * argv[]){
    int port;
    char adresaIP[14] = {0};
    if(argc!= 3){
        perror("nr gresit de argumente");
        exit(5);
    }
    strcpy(adresaIP, argv[1]);
    port = atoi(argv[2]);
    client(adresaIP,port);
    return 0;
}






