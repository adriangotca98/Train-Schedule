#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cerrno>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <netdb.h>
#include <cstring>
#include <string>

using namespace std;

int port;

int main(int argc, char *argv[])
{
	int sd;
	struct sockaddr_in server;
	int nr = 0;
	char buf[10];

	if (argc != 3)
	{
		printf("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
		return -1;
	}

	port = atoi(argv[2]);

	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Eroare la socket().\n");
		return errno;
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_port = htons(port);

	if (connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
	{
		perror("[client]Eroare la connect().\n");
		return errno;
	}

	printf("[client]Introduceti un numar: ");
	fflush(stdout);
	string message = readString(0);

	printf("[client] Am citit %s\n", message);

	if (write(sd, message.c_str(), message.size()) <= 0)
	{
		perror("[client]Eroare la write() spre server.\n");
		return errno;
	}

	if (read(sd, &nr, sizeof(int)) < 0)
	{
		perror("[client]Eroare la read() de la server.\n");
		return errno;
	}
	printf("[client]Mesajul primit este: %d\n", nr);

	close(sd);
}

string readString(int fileDescriptor){
    char chunk[1025];
    string str="";
    int dim=0;
    do {
        dim = read(fileDescriptor,chunk,1024);
        chunk[dim]='\0';
        str+=chunk;
    } while (dim>0);
    return str;
}