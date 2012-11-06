#include "socket.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argb[])
{
	socket_t socket_fd;
	char buff[512];
	bool shutdown;
	const char* msg = "Hello, this is a test message.";
	int i, result, msglen=strlen(msg);

	socket_global_init();

	socket_fd = socket_create_connector();
	printf("socket_create_connector return %d\n", socket_fd);
	result = socket_connect(socket_fd, "127.0.0.1", 5555);
	printf("socket_connect result=%d\n", result);
	result = socket_recv(socket_fd, buff, sizeof(buff));
	if (result < 0) {
		printf("socket_recv fail, return %d\n", result);
		return result;
	}
	buff[result] = '\0';
	printf("socket_recv result=%d, buff=%s\n", result, buff);

	shutdown = false;
	if(!shutdown)
	{
		i = 0;
		for (; i < 10; i++)
		{
			socket_send(socket_fd, msg, msglen);
			result = socket_recv(socket_fd, buff, sizeof(buff));
			if (result < 0) {
				printf("socket_recv fail, return %d\n", result);
				return result;
			}
			buff[result] = '\0';
			printf("socket_recv result=%d, buff=%s\n", result, buff);
		}
	}
	socket_close(socket_fd);

	socket_global_clear();
	return 0;
}
