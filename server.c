#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
	struct sockaddr_in dest;
	struct sockaddr_in serv;
	char buffer[513];
	int connectSocket;
	int len;
	int port;
	int sock;
	socklen_t socksize = sizeof(struct sockaddr_in);

	if(argc != 2) {
		printf("Format for server connection is ./server [[PORT NUMBER]].\n");
		exit(0);
	}

	port = atoi(argv[1]);
	if(!port || port < 0) {
		printf("Port must be a number and non-negative.\n");
		exit(0);
	}

	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = htonl(INADDR_ANY);
	serv.sin_port = htons(port);

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(bind(sock, (struct sockaddr*)&serv, sizeof(struct sockaddr)) == -1) {
		fprintf(stderr, "Server bind was unsuccessful.\n");
		exit(0);
	}

	listen(sock, 20);

	connectSocket = accept(sock, (struct sockaddr*)&dest, &socksize);
	while(connectSocket) {
		printf("Incoming connection from %s \n", inet_ntoa(dest.sin_addr));
		len = recv(connectSocket, buffer, 512, 0);
		printf("%s\n%d bytes\n", buffer, len);

		send(connectSocket, "File finished sending", strlen(buffer), 0); 
		close(connectSocket);
		connectSocket = accept(sock, (struct sockaddr *)&dest, &socksize);
	}

	close(sock);
	return 0;
}
