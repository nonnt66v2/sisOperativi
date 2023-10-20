#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(){

	int server_socketfd, client_socketfd;
	int server_len, client_len;
	struct sockaddr_un server_address;
	struct sockaddr_un client_address;
	
	//rimuoviamo eventuali vecchie socket e creiamo una socket senza nome per il server
	unlink("server_socket");
	server_socketfd= socket(AF_UNIX, SOCK_STREAM, 0);
	//assegnamo nome alla socket
	server_address.sun_family=AF_UNIX;
	strcpy(server_address.sun_path, "server_socket");
	server_len=sizeof(server_address);
	bind(server_socketfd, (struct sockaddr*)&server_address, server_len);
	//creiamo una coda di connessione e aspettiamo i client
	listen(server_socketfd, 5);
	while(1){
		char ch;
		printf("server in attesa\n");
		
		client_len=sizeof(client_address);
		client_socketfd=accept(server_socketfd, (struct sockaddr*)&client_address, &client_len);
		
		//leggiamo e scriviamo dal server
		read(client_socketfd, &ch, 1);
		ch++;
		write(client_socketfd, &ch, 1);
		close(client_socketfd);
	}
	exit(0);
}
