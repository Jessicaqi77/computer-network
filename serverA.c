
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>
#define Server_A_UDP_PORT 21704
#define AWS_UDP_PORT 24704
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main()
{
   int sock, length, n,sock_send;
   socklen_t;
   struct sockaddr_in server;//receive
   struct sockaddr_in aws_send;//send to aws
   struct sockaddr_in from;
   int fromlen=sizeof(struct sockaddr_in);
   char buf[1024];

//****************UDP Establish **************
//This part is modified from http://www.linuxhowtos.org/C_C++/socket.htm	
   sock=socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("Opening socket");
   length = sizeof(server);
   bzero(&server,length);
   server.sin_family=AF_INET;
   server.sin_addr.s_addr=INADDR_ANY;
   server.sin_port=htons(Server_A_UDP_PORT);
   loop:
   bind(sock,(struct sockaddr *)&server,length);
   fromlen = sizeof(struct sockaddr_in);
   printf("The Server A is up and running using UDP on port %d\n",htons(server.sin_port));

// Received the total counts of numbers and the actual numbers

        float recv_num1, recv_num2;
        //receive
	n = recvfrom(sock,(char *)&recv_num1,sizeof(recv_num1),0,(struct sockaddr*)&from,&fromlen);
	if (n < 0) error("recvfrom");
	printf("The Server A  received input %f\n", recv_num1);
        float mula=recv_num1*recv_num1;
        printf("The Server A  calculated square: %f\n", mula);
        //send
        sock_send=socket(AF_INET, SOCK_DGRAM, 0);
        aws_send.sin_family=AF_INET;
        aws_send.sin_addr.s_addr = inet_addr("127.0.0.1");
        aws_send.sin_port=htons(AWS_UDP_PORT);
        sendto(sock_send,(char *)&mula,sizeof(mula),0,(struct sockaddr *)&aws_send,fromlen);
        //if (m  < 0) error("sendto");
	printf("The Server A  finished sending the output to AWS . \n");
        close(sock_send);
     goto loop;
 
 }

