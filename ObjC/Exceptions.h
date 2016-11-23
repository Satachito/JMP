#pragma once
//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

//	OSX		:throws non-zero integer
//	.*IX	:throws const char* (maybe volatile)
//	nullptr	:throws nullptr
//	ASSERT	:call assert

//	NSError*	:	definde in Cocoa.h

#include	<errno.h>
#include	<string.h>
#include	<assert.h>

#ifdef __OBJC__
#import		<Foundation/Foundation.h>
#else
#include	<iostream>
#endif

namespace
JpMooParaiso
{	inline	const char*
	OSTypeString( int p )
	{	static	char v[ 5 ];
		v[ 0 ] = p >> 24;
		v[ 1 ] = p >> 16;
		v[ 2 ] = p >>  8;
		v[ 3 ] = p >>  0;
		v[ 4 ] = 0;
		return v;
	}
#ifdef __OBJC__
	inline	void
	ErrorLog( const char* pFile, int pLine, const char* pDomain, int64_t p = 0, const char* pSupp = "" )
	{	NSLog( @"%s:%d\n%s:%lld(%s)\n", pFile, pLine, pDomain, p, pSupp );
	}
#else
	inline	void
	ErrorLog( const char* pFile, int pLine, const char* pDomain, int64_t p = 0, const char* pSupp = "" )
	{	fprintf( stderr, "%s:%d\n%s:%lld(%s)\n", pFile, pLine, pDomain, p, pSupp );
	}
#endif
	template <typename T>	void
	X( T p, const char* pFile, int pLine )		//	OSX
	{	if ( p ) 
		{	ErrorLog( pFile, pLine, "OSX", p, OSTypeString( (int)p ) );
			throw p;
		}
	}

	template <typename T>	T
	U( T p, const char* pFile, int pLine )		//	.*IX
	{	if ( p == -1 )
		{	const char* e = ::strerror( errno );
			ErrorLog( pFile, pLine, ".*IX", errno, e );
			throw e;
		}	
		return p;
	}

	template <typename T>	T*
	N( T* p, const char* pFile, int pLine )		//	nullptr
	{	if ( ! p ) 
		{	ErrorLog( pFile, pLine, "nullptr" );
			throw nullptr;
		}	
		return p;
	}

	template <typename T>	void
	A( T p, const char* pFile, int pLine )		//	ASSERT
	{	if ( ! p ) 
		{	ErrorLog( pFile, pLine, "ASSERT" );
			assert( false );
		}	
	}
}

#define X( p ) JpMooParaiso::X( p, __FILE__, __LINE__ )
#define U( p ) JpMooParaiso::U( p, __FILE__, __LINE__ )
#define N( p ) JpMooParaiso::N( p, __FILE__, __LINE__ )
#ifdef	DEBUG
#define A( p ) JpMooParaiso::A( p, __FILE__, __LINE__ )
#else
#define A( p ) p
#endif


