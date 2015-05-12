/************************************************************************

		FileName:iocp.h
		Author	:eliteYang

http://www.cppfans.org

************************************************************************/
#ifndef __IOCP_H__
#define __IOCP_H__

#include
#include 

#define DefaultPort 20000
#define DataBuffSize 8 * 1024

typedef struct
{
	OVERLAPPED overlapped;
	WSABUF databuff;
	CHAR buffer[ DataBuffSize ];
	DWORD bytesSend;
	DWORD bytesRecv;
}PER_IO_OPERATEION_DATA, *LPPER_IO_OPERATION_DATA;

typedef struct
{
	SOCKET socket;
}PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

#endif

/************************************************************************

		FileName:iocp.cpp
		Author	:eliteYang

http://www.cppfans.org

************************************************************************/

#include "iocp.h"
#include <windows.h>

using namespace std;

#pragma comment( lib, "Ws2_32.lib" ) // 需要动态链接winsock2库

DWORD WINAPI ServerWorkThread( LPVOID CompletionPortID );

void main()
{

	SOCKET acceptSocket;
	HANDLE completionPort;
	LPPER_HANDLE_DATA pHandleData;
	LPPER_IO_OPERATION_DATA pIoData;
	DWORD recvBytes;
	DWORD flags;

	WSADATA wsaData;
	DWORD ret;
	if ( ret = WSAStartup( 0x0202, &wsaData ) != 0 ) // 初始化winsock
	{
		std::cout << "WSAStartup failed. Error:" << ret << std::endl;
		return;
	}

	completionPort = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 ); // 创建一个完成端口 得到句柄 初始化过程
	if ( completionPort == NULL )
	{
		std::cout << "CreateIoCompletionPort failed. Error:" << GetLastError() << std::endl;
		return;
	}

	SYSTEM_INFO mySysInfo;
	GetSystemInfo( &mySysInfo );

	// 创建 2 * CPU核数 + 1 个线程
	DWORD threadID;
	for ( DWORD i = 0; i < ( mySysInfo.dwNumberOfProcessors * 2 + 1 ); ++i )
	{
		HANDLE threadHandle;
		threadHandle = CreateThread( NULL, 0, ServerWorkThread, completionPort, 0, &threadID );
		if ( threadHandle == NULL )
		{
			std::cout << "CreateThread failed. Error:" << GetLastError() << std::endl;
			return;
		}

		CloseHandle( threadHandle );
	}

	// 启动一个监听socket 创建一个listen fd
	SOCKET listenSocket = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
	if ( listenSocket == INVALID_SOCKET )
	{
		std::cout << " WSASocket( listenSocket ) failed. Error:" << GetLastError() << std::endl;
		return;
	}

	SOCKADDR_IN internetAddr;
	internetAddr.sin_family = AF_INET;
	internetAddr.sin_addr.s_addr = htonl( INADDR_ANY );
	internetAddr.sin_port = htons( DefaultPort );

	// 绑定监听端口
	if ( bind( listenSocket, (PSOCKADDR)&internetAddr, sizeof( internetAddr ) ) == SOCKET_ERROR )
	{
		std::cout << "Bind failed. Error:" << GetLastError() << std::endl;
		return;
	}

	if ( listen( listenSocket, 5 ) ==  SOCKET_ERROR )
	{
		std::cout << "listen failed. Error:" << GetLastError() << std::endl;
		return;
	}

	// 开始死循环，处理数据
	while ( 1 )
	{
		acceptSocket = WSAAccept( listenSocket, NULL, NULL, NULL, 0 ); // winsock accept
		if ( acceptSocket == SOCKET_ERROR )
		{
			std::cout << "WSAAccept failed. Error:" << GetLastError() << std::endl;
			return;
		}

		pHandleData = (LPPER_HANDLE_DATA)GlobalAlloc( GPTR, sizeof( PER_HANDLE_DATA ) );
		if ( pHandleData = NULL )
		{
			std::cout << "GlobalAlloc( HandleData ) failed. Error:" << GetLastError() << std::endl;
			return;
		}

		// 主线程中将一个完成端口与得到的accept socket_fd绑定 即与对等方fd绑定
		// 在工作线程中从这个完成端口中得到数据或者发送数据
		pHandleData->socket = acceptSocket;
		if ( CreateIoCompletionPort( (HANDLE)acceptSocket, completionPort, (ULONG_PTR)pHandleData, 0 ) == NULL ) 
		{
			std::cout << "CreateIoCompletionPort failed. Error:" << GetLastError() << std::endl;
			return;
		}

		pIoData = ( LPPER_IO_OPERATION_DATA )GlobalAlloc( GPTR, sizeof( PER_IO_OPERATEION_DATA ) );
		if ( pIoData == NULL )
		{
			std::cout << "GlobalAlloc( IoData ) failed. Error:" << GetLastError() << std::endl;
			return;
		}

		ZeroMemory( &( pIoData->overlapped ), sizeof( pIoData->overlapped ) );
		pIoData->bytesSend = 0;
		pIoData->bytesRecv = 0;
		pIoData->databuff.len = DataBuffSize;
		pIoData->databuff.buf = pIoData->buffer;

		flags = 0;
		if ( WSARecv( acceptSocket, &(pIoData->databuff), 1, &recvBytes, &flags, &(pIoData->overlapped), NULL ) == SOCKET_ERROR )
		{
			if ( WSAGetLastError() != ERROR_IO_PENDING )
			{
				std::cout << "WSARecv() failed. Error:" << GetLastError() << std::endl;
				return;
			}
			else
			{
				std::cout << "WSARecv() io pending" << std::endl;
				return;
			}
		}
	}
}

//  工作线程执行函数 传入完成端口fd
DWORD WINAPI ServerWorkThread( LPVOID CompletionPortID )
{
	HANDLE complationPort = (HANDLE)CompletionPortID;
	DWORD bytesTransferred;
	LPPER_HANDLE_DATA pHandleData = NULL;
	LPPER_IO_OPERATION_DATA pIoData = NULL;
	DWORD sendBytes = 0;
	DWORD recvBytes = 0;
	DWORD flags;

	while ( 1 )
	{
		// 当线程池中的一个线程调用 GetQueuedCompletionStatus 时，调用线程的 ID 就被放入等待线程队列中。
		// 这样，I/O完成端口对象总是知道哪个线程正在等待处理完成的I/O请求。当完成队列里出现一项时，
		// 完成端口就唤醒等待线程队列里的一个线程，并把所有信息通过参数传过去。
	    // I/O 完成队列里的表项是按照先进先出（FIFO）方式删除的。但是调用 GetQueuedCompletionStatus 的线程却是按照后进先出（LIFO）方式被唤醒的。
	    // 原因也是为了提高性能。比如，有4个线程等在线程队列中。如果出现了一个I/O项，最后一个调用 GetQueuedCompletionStatus  
	    // 的线程被唤醒来处理这一项。当处理完后，它再次调用 GetQueuedCompletionStatus 进入等待线程队列。这时如果出现了另一个I/O完成项，
	    // 同一线程将被唤醒来处理这一新项。只要I/O请求完成的足够慢，使得一个线程能处理它们，系统就总是唤醒同一个线程，其它三个线程将继续休眠。
	    // 通过使用LIFO算法，不被调度的线程的内存资源（如栈空间）可以被交换到磁盘上和从处理器的缓存中清除。
	    // 这意味着有多个线程等待一个完成端口也没有什么坏处。

	    // 调用GetQueuedCompletionStatus()喊这个函数进入了 休眠等待IO完成的时候 将它唤醒
	    // PostQueuedCompletionStatus()函数可以主动向IO完成队列中加入一个IO完成请求
		if ( GetQueuedCompletionStatus( complationPort, &bytesTransferred, (PULONG_PTR)&pHandleData, (LPOVERLAPPED *)&pIoData, INFINITE ) == 0 )
		{
			std::cout << "GetQueuedCompletionStatus failed. Error:" << GetLastError() << std::endl;
			return 0;
		}

		// 检查数据是否已经传输完了
		if ( bytesTransferred == 0 )
		{
			std::cout << " Start closing socket..." << std::endl;
			if ( CloseHandle( (HANDLE)pHandleData->socket ) == SOCKET_ERROR )
			{
				std::cout << "Close socket failed. Error:" << GetLastError() << std::endl;
				return 0;
			}

			GlobalFree( pHandleData );
			GlobalFree( pIoData );
			continue;
		}

		// 检查管道里是否有数据
		if ( pIoData->bytesRecv == 0 )
		{
			pIoData->bytesRecv = bytesTransferred;
			pIoData->bytesSend = 0;
		}
		else
		{
			pIoData->bytesSend += bytesTransferred;
		}

		// 数据没有发完，继续发送
		if ( pIoData->bytesRecv > pIoData->bytesSend )
		{
			ZeroMemory( &(pIoData->overlapped), sizeof( OVERLAPPED ) );
			pIoData->databuff.buf = pIoData->buffer + pIoData->bytesSend;
			pIoData->databuff.len = pIoData->bytesRecv - pIoData->bytesSend;

			// 发送数据出去
			if ( WSASend( pHandleData->socket, &(pIoData->databuff), 1, &sendBytes, 0, &(pIoData->overlapped), NULL ) == SOCKET_ERROR )
			{
				if ( WSAGetLastError() != ERROR_IO_PENDING )
				{
					std::cout << "WSASend() failed. Error:" << GetLastError() << std::endl;
					return 0;
				}
				else
				{
					std::cout << "WSASend() failed. io pending. Error:" << GetLastError() << std::endl;
					return 0;
				}
			}

			std::cout << "Send " << pIoData->buffer << std::endl;
		}
		else
		{
			pIoData->bytesRecv = 0;
			flags = 0;

			ZeroMemory( &(pIoData->overlapped), sizeof( OVERLAPPED ) );
			pIoData->databuff.len = DataBuffSize;
			pIoData->databuff.buf = pIoData->buffer;

			if ( WSARecv( pHandleData->socket, &(pIoData->databuff), 1, &recvBytes, &flags, &(pIoData->overlapped), NULL ) == SOCKET_ERROR )
			{
				if ( WSAGetLastError() != ERROR_IO_PENDING )
				{
					std::cout << "WSARecv() failed. Error:" << GetLastError() << std::endl;
					return 0;
				}
				else
				{
					std::cout << "WSARecv() io pending" << std::endl;
					return 0;
				}
			}
		}
	}
}