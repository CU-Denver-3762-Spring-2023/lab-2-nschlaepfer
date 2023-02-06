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

//#define buffer 10000

//Function to check the validity of the entred ip
  //Takes in char pointer wich has ip in string format
  //inet_pton() checks if it is valid?
  //Converts Ip from text to binary.
  //If not valid exits program
  //if valid returns 0
int check_ip_validity(char* ip_address){
    struct sockaddr_in inaddr;  /* structures for checking addresses */
    if (!inet_pton(AF_INET, ip_address, &inaddr)){
        printf ("error, bad ip address\n");
        exit (1); /* just leave if is incorrect */
    }
    return 0;
}
//Function to check the validity of the port
  //first checks if string is a digit else exits
  //Then if true converts string to int using strol
  //checks if it is within range of the ~64k list
  //then returns value of port number
  //Else exits
int check_port_validity(char* port){
    int i;
    for (i=0;i<strlen(port); i++){
    if (!isdigit(port[i]))
      {
        printf ("The Portnumber isn't a number!\n");
        exit(1);
      }
    }
    int portNumber = strtol(port, NULL, 10); /* many ways to do this */
    /* exit if a portnumber too big or too small  */
    if ((portNumber > 65535) || (portNumber < 0)){
        printf ("you entered an invalid socket number\n");
        exit (1);
    }
    return portNumber;
}

int send_data(int sd, char* serverIP, int portNumber, char* bufferOut){

    struct sockaddr_in server_address;  /* structures for addresses */
    server_address.sin_family = AF_INET; /* use AF_INET addresses */
    server_address.sin_port = htons(portNumber); /* convert port number */
    server_address.sin_addr.s_addr = inet_addr(serverIP); /* convert IPaddr */
    memset (bufferOut, 0, 10000); // always null out buffers in C before using them
    
    int sendResult = sendto(sd, bufferOut, strlen(bufferOut), 0, (struct sockaddr*) &server_address, sizeof(server_address));
    if (sendResult < 0) {
        perror("Error sending data");
        exit(EXIT_FAILURE);
    }
    return 0;
}

//opens the file and sends the contents 
  //Makes pointer for file name
  //opens file else return error
  //sends data while theres stuff in bugger
void send_file_contents(char* fileName, int sd, char* serverIP, int portNumber){
    char buffer[10000]; //does this need to be bigger?
    FILE *fp;
    fp = fopen(fileName, "r"); //read mode
    if (fp == NULL)
    {
        perror("Error opening file"); //protection
        exit(EXIT_FAILURE);
    }
    while (fgets(buffer, 10000, fp) != NULL) {
        send_data(sd, serverIP, portNumber, buffer);
    }
    fclose(fp);
}

//Main function to call the other functions inlucdes prompt message.
  //Takes in 2 arg IP and port number (CL arguments)
  //Assigns the IP and port number to variables
  //Validates IP and Portnumber (calls the function)
  //After validation creates socket to the IP and port and then closes it.
int main(int argc, char *argv[])
{
  int sd; /* the socket descriptor */
  /* check to see if the right number of parameters was entered */
  if (argc < 3){
    printf ("usage is client <ipaddr> <portnumber>\n");
    exit(1); /* just leave if wrong number entered */
  }
  char* serverIP = argv[1];
  check_ip_validity(serverIP); //calls function
  int portNumber = check_port_validity(argv[2]); //
  sd = socket(AF_INET, SOCK_DGRAM, 0); /* create a socket */
  if (sd == -1){
        perror("socket");
        exit(1);
    }

    send_file_contents("messages.txt", sd, serverIP, portNumber);

  //send_data(sd, serverIP, portNumber);
  close(sd);

}
