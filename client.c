#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BUFSIZE 1024
//#define PORT 9000

void client(char adresa, int port){
	char buf[BUFSIZE];
	int clientSocket;
	struct sockaddr_in serverAdrr;
	if ((clientSocket=socket(PF_INET, SOCK_STREAM, 0))<0){
		perror("eroare socket");
		exit(2);
	}
    memset(&serverAdrr, '\0', sizeof(serverAdrr)); //Alocam un bloc de memorie doar pentru structura asta;
	serverAdrr.sin_family=AF_INET;
	serverAdrr.sin_port=htons(port);
    serverAdrr.sin_addr.s_addr=inet_addr(&adresa); // nu stiu daca e & sau fara&
	connect(clientSocket, (struct sockaddr*)&serverAdrr, sizeof(serverAdrr));
	
	recv(clientSocket, buf, sizeof(buf), 0);
	
	printf("data : %s", buf);
} 


