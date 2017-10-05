#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>

int main(int argc, char *argv[]) {
	char address[256];
	char line[128];
	char portStr[16];
	char *token;
	FILE *file;
	int argLen = 513;
	int sock;
	int len;
	int port;
	struct addrinfo hints;
	struct addrinfo *info;
	struct sockaddr_in dest;
	clock_t t;

	if(argc < 2 || argc > 3) {
		printf("Format for client connection is ./client [[IP ADDRESS]]:[[PORT NUMBER]] [[OPTIONAL CLIENT BUFFER SIZE]].\n");
		exit(0);
	}

	if(argc == 3) {
		argLen = atoi(argv[2]);
	}

	char buffer[argLen + 1];

	token = strtok(argv[1], ":");
	if(!token) {
		printf("Problem tokenizing the argument.\n");
		exit(0);
	}

	strcpy(address, token);

	token = strtok(NULL, ":");
	if(!token) {
		printf("Argument must be delimited by a colon (:).\n");
		exit(0);
	}

	port = atoi(token);
	if(!port) {
		printf("Port must be a number.\n");
		exit(0);
	}

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	dest.sin_port = htons(port);

	if(strcmp(address, "127.0.0.1") != 0) {
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;

		sprintf(portStr, "%d", port);
		if(getaddrinfo(address, portStr, &hints, &info)) {
			printf("Problem resolving address.\n");
			exit(0);
		}

		struct sockaddr_in *temp = (struct sockaddr_in*)info->ai_addr;
		inet_pton(AF_INET, inet_ntoa(temp->sin_addr), &(dest.sin_addr));
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);

	file = fopen("file.txt", "r");
	if(!file) {
		printf("Error opening file.txt. There must be a file.txt available to read.\n");
		exit(0);
	}

	connect(sock, (struct sockaddr *)&dest, sizeof(struct sockaddr_in));
	t = clock();

	while(fgets(line, argLen, file)) {
		send(sock, line, strlen(line), 0);
	}

	len = recv(sock, buffer, 513, 0);

	t = clock() - t;
	printf("This transfer took %f ms to execute\n", (((double)t) / CLOCKS_PER_SEC) * 1000);

	buffer[len] = '\0';
	printf("%s (%d bytes)\n", buffer, len);

	close(sock);
	return 0;
}
