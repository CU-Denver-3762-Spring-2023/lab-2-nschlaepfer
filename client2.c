/**********************************************************************/
/*Name: Nicolas W Schlaepfer @nschlaepfer
/*Class: Network Programming MW @ 9:30 am
/*Professor: @daveogle1818
/*Lab Number: 2
/*Due Date: feb 1st @ 11:59 pm                                                                  
/**********************************************************************/
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_PORT 65535
#define MAX_SERVERS 10
#define MAX_IP_LENGTH 16

struct ip_port {
    char ip[MAX_IP_LENGTH];
    int port;
};

int check_ip_validity(char* ip_address){
    struct sockaddr_in inaddr;
    if (!inet_pton(AF_INET, ip_address, &inaddr)){
        printf ("error, bad ip address\n");
        exit (1);
    }
    return 0;
}

int check_port_validity(char* port){
    int i;
    for (i=0;i<strlen(port); i++){
        if (!isdigit(port[i])) {
            printf ("The Portnumber isn't a number!\n");
            exit(1);
        }
    }
    int portNumber = strtol(port, NULL, 10);
    if ((portNumber > MAX_PORT) || (portNumber < 0)){
        printf ("you entered an invalid socket number\n");
        exit (1);
    }
    return portNumber;
}

//function prototype for parse
void parse(const char *line, struct ip_port* server);

void read_config(char* fileName, struct ip_port* servers, int* num_servers){
  char buffer[1000];
  FILE *fp;
  fp = fopen(fileName, "r");
  if (fp == NULL)
  {
    perror("Error opening file");
    exit(EXIT_FAILURE);

  }
  while (fgets(buffer, 1000, fp) != NULL){
    //call parse function here? 
    parse(buffer, &servers[*num_servers]);
    (*num_servers)++;
  }
  fclose(fp);
}

//parse function for the config file to get ip and port
void parse(const char *line, struct ip_port* server) {
    sscanf(line, "%s %d", server->ip, &server->port);
}



int send_data(int sd, struct ip_port* servers, int num_servers, char* bufferOut){
    memset (bufferOut, 0, 10000);
    for (int i = 0; i < num_servers; i++) {
        struct sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(servers[i].port);
        server_address.sin_addr.s_addr = inet_addr(servers[i].ip);
        int sendResult = sendto(sd, bufferOut, strlen(bufferOut), 0, (struct sockaddr*) &server_address, sizeof(server_address));
        if (sendResult < 0) {
            perror("Error sending data");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}

void send_file_contents(char* fileName, int sd, struct ip_port* servers, int num_servers){
    char buffer[10000];
    FILE *fp;
    fp = fopen(fileName, "r");
    if (fp == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    while (fgets(buffer, 10000, fp) != NULL) {
        send_data(sd, servers, num_servers, buffer);
    }
    fclose(fp);
}

int main(int argc, char *argv[])
{
    int sd;
    if (argc < 2) {
        printf ("usage is client <config_file>\n");
        exit(1);
    }
    struct ip_port servers[MAX_SERVERS];
    int num_servers;
    read_config(argv[1], servers, &num_servers);
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd == -1){
        perror("socket");
        exit(1);
    }
    send_file_contents("messages.txt", sd, servers, num_servers);
    close(sd);
    return 0;
}

