#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define READ 0 /* Estremità in lettura della pipe */
#define WRITE 1 /* Estremità in scrittura della pipe */
char *msg[3] = { "Primo", "Secondo", "Terzo" };
int main (void) {
	int fd[2], i, length, bytesRead;
	char buffer [100]; /* Buffer del messaggio */
	pipe (fd); /* Crea una pipe anonima */
	if (fork () == 0) { /* Figlio, scrittore */
	close(fd[READ]); /* Chiude l’estremità inutilizzata */
		for (i = 0; i < 3; i++) {
		length=strlen (msg[i])+1 ; /* include \0 */
		write (fd[WRITE], &length, sizeof(int));
		write (fd[WRITE], msg[i], length); }
		close (fd[WRITE]); /* Chiude l’estremità usata */
	}
	else { /* Genitore, lettore */
		close (fd[WRITE]); /* Chiude l’estremità non usata */
		while (read(fd[READ], &length, sizeof(int))) {
			bytesRead = read (fd[READ], buffer, length);
		if (bytesRead != length) {
			printf("Errore!\n");
			exit(1);
		}

		printf("Padre:Letti %d byte:%s\n",bytesRead,buffer);
		}
		close (fd[READ]); /* Chiude l’estremità usata */
	}
exit(0);
}
