#pragma once
//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

#include	"Exceptions.h"

#include	<CoreFoundation/CoreFoundation.h>

namespace
JpMooParaiso
{	namespace
	Apple
	{	template	<typename T>	struct	//	T must be an equivalent of CFTypeRef
		Type
		{	T	u;
			virtual
		~	Type()
			{	CFRelease( u );
			}
			Type( T p, bool pNeedToRetain = false )
			:	u( N( p ) )
			{	if ( pNeedToRetain ) CFRetain( p );
			}
			Type( Type const& p )
			:	u( p.u )
			{	CFRetain( u );
			}
			Type( Type const&& p )
			:	u( p.u )
			{
			}
			Type const& operator
			= ( Type const& p )
			{	CFRelease( u );
				u = p.u;
				CFRetain( u );
				return p;
			}
			Type const& operator
			= ( Type const&& p )
			{	u = p.u;
				return p;
			}
			operator
			T() const
			{	return u;
			}
		};

		//	Data( Data const& p ) -> Shallow copy
		//	Data( CFDataRef p, bool ) -> Deep copy
		struct
		Data : Type<CFDataRef>
		{	bool uMutable;
			Data( CFIndex p )
			:	Type( CFDataCreateMutable( nullptr, p ) )
			,	uMutable( true )
			{	CFDataSetLength( (CFMutableDataRef)u, p );
			}
			Data( CFDataRef p )
			:	Type( p, true )
			,	uMutable( false )
			{
			}
			Data( CFMutableDataRef p )
			:	Type( p, true )
			,	uMutable( true )
			{
			}
			Data( CFDataRef p, bool pMutable )
			:	Type( pMutable ? CFDataCreateMutableCopy( nullptr, 0, p ) : CFDataCreateCopy( nullptr, p ) )
			,	uMutable( pMutable )
			{
			}
			Data( CGDataProviderRef p )
			:	Type( CGDataProviderCopyData( p ) )
			,	uMutable( false )
			{
			}
			CFIndex
			Length()
			{	return CFDataGetLength( (CFDataRef)u );
			}
			operator
			CFMutableDataRef() const
			{	assert( uMutable );
				return (CFMutableDataRef)u;
			}
			operator
			const UInt8*() const
			{	return CFDataGetBytePtr( (CFDataRef)u );
			}
			operator
			UInt8*() const
			{	assert( uMutable );
				return CFDataGetMutableBytePtr( (CFMutableDataRef)u );
			}
		};
		inline	void
		Dispatch( void (^p)() )
		{	dispatch_async( dispatch_get_main_queue(), p );
		}
		inline	void
		DispatchAfter( void (^p)(), int64_t pAfter )
		{	dispatch_after( dispatch_time( DISPATCH_TIME_NOW, pAfter * NSEC_PER_SEC ), dispatch_get_main_queue(), p );
		}
	}
}

