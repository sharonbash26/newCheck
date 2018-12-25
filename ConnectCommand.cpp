#include "ConnectCommand.h"

int ConnectCommand::execute() {
std::cout<<this->ip<<" and PORT ************************************************ "<<this->port<<"\n";
std::cout<<"YYYYYYYYYYYYYY "<<bindedVariables.size()<<"\n";
std::cout<<"YYYYYYYYYYYYYY "<<bindMap.size()<<"\n";
    for (int i = 0 ; i < bindedVariables.size(); i++) {
        std::cout<<"HHHHHHHH "<< bindedVariables[i]
            <<": "<<varMap[bindedVariables[i]]<<": "<<bindMap[bindedVariables[i]]<<"\n";
        
        this->connectToServer(varMap[bindedVariables[i]],bindMap[bindedVariables[i]]);
    }
    
    return 3;
}

void ConnectCommand::connectToServer(std::string value, std::string link) {
    int sockfd, portno, n;
   struct sockaddr_in serv_addr;
   struct hostent *server;
   
   char buffer[256];
	
   portno = stod(this->port);
   
   /* Create a socket point */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
	
   server = gethostbyname(this->ip.c_str());
   
   if (server == NULL) {
      fprintf(stderr,"ERROR, no such host\n");
      exit(0);
   }
   
   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
   serv_addr.sin_port = htons(portno);
   
   /* Now connect to the server */
   if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR connecting");
      exit(1);
   }
   
   /* Now ask for a message from the user, this message
      * will be read by server
   */
	
   printf("Please enter the message: ");
   bzero(buffer,256);
  // fgets(buffer,255,stdin);

    if (link[0] == '\"' && link[link.length() - 1] == '\"' && link.length() > 1) {
        link = link.substr(1, link.length() - 2);
    } 

  std::string command = "set ";
  command += link;
  command += " ";
  command += value;
  std::cout<<"77777777777777777777777777: "<<command<<"\n";
  std::cout<<"88888888888888888888888888: "<<buffer<<"\n";
   strcpy(buffer, command.c_str());
   strcat(buffer, "\r\n");
   
   /* Send message to the server */
   n = write(sockfd, buffer, strlen(buffer));
   
   if (n < 0) {
      perror("ERROR writing to socket");
      exit(1);
   }
   
   /* Now read server response */
   bzero(buffer,256);
   n = read(sockfd, buffer, 255);
   
   if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
   }
	
   printf("%s\n",buffer);
}