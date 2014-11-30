#pragma once
//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

#include	"Apple.h"
#include	<CFNetwork/CFNetwork.h>

#include	<sys/socket.h>
#include	<netinet/in.h>

namespace
JpMooParaiso
{	namespace
	Apple
	{
		struct
		Socket : Type<CFSocketRef>
		{
			Socket( CFSocketCallBack pSCB, CFSocketContext const& p )
			:	Type
			(	CFSocketCreate
				(	kCFAllocatorDefault
				,	PF_INET
				,	SOCK_STREAM
				,	IPPROTO_TCP
				,	kCFSocketAcceptCallBack
				,	pSCB
				,	&p
				)
			)
			{
			}
		};

		static	void
		IterateHeader( const void* pKey, const void* pValue, void* pMessageRef )
		{	CFHTTPMessageSetHeaderFieldValue
			(	(CFHTTPMessageRef)pMessageRef
			,	(CFStringRef)pKey
			,	(CFStringRef)pValue
			);
		}

		struct
		HTTPMessage : Type<CFHTTPMessageRef>
		{	HTTPMessage()
			:	Type( CFHTTPMessageCreateEmpty( nullptr, TRUE ) )		//	TRUE value for historical type Boolean
			{
			}

			HTTPMessage
			(	CFStringRef		pRequestMethod
			,	CFURLRef		pURL
			,	CFDictionaryRef	pHTTPHeader = NULL
			,	CFDataRef		pHTTPBody = NULL
			,	CFStringRef 	pHTTPVersion = kCFHTTPVersion1_1
			)
			: Type
			(	CFHTTPMessageCreateRequest
				(	nullptr
				,	pRequestMethod
				,	pURL
				,	pHTTPVersion
				)
			)
			{	if ( pHTTPHeader )	CFDictionaryApplyFunction( pHTTPHeader, IterateHeader, (void*)u );
				if ( pHTTPBody )	CFHTTPMessageSetBody( (CFHTTPMessageRef)u, pHTTPBody );
			}
			CFDataRef
			Data()
			{	return Apple::Data( CFHTTPMessageCopySerializedMessage( (CFHTTPMessageRef)u ), false );
			}
		};

		struct
		HTTPMessageResponse : Type<CFHTTPMessageRef>
		{	HTTPMessageResponse()
			:	Type( CFHTTPMessageCreateEmpty( nullptr, FALSE ) )	//	FALSE value for historical type Boolean
			{
			}
			HTTPMessageResponse
			(	CFIndex			pStatusCode
			,	CFStringRef		pStatusDescription
			,	CFDictionaryRef	pHTTPHeader = NULL
			,	CFDataRef		pHTTPBody = NULL
			,	CFStringRef		pHTTPVersion = kCFHTTPVersion1_1
			)
			:	Type( CFHTTPMessageCreateResponse( nullptr, pStatusCode, pStatusDescription, pHTTPVersion ) )
			{	if ( pHTTPHeader )	CFDictionaryApplyFunction( pHTTPHeader, IterateHeader, (void*)u );
				if ( pHTTPBody )	CFHTTPMessageSetBody( (CFHTTPMessageRef)u, pHTTPBody );
			}
		};

		struct
		SocketListener
		{	void (^uAcceptHandler)( CFSocketNativeHandle const&, sockaddr_in const& );

			static	void
			ConnectionCB
			(	CFSocketRef				pSocket
			,	CFSocketCallBackType	pType
			,	CFDataRef				pAddress
			,	const void*				p
			,	void*					pContext
			)
			{	assert( pType == kCFSocketAcceptCallBack );
				((SocketListener*)pContext)->uAcceptHandler( *(CFSocketNativeHandle*)p, *(sockaddr_in*)CFDataGetBytePtr( pAddress ) );
			}

			SocketListener( int pPort, void (^pAcceptHandler)( CFSocketNativeHandle const&, sockaddr_in const& ) )
			:	uAcceptHandler( pAcceptHandler )
			{
				CFSocketContext wSC = { 0, this, NULL, NULL, NULL };

				CFSocketRef	wSocket = CFSocketCreate
				(	kCFAllocatorDefault
				,	PF_INET
				,	SOCK_STREAM
				,	IPPROTO_TCP
				,	kCFSocketAcceptCallBack
				,	ConnectionCB
				,	&wSC
				);
				int wTrue = 1;
				U( setsockopt( CFSocketGetNative( wSocket ), SOL_SOCKET, SO_REUSEADDR, (const void*)&wTrue, sizeof( wTrue ) ) );

				struct sockaddr_in wServer;
				memset( &wServer, 0, sizeof( wServer ) );
				wServer.sin_len = sizeof( wServer );
				wServer.sin_family = AF_INET;
				wServer.sin_port = htons( pPort );
				wServer.sin_addr.s_addr= INADDR_ANY;
			 
				X( CFSocketSetAddress( wSocket, (__bridge CFDataRef)[ NSData dataWithBytes:&wServer length:sizeof( wServer ) ] ) );

				CFRunLoopSourceRef w = CFSocketCreateRunLoopSource( kCFAllocatorDefault, wSocket, 0 );
				CFRelease( wSocket );
				CFRunLoopAddSource( CFRunLoopGetCurrent(), w, kCFRunLoopDefaultMode );
				CFRelease( w );
			}
		};
	}
}

