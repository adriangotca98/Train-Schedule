#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <cstdlib>
#include <signal.h>
#include <pthread.h>
#include <stdint.h>
#include <iostream>

#define PORT 2908

using namespace std;

typedef struct thData{
	int idThread;
	int cl;
} thData;
string readString(int);
static void *treat(void *);
void raspunde(void *);
int RSWD(string&,int);
int WSWD(string,int);
int main(){
	struct sockaddr_in server;
	struct sockaddr_in from;
	int sd;
	int pid;
	pthread_t th[100];
	int i = 0;
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("[server]Eroare la socket().\n");
		return errno;
	}
	int on = 1;
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	bzero(&server, sizeof(server));
	bzero(&from, sizeof(from));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(PORT);
	if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1) {
		perror("[server]Eroare la bind().\n");
		return errno;
	}
	if (listen(sd, 2) == -1){
		perror("[server]Eroare la listen().\n");
		return errno;
	}
	while (1){
		int client;
		thData *td;
		int length = sizeof(from);
		printf("[server]Asteptam la portul %d...\n", PORT);
		fflush(stdout);
		client = accept(sd, (struct sockaddr *)&from, (socklen_t*)&length);
		if (client < 0) {
			perror("[server]Eroare la accept().\n");
			continue;
		}
		td = (struct thData *)malloc(sizeof(struct thData));
		td->idThread = i++;
		td->cl = client;
		pthread_create(&th[i], NULL, &treat, td);
	}
}

static void *treat(void *arg) {
	struct thData tdL;
	tdL = *((struct thData *)arg);
	printf("[thread]- %d - Asteptam comenzi...\n", tdL.idThread);
	fflush(stdout);
	pthread_detach(pthread_self());
	raspunde((struct thData *)arg);
	close((intptr_t)arg);
	return (NULL);
}

void raspunde(void *arg){
	string command="";
	struct thData tdL;
	tdL = *((struct thData*)arg);
	while (1) {
		command="";
		if (RSWD(command, tdL.cl) == -1) {
			printf("[Thread %d]", tdL.idThread);
			perror("Eroare la citirea comenzii de la client: ");
			break;
		}
		if (command=="") break;
		fflush(stdout);
		printf("[Thread %d]Comanda a fost receptionata...%s\n", tdL.idThread, command.c_str());
		if (WSWD(command + " - OK", tdL.cl) == -1) {
			perror("Eroare la trimitere spre client");
			break;
		}
		printf("message sent!\n");
	}
}

int RSWD(string &command, int client) {
	int nr = 0;
	char c;
	do {
		if (read(client, &c, 1) == -1) return -1;
		if (!isdigit(c)) break;
		nr = nr * 10 + c - '0';
	} while (1);
	char *buf = new char[nr + 5];
	if (read(client, buf, nr) == -1) return -1;
	buf[nr] = '\0';
	command = buf;
	return 1;
}

int WSWD(string s, int client) {
	string r = to_string(s.size()) + " " + s;
	if (write(client, r.c_str(), r.size()) == -1) return -1;
	return 1;
}