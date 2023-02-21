#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#define SIZE 1000


/*********************************************************/
/* this is the server for lab0. it will receive a msg    */
/* from the client, and print it out. Make this your     */
/* own. Comment it, modularize it, learn it, fix it, etc */
/*********************************************************/
int retrieveMsg(int sd, struct sockaddr_in server_address);
void write_file(int sd, struct sockaddr_in server_address);
struct field {
  char key[200];
  char value[200];
};

void write_file(int sd, struct sockaddr_in server_address){
  FILE *fp;
  char *filename = "server.txt";
  int n;
  char buffer[SIZE];
  socklen_t addr_size;
  char* strParse;
  char *key, *value;
  const char *delim = " :\"";
  // Creating a file.
  fp = fopen(filename, "w");
  if (fp == NULL) {
    perror("Error opening file");
  return;
}
  // Receiving the data and writing it into the file.
  while(1){

    addr_size = sizeof(server_address);
    n = recvfrom(sd, buffer, SIZE, 0, (struct sockaddr*)&server_address, &addr_size);
    if (n < 0) {
      perror("Error receiving data");
    return;
    }
      // Parsing the buffer using strtok
    strParse = strtok(buffer, delim);
    int i = 0;
    while (strParse != NULL) {

      if (i % 2 == 0) {
        key = strParse;
      } else {
        value = strParse;
        // Check if the value is surrounded by quotes
        if (value[strlen(value) - 1] == '"') {
          value[strlen(value) - 1] = '\0';
          }

        if (value[0] == '"') {
          // Print the whole string inside the quotes
          printf("%s: %s\n", key, &value[i]);
          break;
        } else {
        printf("%12s%15s\n",key,value);
        
      }
          }
      i++;
      strParse = strtok(NULL, delim);
    }
    memset(buffer, 0, SIZE);
  }
    fclose(fp);
    return;
  }

int retrieveMsg(int sd, struct sockaddr_in server_address){ //Retrieves the message from the client and spits out the string
  char bufferReceived[1000]; // used in recvfrom
  int rc; // always need to check return codes!
  struct sockaddr_in from_address;  /* address of sender */
  socklen_t fromLength;
  int flags = 0; // used for recvfrom
  
  memset (bufferReceived, 0, 1000); // always zero out the buffer before using it

  /* NOTE - you MUST MUST MUST give fromLength an initial value */
  fromLength = sizeof(struct sockaddr_in);

  rc = recvfrom(sd, bufferReceived, 1000, flags,
		(struct sockaddr *)&from_address, &fromLength); //receive buffer from the client

  /* check for any possible errors and exit if any are found */
  if (rc <=0){
    perror ("recvfrom");
    printf ("leaving, due to socket error on recvfrom\n");
    exit (1);
  }
 
  /* print out what the we receive from the client*/
  printf ("RECEIVED DATA: \n");
  return 0;

}

int main(int argc, char *argv[])
{
  int sd; /* socket descriptor */
  int i;  /* loop variable */
  struct sockaddr_in server_address; /* my address */
  int port = 0; // get this from command line
  int e;
  /* first, decide if we have the right number of parameters */
  if (argc < 2){
    printf("usage is: server <portnumber>\n");
    exit (1);
  }
    /* now fill in the address data structure we use to sendto the server */
  for (i=0;i<strlen(argv[1]); i++){
    if (!isdigit(argv[1][i]))
      {
	printf ("The portnumber isn't a number!\n"); //tell the user that the input was invalid
	exit(1);
      }
  }

  port = strtol(argv[1], NULL, 10); /* many ways to do this */

  if ((port > 65535) || (port < 0)){
  printf ("you entered an invalid port number\n"); //lets the user know that the port number was beyond the valid range
  exit (1);
    }
  
    /* first create a socket */
  sd = socket(AF_INET, SOCK_DGRAM, 0); /* create a socket */
    /* always check for errors */
  if (sd == -1){ /* means some kind of error occured */
    perror ("socket");
    exit(1); /* just leave if wrong number entered */
  }
  /* Initialize the server address structure */
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET; /* use AF_INET addresses */
  server_address.sin_port = htons(port); /* convert port number */
  server_address.sin_addr.s_addr = INADDR_ANY; /* any adapter */

  /* bind to the client address */
  e = bind (sd, (struct sockaddr *)&server_address,
	     sizeof(struct sockaddr )); //Binds the server address from the client to the server
  
  if (e < 0){
    perror("bind");
    exit (1);
  }

  /* call the function to retrieve messages */
  retrieveMsg(sd, server_address);
  for(;;){
  write_file(sd,server_address);
  printf("%s%10d", "port", port);
  }
  /* call the function to write to the file */
  /*write_file(sd, server_address);*/
  return 0;
}
