

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <ctype.h>
#include <strings.h>

#define AWS_TCP_PORT 25704

void error(const char *msg)
{
    	perror(msg);
    	exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    //struct hostent *server;
	
    //Buffer stores the nums.csv, reduction stores the function
    char buffer[1024];
    char function[8];
    char count[4];
    float recv_num;

    

//*********** Sending Reduction Type to AWS Using TCP*************
//*********** This part is modified from http://www.linuxhowtos.org/data/6/client.c *********
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("The client is up and running.\n");
    if (sockfd < 0)  error("ERROR opening socket");
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    //bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    //bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(AWS_TCP_PORT);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)  error("ERROR connecting");  	
	

//***********Sending the numbers(300 in this case) firstly, then send integer array ***********
    recv_num=atof(argv[2]);
    n = send(sockfd,(char *)&recv_num,sizeof(recv_num),0);
    if (n < 0) error("ERROR writing to socket");
    if (strcmp(argv[1],"LOG")==0) strcpy(function, "LOG");
    else if (strcmp(argv[1],"DIV")==0) strcpy(function, "DIV");
    send(sockfd,function,sizeof(function),0);
    printf("The client sent %f and %s to AWS.\n",recv_num,function);

// The result of reduction
    float result;

    n = recv(sockfd,(char *)&result,sizeof(result),0);
    if (n < 0) error("ERROR reading from socket");
    printf("According to AWS %s on %f:%f\n",function,recv_num,result);

    close(sockfd);
    return 0;
}
