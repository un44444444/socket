#ifndef WIN32
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define  closesocket(a)  close(a)
#include <signal.h>
#include <netinet/tcp.h>//TCP_NODELAY
#include <netdb.h>//gethostbyname
#include <sys/ioctl.h>//FIONBIO
#else// defined(WIN32)
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif//WIN32

#include "socket.h"

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

void socket_global_init()
{
#ifdef WIN32
	WORD version = MAKEWORD(2, 2);
	WSADATA data;
	WSAStartup(version, &data);
#else// !defined(WIN32)
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGPIPE, &sa, 0);
#endif//WIN32
}

void socket_global_clear()
{
#ifdef WIN32
	WSACleanup();
#endif//WIN32
}

socket_t socket_create_acceptor(uint16_t port, bool reuse)
{
	struct sockaddr_in address;
	socklen_t socklen;
	int result = 0;

	socket_t sock = socket(PF_INET, SOCK_STREAM, 0);
	if ( sock < 0 ) return -1;

	address.sin_family = PF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;
	socklen = sizeof(address);
	if (reuse)
		socket_setsockopt(sock, SO_REUSEADDR);

	result = bind(sock, (struct sockaddr *)(&address), socklen);
	if (result < 0) return -1;
	result = listen(sock, SOMAXCONN);
	if (result < 0) return -1;
	return sock;
}

socket_t socket_create_connector()
{
	return socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
}

const char* socket_hostname(const char* name)
{
	struct hostent *hptr = NULL;
	const char* hostname = NULL;
	hptr = gethostbyname(name);
	if (hptr == NULL)
	{
		return NULL;
	}
	hostname = hptr->h_addr_list[0];

	return hostname;
}

int socket_setnonblock(socket_t fd)
{
	/* If they have O_NONBLOCK, use the Posix way to do it */
#if defined(O_NONBLOCK)
	long flags;
	/* Fixme: O_NONBLOCK is defined but broken on SunOS 4.1.x and AIX 3.2.5. */
	if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
		flags = 0;
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else// !defined(O_NONBLOCK)
	/* Otherwise, use the old way of doing it */
	u_long flags = 1;
#ifdef WIN32
	return ioctlsocket(fd, FIONBIO, &flags);
#else// !defined(WIN32)
	return ioctl(fd, FIONBIO, &flags);
#endif//WIN32
#endif//defined(O_NONBLOCK)
}

int socket_connect(socket_t sock, const char* ip_address, uint16_t port)
{
	struct sockaddr_in addr;
	int result = 0;

	addr.sin_family = PF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip_address);

	result = connect(sock, (struct sockaddr *)(&addr), sizeof(addr));
	return result;
}

socket_t socket_accept(socket_t sock)
{
	return accept(sock, 0, 0);
}

int socket_send(socket_t sock, const char* msg, int length)
{
	return send(sock, msg, length, 0);
}

int socket_recv(socket_t sock, char* buff, int buffsize)
{
	return recv(sock, buff, buffsize, 0);
}

void socket_close(socket_t sock)
{
	shutdown(sock, 2);
	closesocket(sock);
}

bool socket_disconnected(socket_t sock)
{
	char byte;
	return recv(sock, &byte, sizeof(byte), MSG_PEEK) <= 0;
}

int socket_setsockopt(socket_t sock, int opt)
{
	int optval = 1;
	int level = SOL_SOCKET;
	if (opt == TCP_NODELAY)
		level = IPPROTO_TCP;

	return setsockopt(sock, level, opt, (char*)&optval, sizeof(optval));
}

int socket_getsockopt(socket_t sock, int opt, int* optval)
{
	socklen_t length = 0;
	int level = SOL_SOCKET;
	if( opt == TCP_NODELAY )
		level = IPPROTO_TCP;

	return getsockopt(sock, level, opt, (char*)optval, &length);
}

#ifdef __cplusplus
}
#endif//__cplusplus
