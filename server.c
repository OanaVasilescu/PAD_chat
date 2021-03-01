//
//  server.c
//  chat
//
//  Created by Oana Vasilescu on 26/02/2021.
//

/*
#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

void server(char adresa_ip[], int port) // adresa ip nu stiu daca e int... cred ca char mai degraba
{
    char *hello = "Hello from server";
    char buffer[1024] = {0};
    int valread;
    int descriptor_socket;
    struct sockaddr_in adresa;
    int adress_len = sizeof(adresa);
    if((descriptor_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))<0){
        // INTREABA: s-ar putea ca argumentele sa nu fie ok asa; la ultima chestie unii puneau 0
        perror("la server nu s-a putut face socket");
        exit(0);
        
    }
    memset(&adresa, '\0', sizeof(adresa)); 
	adresa.sin_family=AF_INET;
	adresa.sin_port=htons(port);
    adresa.sin_addr.s_addr=inet_addr(adresa_ip); // nu stiu daca & e ok sau nu
    
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
    valread = read(descriptor_socket , buffer, 1024);
        printf("%s\n",buffer );
        send(descriptor_socket , hello , strlen(hello) , 0 );
        printf("Hello message sent\n");

}

int main(int argc, const char * argv[]){
    server("192.168.1.5", 8080);
}
 
 */


#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
       
    // Creating socket file descriptor
    if ((server_fd = socket(PF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
       
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    valread = read( new_socket , buffer, 1024);
    printf("%s\n",buffer );
    send(new_socket , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    return 0;
}


