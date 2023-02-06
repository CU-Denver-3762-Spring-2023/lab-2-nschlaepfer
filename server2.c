#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>



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
//Function to bind socket (port bumber and socket descriptor)
  //Creates variables to hold port info
  //Using AF_INET = IPv4 addy
  //htons to convert
  //returns 0 if successful
int bind_socket(int sd, int portNumber){
    struct sockaddr_in server_address; /* my address */
    server_address.sin_family = AF_INET; /* use AF_INET addresses */
    server_address.sin_port = htons(portNumber); /* convert port number */
    server_address.sin_addr.s_addr = INADDR_ANY; /* any adapter */
    int rc = bind (sd, (struct sockaddr *)&server_address,
         sizeof(struct sockaddr ));
    if (rc < 0){
        perror("bind");
        exit (1);
    }
    return 0;
}



//structure to store keyvalue pairs
struct  _tokens {
  char key[1000];
  char value [1000];
};
struct _tokens tokens [100];


//where should this go?
void cleanse (char * buffer){

  char *startPtr;
  int count = 0;
  int i;
  startPtr = strstr (buffer, "\"");

  /* replace the ' ' in the string with & so tokenize will work */
  if (startPtr != NULL){
    for(i=0; i<strlen(startPtr) && count < 2 ;i++){
      if (startPtr[i] == '"'){
 count ++;
      }
      else if (startPtr[i] == ' '){
 startPtr[i] = '^';
      }
    }
  }
}






int findTokens(char *buffer , struct _tokens *tokens){
  
  int counter= 0; // how many tokens did i find!
  char * ptr;

  /* recall the protocol says the key:value are separated by ' ' */
  /* so i will tokenize first on the ' '                         */
  
  ptr = strtok (buffer," "); 
  while (ptr != NULL) // keep going until no more ' ' 
    {

      int i = 0; // KMT would be laughing
      int flag = 0; //
      int k = 0; 
      memset (tokens[counter].key, 0, 100);
      memset (tokens[counter].value, 0, 100);

      for (i=0;i<strlen(ptr);i++){
 if (flag ==0){ // doing the key portion
   if (ptr[i] != ':')
     tokens[counter].key[i] = ptr[i];
   else // must be a ':'
     flag = 1;
 }
 else{ // doing the value portion
   if (ptr[i] == '^'){ // undoing the cleansing of msg portion
     ptr[i] = ' ';
   }
   tokens[counter].value[k] = ptr[i];
   k++;
 }
      }
      //      sprintf (tokens[counter].key, "%s", ptr); // fix here. late night
      ptr = strtok (NULL, " ");
      counter ++;
    }
  return counter;
}





//Main fucntion to call and create socket descriptor
int main(int argc, char *argv[])
{
  int sd; /* socket descriptor */
  /* first, decide if we have the right number of parameters */
  if (argc < 2){
    printf("usage is: server <portnumber>\n");
    exit (1);
  }
  //char* serverIP = argv[1];
  //check_ip_validity(serverIP); //calls function
  //int portNumber = check_port_validity(argv[2]); //do i need this?
  sd = socket(AF_INET, SOCK_DGRAM, 0); /* create a socket */
  /* always check for errors */
  if (sd == -1){
    perror("socket error");
    exit(1);

  } /* means some kind of error occured */



  //int reveived_data_result = receive_data(sd); //calls the receive function to print

  return 0;


}