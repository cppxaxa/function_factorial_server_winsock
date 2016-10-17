#include <io.h>
#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

typedef long long ll;

ll factorial(ll n){
	if (n == 0)
		return 1;
	return n*factorial(n - 1);
}

int main(int argc, char *argv[])
{
	WSADATA wsa;
	SOCKET s, new_socket;
	struct sockaddr_in server, client;
	int c;
	char *message;

	printf("Initializing Winsock\t");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("YES\n");

	//Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8888);

	//Bind
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	puts("Bind\t\tYES");

	//Listen to incoming connections
	listen(s, 3);

	//Accept and incoming connection
	puts("Waiting for incoming connections\n");

	c = sizeof(struct sockaddr_in);

	while ((new_socket = accept(s, (struct sockaddr *)&client, &c)) != INVALID_SOCKET)
	{
		puts("Connection accepted");
		char buf[100];
		memset(buf, 0, 100);
		//Reply to the client
		message = "Connected\n\r";
		send(new_socket, message, strlen(message), 0);

		char data[100];
		char *p = data;
		memset(data, 0, 100);

		int ret = 1;
		int showFactorial = 0;
		while (ret > 0){
			memset(buf, 0, 100);
			ret = recv(new_socket, buf, 100, 0);
			
			for (int i = 0; i < strlen(buf); i++){
				if (buf[i] == '='){
					showFactorial = 1;
					break;
				}
				*p = buf[i];
				p++;
			}
			printf("%s\n", data);

			if (showFactorial){
				p = data;
				ll num = atoll(data);
				ll fact = factorial(num);

				printf("Factorial of %lld : %lld\n", num, fact);

				message = (char*)malloc(100);
				memset(message, 0, 100);
				sprintf(message, "%lld\n\r", fact);
				send(new_socket, message, strlen(message), 0);
				free(message);

				// END
				memset(data, 0, 100);
				showFactorial = 0;
			}
		}

		closesocket(new_socket);
	}

	if (new_socket == INVALID_SOCKET)
	{
		printf("accept failed with error code : %d", WSAGetLastError());
		return 1;
	}

	closesocket(s);
	WSACleanup();

	return 0;
}