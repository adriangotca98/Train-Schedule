#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "Classes/CommandParser.h"
#include "Classes/LoginCommand.h"
#include "Classes/DisplayDayCommand.h"
#include "Classes/LogoutCommand.h"
#include "Classes/GetCommand.h"
#include "Classes/UpdateCommand.h"

#define PORT 2908

using namespace std;
using namespace rapidxml;

typedef struct ThreadData
{
	int idThread, client;
} thData;
xml_document<> *users, *trains, *now;
struct sockaddr_in server, from;
int serverDescriptor, so_reuseaddr = 1, i;
pthread_t threadList[100];
string readString(int);
static void *treat(void *);
void raspunde(void *);
int RSWD(string &, int);
int WSWD(string, int);
void init();
int main()
{
	init();
	if ((serverDescriptor = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Eroare la socket().\n");
		return errno;
	}
	setsockopt(serverDescriptor, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr, sizeof(so_reuseaddr));
	bzero(&server, sizeof(server));
	bzero(&from, sizeof(from));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(PORT);
	if (bind(serverDescriptor, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
	{
		perror("Eroare la bind().\n");
		return errno;
	}
	if (listen(serverDescriptor, 2) == -1)
	{
		perror("Eroare la listen().\n");
		return errno;
	}
	while (1)
	{
		int clientDescriptor;
		ThreadData *threadData;
		int length = sizeof(from);
		printf("Asteptam la portul %d...\n", PORT);
		fflush(stdout);
		clientDescriptor = accept(serverDescriptor, (struct sockaddr *)&from, (socklen_t *)&length);
		if (clientDescriptor < 0)
		{
			perror("Eroare la accept().\n");
			continue;
		}
		threadData = new ThreadData();
		threadData->idThread = i++;
		threadData->client = clientDescriptor;
		pthread_create(&threadList[i], NULL, &treat, threadData);
	}
}

static void *treat(void *arg)
{
	printf("Asteptam comenzi...\n");
	fflush(stdout);
	pthread_detach(pthread_self());
	raspunde((struct ThreadData *)arg);
	close((intptr_t)arg);
	return (NULL);
}

void raspunde(void *arg)
{
	loggedUser user;
	user.entityRepresented=user.typeOfUser=user.username="";
	string request = "", response = "";
	struct ThreadData threadData;
	threadData = *((struct ThreadData *)arg);
	while (1)
	{
		request = "";
		if (RSWD(request, threadData.client) == -1)
		{
			perror("Eroare la citirea comenzii de la client: ");
			break;
		}
		if (request == "")
			break;
		string response = CommandParser::parse(request);
		if (response.substr(0, 2) == "OK")
		{		
			Command *command;
			if (response.substr(3) == "login")
			{
				command = new LoginCommand(users,trains,now,&user);
				command->parse(request);
				response=command->execute();
				if (user.typeOfUser=="Station" && response=="Login successful!"){
					command = new DisplayDayCommand(trains, &user);
					string commandForDDC = "dd " + user.entityRepresented;
					command->parse(commandForDDC);
					response+="\n"+command->execute();
				}
			}
			else if (response.substr(3) == "logout"){
				command = new LogoutCommand(&user);
				command->parse(request);
				response = command->execute();
			}
			else if (response.substr(3) == "get") {
				command = new GetCommand(trains,now,&user);
				command->parse(request);
				response = command->execute();
			}
			else if (response.substr(3) == "update"){
				command = new UpdateCommand(trains,now,users,&user);
				command->parse(request);
				response = command->execute();
				ofstream f1("users.xml");
				f1<<*users;
				ofstream f2("now.xml");
				f2<<*now;
				f1.close();
				f2.close();
			}
		}
		fflush(stdout);
		if (WSWD(response, threadData.client) == -1)
		{
			perror("Eroare la trimitere spre client");
			break;
		}
		printf("message sent!\n");
	}
}

int RSWD(string &command, int client)
{
	int nr = 0;
	char c;
	do
	{
		if (read(client, &c, 1) == -1)
			return -1;
		if (!isdigit(c))
			break;
		nr = nr * 10 + c - '0';
	} while (1);
	char *buf = new char[nr + 5];
	if (read(client, buf, nr) == -1)
		return -1;
	buf[nr] = '\0';
	command += buf;
	return 1;
}

int WSWD(string s, int client)
{
	string r = to_string(s.size()) + " " + s;
	if (write(client, r.c_str(), r.size()) == -1)
		return -1;
	return 1;
}

void init()
{
	file<> usersFile("users.xml");
	file<> trainsFile("trains.xml");
	file<> nowFile("now.xml");
	users = new xml_document<>();
	trains = new xml_document<>();
	now = new xml_document<>();
	now->parse<0>(now->allocate_string(nowFile.data()));
	users->parse<0>(users->allocate_string(usersFile.data()));
	trains->parse<0>(trains->allocate_string(trainsFile.data()));
}