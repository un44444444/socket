#ifndef _CUSTOM_SOCKET_H_
#define _CUSTOM_SOCKET_H_

#ifndef WIN32
#include <stdint.h>
#include <stdbool.h>
typedef int socket_t;
#else// defined(WIN32)
#include <winsock2.h>
#include <windef.h>
typedef SOCKET socket_t;
typedef int socklen_t;
typedef unsigned __int16 uint16_t;
#ifndef __cplusplus
typedef BOOL bool;
#define true TRUE
#define false FALSE
#endif//__cplusplus
#endif//WIN32

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

void socket_global_init();
void socket_global_clear();
socket_t socket_create_acceptor(uint16_t port, bool reuse);
socket_t socket_create_connector();
const char* socket_hostname(const char* name);
int socket_setnonblock(socket_t sock);
int socket_connect(socket_t sock, const char* ip_address, uint16_t port);
socket_t socket_accept(socket_t sock);
int socket_send(socket_t sock, const char* msg, int length);
int socket_recv(socket_t sock, char* buff, int buffsize);
void socket_close(socket_t sock);
bool socket_disconnected(socket_t sock);
int socket_setsockopt(socket_t sock, int opt );
int socket_getsockopt(socket_t sock, int opt, int* optval);

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//_CUSTOM_SOCKET_H_
