/* Server side - UDP Code				    */
/* By Hugh Smith	4/1/2017	*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "gethostbyname.h"
#include "networks.h"
#include "safeUtil.h"
#include "PDU.h"
#include "cpe464.h"

#define MAXBUF 80

void processClient(int socketNum);
int checkArgs(int argc, char *argv[]);

int main ( int argc, char *argv[]  )
{ 
	sendErr_init(atof(argv[1]), DROP_OFF, FLIP_ON, DEBUG_OFF, RSEED_ON);

	int socketNum = 0;				
	int portNumber = 0;

	portNumber = checkArgs(argc, argv);
	socketNum = udpServerSetup(portNumber);

	processClient(socketNum);

	close(socketNum);
	
	return 0;
}

void processClient(int socketNum)
{
	int dataLen = 0; 
	char buffer[MAXBUF + 1];	  
	struct sockaddr_in6 client;		
	socklen_t clientAddrLen = sizeof(client);	
	
	buffer[0] = '\0';
	while (buffer[0] != '.')
	{
		dataLen = recvfromErr(socketNum, buffer, MAXBUF, 0, (struct sockaddr *) &client, &clientAddrLen);//safeRecvfrom(socketNum, buffer, MAXBUF, 0, (struct sockaddr *) &client, &clientAddrLen);
	
		printf("Received message from client with ");
		printPDU((uint8_t*)buffer, 7 + strlen((char*)(buffer + 7)));
		// printIPInfo(&client);
		// printf(" Len: %d \'%s\'\n", dataLen, buffer);

		// just for fun send back to client number of bytes received
		// sprintf(buffer, "bytes: %d", dataLen);
		sendtoErr(socketNum, buffer, dataLen, 0, (struct sockaddr *) & client, clientAddrLen);// safeSendto(socketNum, buffer, dataLen, 0, (struct sockaddr *) & client, clientAddrLen);
	}
}

int checkArgs(int argc, char *argv[])
{
	// Checks args and returns port number
	int portNumber = 0;
	
	if (argc > 3)
	{
		fprintf(stderr, "Usage %s [optional port number]\n", argv[0]);
		exit(-1);
	}
	else if(argc < 2){
		fprintf(stderr, "missing error rate"); 
		exit(-1); 
	}
	
	if (argc == 3)
	{
		portNumber = atoi(argv[2]);
	}
	
	return portNumber;
}


