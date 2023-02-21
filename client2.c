#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

#define BUFFER_SIZE 1024
#define NUM_SERVERS 4

int check_port_validity(const char *port)
{
  int i;
  for (i = 0; i < strlen(port); i++)
  {
    if (!isdigit(port[i]))
    {
      fprintf(stderr, "The Portnumber isn't a number!\n");
      exit(EXIT_FAILURE);
    }
  }
  long portNumber = strtol(port, NULL, 10);
  if ((portNumber > 65535) || (portNumber < 0))
  {
    fprintf(stderr, "Invalid port number: %ld\n", portNumber);
    exit(EXIT_FAILURE);
  }
  return (int) portNumber;
}

void send_message(FILE *fp, int sockfd, struct sockaddr_in addr) {
  char buffer[BUFFER_SIZE];
  while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
    printf("Sending data: '%s'\n", buffer);
    int n = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&addr, sizeof(addr));
    if (n == -1) {
      perror("Error sending data to the server");
      exit(EXIT_FAILURE);
    }
    bzero(buffer, BUFFER_SIZE);
  }
  // Sending the 'END' message to indicate the end of file
  strcpy(buffer, "END");
  sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&addr, sizeof(addr));
  fclose(fp);
}

void send_to_servers(const char *con_file, const char *msg_file) {
  int sd, i, port;
  struct sockaddr_in addr[NUM_SERVERS];
  char ip[20];
  FILE *fp;

  // Read the config file
  fp = fopen(con_file, "r");
  if (fp == NULL) {
    perror("Unable to read the config file");
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < NUM_SERVERS; i++) {
    if (fscanf(fp, "%s %d", ip, &port) != 2) {
      printf("Error reading config file\n");
      exit(EXIT_FAILURE);
    }

    addr[i].sin_family = AF_INET;
    addr[i].sin_port = htons(port);
    addr[i].sin_addr.s_addr = inet_addr(ip);
  }
  fclose(fp);

  // Open the message file to be sent to the servers
  FILE *msg_fp = fopen(msg_file, "r");
  if (msg_fp == NULL) {
    perror("Unable to read the message file");
    exit(EXIT_FAILURE);
  }

  // Send the message to each server
  for (i = 0; i < NUM_SERVERS; i++) {
    // Create a socket for each server
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
      perror("Error creating socket");
      exit(EXIT_FAILURE);
    }

    send_message(msg_fp, sd, addr[i]);

    // Indicate which server is the client sending to 
    printf("Sending file to server %d\n", i + 1);

    close(sd);
  }

  printf("Data Transferred...\n");
  printf("Disconnecting from the server...\n");
}

int main()
{

    char *conFile = "config.file";
    char *filename = "messages.txt";
    for(;;){
            send_to_servers(conFile, filename);
            break;
    }

    printf("Data Transferred...\n");
    printf("Disconnecting from the server...\n");

    return 0;
    }