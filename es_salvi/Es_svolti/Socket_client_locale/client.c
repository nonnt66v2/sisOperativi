#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	int socket_fd;
	int len;
	struct sockaddr_un address;
	int result;

	char ch='A';
	
	/*creiamo una socket per il client*/

	socket_fd=socket(AF_UNIX, SOCK_STREAM, 0); //creiamo una socket che restituisce il fd della socket, il primo argomento ci dice che stiamo nel dominio locale (stesso host), e usiamo il tipo TCP ovvero stream socket
	
	// definiamo le generalita della socket del server
	address.sun_family=AF_UNIX;
	strcpy(address.sun_path, "server_socket");
	len=sizeof(address);

	// connettiamo la nostra socket a quella del server
	result = connect(socket_fd, (struct sockaddr*)&address, len);
	if(result==-1){ printf("errore conessione rifiutata\n"); exit(-1); }
	
	// leggiamo e scriviamo dal server via socket_fd
	write(socket_fd, &ch, 1);
	read(socket_fd, &ch, 1);
	printf("char dal server= %c\n", ch);
	close(socket_fd);
	exit(0);
}
