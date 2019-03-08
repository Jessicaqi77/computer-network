// A AWS server with TCP and UDP in the internet domain 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <strings.h>

#define Server_A_UDP_PORT 21704
#define Server_B_UDP_PORT 22704
#define Server_C_UDP_PORT 23704
#define AWS_UDP_PORT 24704
#define AWS_TCP_PORT 25704

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main()
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[3];
    char function[8];
    float recv_num;
    struct sockaddr_in serv_addr, cli_addr,cli_AWS;
    int n;
   

//************** TCP Establish and receive function ********************
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(AWS_TCP_PORT);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
    error("ERROR on binding");
    listen(sockfd,5);
    printf("The AWS is up and running.\n");
    clilen = sizeof(cli_addr);
    
//****************TCP end*************

// Here we assign the UDP information
//This part is modified from http://www.linuxhowtos.org/C_C++/socket.htm	
	int sock_udp, n_udp;
	int length_udp;
	struct sockaddr_in server_C,server_A,server_B,from;
	struct hostent *hp;
	char buffer_udp[256];	

        server_C.sin_family = AF_INET;
	//hp = gethostbyname("localhost");
	//if(hp == 0) error("Unknown host");
	//bcopy((char *)hp->h_addr,(char *)&server_C.sin_addr,hp->h_length);
	server_C.sin_port = htons(Server_C_UDP_PORT);
        server_C.sin_addr.s_addr=inet_addr("127.0.0.1");
	length_udp = sizeof(struct sockaddr_in);
	bzero(buffer_udp,256);
	
	server_A.sin_family = AF_INET;
        server_A.sin_addr.s_addr=inet_addr("127.0.0.1");
	//bcopy((char *)hp -> h_addr,(char *)&server_A.sin_addr,hp -> h_length);
	server_A.sin_port = htons(Server_A_UDP_PORT);

	server_B.sin_family = AF_INET;
        server_B.sin_addr.s_addr=inet_addr("127.0.0.1");
	//bcopy((char *)hp -> h_addr,(char *)&server_B.sin_addr,hp -> h_length);
	server_B.sin_port = htons(Server_B_UDP_PORT);

        sock_udp = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock_udp < 0) error("socket");
        cli_AWS.sin_family = AF_INET;
        cli_AWS.sin_port=htons(AWS_UDP_PORT);
        cli_AWS.sin_addr.s_addr=inet_addr("127.0.0.1");
        loop:
        bind(sock_udp,(struct sockaddr *)& cli_AWS, sizeof(serv_addr));	
	
        while(1){
   
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) error("ERROR on accept");
        //bzero(buffer,4);
        n = recv(newsockfd,(char *)&recv_num,sizeof(recv_num),0);
        if (n < 0) error("ERROR reading from socket");
        n = recv(newsockfd,(char *)&function,sizeof(function),0);
        if (n < 0) error("ERROR reading from socket");
        printf("The AWS recevied %f and function= %s from the client using TCP over port 25704\n",recv_num,function);
 
//Dealing with A:       
        int n_udp1;
        n_udp1=socket(AF_INET,SOCK_DGRAM,0);
	sendto(n_udp1,(char *)&recv_num,sizeof(recv_num),0,(const struct sockaddr *)&server_A,length_udp);
	//if (n_udp1 < 0 ) error("sendto");
	printf("The AWS sent %f to Backend-Server A\n",recv_num);
	close(n_udp1);
        float mula;
	recvfrom(sock_udp,(char *)&mula,sizeof(mula),0,(struct sockaddr *)&from,&length_udp);
	printf("The AWS received %f Backend-Server A using UDP over port 24704\n",mula);
        float val=mula;

        int socksent_A; 
        socksent_A = socket(AF_INET, SOCK_DGRAM, 0);
        //send the divided number to serverC
        sendto(socksent_A, (char *)&val, sizeof(val), 0, (struct sockaddr *)&server_A, length_udp);
        printf("The AWS sent %f to Backend-Server A\n", val);
        //send the funtion type to serverC
        close(socksent_A);
        // receive the result from serverA
        float mula2;
        recvfrom(sock_udp, (char *)&mula2, sizeof(mula2), 0, (struct sockaddr *)&from, &length_udp);
        printf("The AWS received %f Backend-Server A using UDP over port 24704\n",mula2);
        
	
//Dealing with B
        int n_udp2;
        n_udp2=socket(AF_INET,SOCK_DGRAM,0);
	sendto(n_udp2,(char *)&recv_num,sizeof(recv_num),0,(const struct sockaddr *)&server_B,length_udp);
        //if (n_udp2 < 0 ) error("sendto");
	printf("The AWS has sent %f numbers to Backend-Server B\n",recv_num);
	close(n_udp2);
        float mulb;
	recvfrom(sock_udp,(char *)&mulb,sizeof(mulb),0,(struct sockaddr *)&from,&length_udp);
	printf("The AWS received %f Backend-Server B using UDP over port 24704 \n",mulb);

        int socksent_B; // process in UDP
        socksent_B= socket(AF_INET, SOCK_DGRAM, 0);
        //send the number to serverB
        sendto(socksent_B, (char *)&val, sizeof(val), 0, (struct sockaddr *)&server_B, length_udp);
        printf("The AWS sent %f number to Backend-Server B\n", val);
        close(socksent_B);
        // receive the result from serverB
        float mulb2;
        recvfrom(sock_udp, (char *)&mulb2, sizeof(mulb2), 0, (struct sockaddr *)&from, &length_udp);
        printf("The AWS received %f Backend-Server B using UDP over port 24704\n",mulb2);
        

// Dealing with Server C, send data first, then opearion

	int n_udp3;
        n_udp3=socket(AF_INET,SOCK_DGRAM,0);
	sendto(n_udp3,(char *)&recv_num,sizeof(recv_num),0,(const struct sockaddr *)&server_C,length_udp);
        //if (n_udp3 < 0 ) error("sendto");
	printf("The AWS has sent %f numbers to Backend-Server C\n",recv_num);
	close(n_udp3);
        float mulc;
	recvfrom(sock_udp,(char *)&mulc,sizeof(mulc),0,(struct sockaddr *)&from,&length_udp);
	printf("The AWS received %f Backend-Server B using UDP over port 24704\n",mulc);
        

        //compute the final value
        printf("Values of powers received by AWS:%f,%f,%f,%f,%f,%f \n",recv_num,mula,mulb,mula2,mulc,mulb2);
        float final;
        if(strcmp(function, "DIV")==0)
        final=1+recv_num+mula+mulb+mula2+mulc+mulb2;
        else if (strcmp(function, "LOG")==0)
        final = -1*(recv_num+mula/2+mulb/3+mula2/4+mulc/5+mulb2/6);
        printf("AWS calculated %s on %f: %f\n", function,recv_num, final);
        send(newsockfd, (char *)&final, sizeof(final), 0);
        printf("The AWS sent %f to client\n",final);
        goto loop;
      }
	close(newsockfd);
	close(sockfd);
	close(sock_udp);
    return 0; 
}

