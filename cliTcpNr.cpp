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
#include <iostream>

using namespace std;

int port;
string readString(int);
int RSWD(string&,int);
int WSWD(string,int);
int RBBB(string&);
int main(int argc, char *argv[])
{
	int sd;
	struct sockaddr_in server;
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

	if (connect(sd, (struct sockaddr *) &server, sizeof(struct sockaddr)) == -1)
	{
		perror("[client]Eroare la connect().\n");
		return errno;
	}

	while (1) {
		string command = "", response = "";
		printf("[client] Introduceti comanda: ");
		fflush(stdout);
		if (RBBB(command) == -1) {
			perror("[client] Eroare la citirea comenzii: ");
			break;
		}
		if (WSWD(command, sd) == -1) {
			perror("[client] Eroare la trimiterea comenzii spre server");
			break;
		}
		if (RSWD(response, sd) == -1) {
			perror("[client] Eroare la citirea raspunsului la comanda: ");
			break;
		}
		cout << response << '\n';
	}
	close(sd);
}

int RSWD(string &command, int client) {
	int nr = 0;
	char c;
	do {
		if (read(client, &c, 1) == -1) {
			return -1;
		}
		if (!isdigit(c)) break;
		nr = nr * 10 + c - '0';
	} while (1);
	char* buf = new char[nr + 5];
	if (read(client, buf, nr) == -1) {
		return -1;
	}
	buf[nr] = '\0';
	command = buf;
	return 1;
}

int WSWD(string s, int client) {
	string r = to_string(s.size()) + " " + s;
	if (write(client, r.c_str(), r.size()) == -1) {
		return -1;
	}
	return 1;
}

int RBBB(string &command) {
	char c;
	do {
		c = cin.get();
		if (c == '\n') return 1;
		command += c;
	} while (1);
}