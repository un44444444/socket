#include "socket_client.h"
#include "../../socket.h"

#include <stdio.h>
#include <time.h>

//
#ifdef _MANAGED
#pragma managed(push, off)
#endif//_MANAGED

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif//_MANAGED

//#define 

CWRAPPER_API int InitialClient(struct ClientHandle* pClientHandle, const char* szServerIp, int nServerPort, const char* pszLogFile)
{
	socket_t socket_fd;
	int result = 0;
	time_t currTime;
	if (NULL == pClientHandle)
		return -1;

	// init client
	socket_global_init();
	socket_fd = socket_create_connector();
	pClientHandle->pClient = malloc(sizeof(socket_t));
	memcpy(pClientHandle->pClient, &socket_fd, sizeof(socket_t));
	// init log file
	if (NULL != pszLogFile)
	{
		pClientHandle->pFile = fopen(pszLogFile, "a+");
	}
	else
	{
		pClientHandle->pFile = stdout;
	}

	// connect to server
	result = socket_connect(*(socket_t*)(pClientHandle->pClient), szServerIp, (uint16_t)nServerPort);
	// log
	time(&currTime);
	fprintf((FILE*)(pClientHandle->pFile), "[%u] InitialClient() pClientHandle=%p, pClientHandle->pClient=%d\n", currTime, pClientHandle, *(socket_t*)pClientHandle->pClient);

	return 0;
}

CWRAPPER_API int DestroyClient(struct ClientHandle* pClientHandle)
{
	socket_t* pSocket;
	time_t currTime;
	if (NULL == pClientHandle)
		return -1;
	pSocket = (socket_t*)pClientHandle->pClient;

	// log
	if (NULL != pClientHandle->pFile)
	{
		time(&currTime);
		fprintf((FILE*)(pClientHandle->pFile), "[%u] DestroyClient() pClientHandle=%p, pClientHandle->pClient=%d\n", currTime, pClientHandle, *(socket_t*)pClientHandle->pClient);
		fclose((FILE*)(pClientHandle->pFile));
		pClientHandle->pFile = NULL;
	}

	// destroy client
	socket_close(*pSocket);
	socket_global_clear();
	free(pSocket);
	pClientHandle->pClient = NULL;

	return 0;
}

CWRAPPER_API int ClientSend(struct ClientHandle* pClientHandle, const char* msg, int msglen)
{
	socket_t* pSocket;
	time_t currTime;
	if (NULL == pClientHandle)
		return -1;
	pSocket = (socket_t*)(pClientHandle->pClient);

	// log
	if (NULL != pClientHandle->pFile)
	{
		time(&currTime);
		fprintf((FILE*)(pClientHandle->pFile), "[%u] ClientSend() pClientHandle=%p\n", currTime, pClientHandle);
	}
	// send
	return socket_send(*pSocket, msg, msglen);
}

CWRAPPER_API int ClientRecv(struct ClientHandle* pClientHandle, char* buff, int buffsize)
{
	socket_t* pSocket;
	time_t currTime;
	int result = 0;
	if (NULL == pClientHandle)
		return -1;
	pSocket = (socket_t*)(pClientHandle->pClient);

	// log
	if (NULL != pClientHandle->pFile)
	{
		time(&currTime);
		fprintf((FILE*)(pClientHandle->pFile), "[%u] ClientRecv() pClientHandle=%p\n", currTime, pClientHandle);
	}
	// recv
	result = socket_recv(*pSocket, buff, buffsize);

	return result;
}
