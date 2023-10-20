/* upperserver.c: un server per rendere maiuscole linee di
testo */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/times.h>
#define SERVER_PORT 1313
#define LINESIZE 80

void upperlines(int in, int out) {
	char inputline[LINESIZE];
	int len, i;
	while ((len = recv(in, inputline, LINESIZE, 0)) > 0) {
		for (i=0; i < len-1; i+=2)
			inputline[i] = toupper(inputline[i]);
	send(out, inputline, len, 0);
	}
}
int main (int argc, char **argv) {
	int sockfd, client_len, clientfd;
	struct sockaddr_in server,client;
	/* impostazione dell’end point della comunicazione */
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("chiamata alla system call socket fallita");
		exit(1);
	}
	//imposto l'indirizzo
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY); //htonl per l'ordinamento dei byte univoco
	server.sin_port = htons(SERVER_PORT);
	/* leghiamo l'indirizzo all’end point della comunicazione */
	if (bind(sockfd, (struct sockaddr*)&server, sizeof(server))==-1) {
		perror("chiamata alla system call bind fallita");
		exit(2);
	}
	listen(sockfd, 1);
	/* gestione delle connessioni dei client */
	while (1) {
		client_len = sizeof(client);
		if ((clientfd = accept(sockfd, (struct sockaddr*)&client, &client_len))<0){
			perror("accepting connection");
			exit(3);
		}
	fprintf(stdout, "Aperta connessione.\n");
	send(clientfd, "Benvenuto allo Spongebob server!\n", 32, 0);
	upperlines(clientfd, clientfd);
	close(clientfd);
	fprintf(stdout, "Chiusa connessione.\n");
	}
}
