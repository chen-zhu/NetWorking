/*
*
* The command line arguments should be:
* ./EmailSender <recipient address>
*
* NOTE:
* 1- I would like you to hardcode the following:
*	a- Sender address: "MAIL FROM".
*	b- Message you are sending.
* 2- Pass the recipient address "RCPT TO" as a command line argument.
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <string.h>

#define SMTP_PORT 25
#define MAXLINE 1024 		

char *Server_IP = "128.111.53.4";//ucsb engineering 

void main (int argc, char ** argv)
{
	char sendline[MAXLINE], recvline[MAXLINE];
	char recipient_address[MAXLINE]; // "RCPT TO" address.
	char sender_address[MAXLINE];	 // "MAIL FROM" adress.
	char *temp;
	
	if (argc != 2)
	{
		printf("usage: ./EmailSender <recipient address,\"RCPT TO\">\n");
		exit(0);
	}
	strcpy(recipient_address, argv[1]);

	/* Establish a TCP connection with the main server */
	int sockfd;
	struct sockaddr_in serveraddr;

	
	sockfd = socket(AF_INET,SOCK_STREAM,0); 
	
	bzero(&serveraddr,sizeof(serveraddr)); 
	serveraddr.sin_family = AF_INET; 
	serveraddr.sin_addr.s_addr = inet_addr(Server_IP); 
	serveraddr.sin_port=htons(SMTP_PORT); 
	
	connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	
	/* Read greeting from the server */
	read(sockfd, recvline, MAXLINE);
	printf("%s\n", recvline);	
	temp = strtok(recvline, " ");
	if (strcmp(temp, "220")!=0)
	{
		printf("220 reply ont received from server.\n");
		exit(0);
	}
	
	/* Send HELO command and get server response */
	strcpy(sendline, "HELO alice\r\n");
	printf("%s\n", sendline);
	write(sockfd, sendline, strlen(sendline));

	read(sockfd, recvline, MAXLINE);
	printf("%s\n", recvline);
	temp = strtok(recvline, " ");
	if (strcmp(temp, "250")!=0)
	{
		printf("250 reply not received from server.\n");
		exit(0);
	}


	//shut down the correspnding socket. 
	shutdown(sockfd, 2); 
	exit(0);	
}  