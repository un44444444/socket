#ifndef _SOCKET_TCP_CLIENT_H_
#define _SOCKET_TCP_CLIENT_H_

#ifdef CWRAPPER_EXPORTS
#define CWRAPPER_API __declspec(dllexport)
#else
#define CWRAPPER_API __declspec(dllimport)
#endif//CWRAPPER_EXPORTS


#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

struct ClientHandle
{
	void* pClient;
	void* pFile;
};

CWRAPPER_API int InitialClient(struct ClientHandle* pClientHandle, const char* szServerIp, int nServerPort, const char* pszLogFile);
CWRAPPER_API int DestroyClient(struct ClientHandle* pClientHandle);
CWRAPPER_API int ClientSend(struct ClientHandle* pClientHandle, const char* msg, int msglen);
CWRAPPER_API int ClientRecv(struct ClientHandle* pClientHandle, char* buff, int buffsize);

#ifdef __cplusplus
};
#endif//__cplusplus


#endif//_SOCKET_TCP_CLIENT_H_
