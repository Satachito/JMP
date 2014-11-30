#pragma once
//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

#include	"Apple.h"

#import		<Accelerate/Accelerate.h>

namespace
JpMooParaiso
{	namespace
	Apple
	{
		inline	CGPoint	operator
		+( CGPoint pL, CGPoint pR )
		{	return CGPointMake( pL.x + pR.x, pL.y + pR.y );
		}
		inline	CGPoint	operator
		-( CGPoint pL, CGPoint pR )
		{	return CGPointMake( pL.x - pR.x, pL.y - pR.y );
		}

		inline	CGSize	operator
		+( CGSize pL, CGSize pR )
		{	return CGSizeMake( pL.width + pR.width, pL.height + pR.height );
		}
		inline	CGSize	operator
		-( CGSize pL, CGSize pR )
		{	return CGSizeMake( pL.width - pR.width, pL.height - pR.height );
		}

		struct
		Image : Type<CGImageRef>
		{	Image( CGImageRef p ) : Type( p ) {}
			Image( CGContextRef p ) : Type( CGBitmapContextCreateImage( p ) ) {}
		};
		struct
		ColorSpace : Type<CGColorSpaceRef>
		{	ColorSpace( CGColorSpaceRef p ) : Type( p ) {}
		};
		struct
		DeviceRGBColorSpace : ColorSpace
		{	DeviceRGBColorSpace() : ColorSpace( CGColorSpaceCreateDeviceRGB() ) {}
		};
		struct
		DeviceGrayColorSpace : ColorSpace
		{	DeviceGrayColorSpace() : ColorSpace( CGColorSpaceCreateDeviceGray() ) {}
		};
		struct
		BitmapContext : Type<CGContextRef>
		{	BitmapContext( CGContextRef p ) : Type( p ) {}
			BitmapContext( CGImageRef p )
			:	Type
			(	CGBitmapContextCreate
				(	(UInt8*)(const UInt8*)Apple::Data( CGImageGetDataProvider( p ) )
				,	CGImageGetWidth( p )
				,	CGImageGetHeight( p )
				,	CGImageGetBitsPerComponent( p )
				,	CGImageGetBytesPerRow( p )
				,	CGImageGetColorSpace( p )
				,	CGImageGetBitmapInfo( p )
				)
			)
			{
			}
			BitmapContext
			(	size_t			pWidth
			,	size_t			pHeight
			,	size_t			pBitsPerComponents
			,	size_t			pBytesPerRow
			,	CGColorSpaceRef	pCSR
			,	CGBitmapInfo	pBI
			,	void*			pData = nullptr
			)
			:	Type( CGBitmapContextCreate( pData, pWidth, pHeight, pBitsPerComponents, pBytesPerRow, pCSR, pBI ) )
			{
//NSLog( @"%zd(%zd), %zd-%lx", Width(), BytesPerRow(), Height(), (long)Data() );
//for ( int y = 0; y < Height(); y++ )
//{	for ( int x = 0; x < Width(); x++ )
//	{	int* w = (int*)Data() + y * BytesPerRow() / 4 + x;
//		*w = 0xffff0000;	//	ABGR
//	}
//}
			}
			void*			Data()				const { return CGBitmapContextGetData( u ); }
			size_t			Width()				const { return CGBitmapContextGetWidth( u ); }
			size_t			Height()			const { return CGBitmapContextGetHeight( u ); }
			size_t			BitsPerComponent()	const { return CGBitmapContextGetBitsPerComponent( u ); }
			size_t			BytesPerRow()		const { return CGBitmapContextGetBytesPerRow( u ); }
			CGColorSpaceRef	ColorSpace()		const { return CGBitmapContextGetColorSpace( u ); }
			CGBitmapInfo	BitmapInfo()		const { return CGBitmapContextGetBitmapInfo( u ); }
			void
			Clear()
			{	int wZero = 0;
				vDSP_vfilli( &wZero, (int*)Data(), 1, BytesPerRow() / 4 * Height() );
			}
			int32_t
			Pixel( int pX, int pY )
			{	return ( 0 <= pX && pX < Width() && 0 <= pY && pY < Height() )
				?	((int*)Data())[ pY * BytesPerRow() / 4 + pX ]
				:	0
				;
			}
			void
			Pixel( int pX, int pY, int32_t pColor )
			{	if ( 0 <= pX && pX < Width() && 0 <= pY && pY < Height() )
				{	((int*)Data())[ BytesPerRow() / 4 * pY + pX ] = pColor;
				}
			}
			void
			HLine( int pY, int pSX, int pEX, int32_t pColor )
			{	if ( pY < 0 || Height() <= pY ) return;
				int	wWidth = (int)Width();
				if ( pSX < 0 ) pSX = 0;
				else if ( wWidth < pSX ) pSX = wWidth;
				if ( pEX < 0 ) pEX = 0;
				else if ( wWidth < pEX ) pEX = wWidth;
				if		( pSX < pEX )	vDSP_vfilli( &pColor, (int32_t*)Data() + BytesPerRow() / 4 * pY + pSX, 1, pEX - pSX );
				else if	( pSX > pEX )	vDSP_vfilli( &pColor, (int32_t*)Data() + BytesPerRow() / 4 * pY + pEX, 1, pSX - pEX );
			}
			void
			VLine( int pX, int pSY, int pEY, int32_t pColor )
			{	if ( pX < 0 || Width() <= pX ) return;
				int	wHeight = (int)Height();
				if ( pSY < 0 ) pSY = 0;
				else if ( wHeight < pSY ) pSY = wHeight;
				if ( pEY < 0 ) pEY = 0;
				else if ( wHeight < pEY ) pEY = wHeight;
				int	wBytesPerRowDiv4 = (int)BytesPerRow() / 4;
				if		( pSY < pEY )	vDSP_vfilli( &pColor, (int32_t*)Data() + wBytesPerRowDiv4 * pSY + pX, wBytesPerRowDiv4, pEY - pSY );
				else if	( pSY > pEY )	vDSP_vfilli( &pColor, (int32_t*)Data() + wBytesPerRowDiv4 * pEY + pX, wBytesPerRowDiv4, pSY - pEY );
			}
			void
			ShiftLeft()
			{	int	wBytesPerRowDiv4 = (int)BytesPerRow() / 4;
				int	wNumInts = wBytesPerRowDiv4 * (int)Height();
				int* w = (int*)Data();
				memmove( w, w + 1, ( wNumInts - 1 ) * 4 );
				int wZero = 0;
				vDSP_vfilli( &wZero, w + wBytesPerRowDiv4 - 1, wBytesPerRowDiv4, Height() );
			}
			void
			ShiftRight()
			{	int	wBytesPerRowDiv4 = (int)BytesPerRow() / 4;
				int	wNumInts = wBytesPerRowDiv4 * (int)Height();
				int* w = (int*)Data();
				memmove( w + 1, w, ( wNumInts - 1 ) * 4 );
				int wZero = 0;
				vDSP_vfilli( &wZero, w, wBytesPerRowDiv4, Height() );
			}
			void
			ShiftDown()
			{	int	wBytesPerRowDiv4 = (int)BytesPerRow() / 4;
				int	wNumInts = wBytesPerRowDiv4 * (int)Height();
				int* w = (int*)Data();
				memmove( w, w + 1, ( wNumInts - wBytesPerRowDiv4 ) * 4 );
				int wZero = 0;
				vDSP_vfilli( &wZero, w + wNumInts - wBytesPerRowDiv4, 1, wBytesPerRowDiv4 );
			}
			void
			ShiftUp()
			{	int	wBytesPerRowDiv4 = (int)BytesPerRow() / 4;
				int	wNumInts = wBytesPerRowDiv4 * (int)Height();
				int* w = (int*)Data();
				memmove( w + 1, w, ( wNumInts - wBytesPerRowDiv4 ) * 4 );
				int wZero = 0;
				vDSP_vfilli( &wZero, w, 1, wBytesPerRowDiv4 );
			}
		};
		struct
		GrayBitmapContext : BitmapContext	//	Normalized to Planar_8
		{	GrayBitmapContext( size_t pW, size_t pH, void* pData = nullptr )
			:	BitmapContext( pW, pH, 8, 0, DeviceGrayColorSpace(), kCGImageAlphaNone, pData )
			{
			}
			GrayBitmapContext( CGImageRef p )
			:	GrayBitmapContext( CGImageGetWidth( p ), CGImageGetHeight( p ) )
			{	CGContextDrawImage( (CGContextRef)u, CGRectMake( 0, 0, Width(), Height() ), p );
			}
		};
		struct
		RGBBitmapContext : BitmapContext	//	Normalized to ARGB8888
		{	RGBBitmapContext( size_t pW, size_t pH, void* pData = nullptr )
			:	BitmapContext( pW, pH, 8, 0, DeviceRGBColorSpace(), kCGBitmapByteOrder32Little | kCGImageAlphaPremultipliedFirst, pData )
			{
			}
			RGBBitmapContext( CGImageRef p )
			:	RGBBitmapContext( CGImageGetWidth( p ), CGImageGetHeight( p ) )
			{	CGContextDrawImage( (CGContextRef)u, CGRectMake( 0, 0, Width(), Height() ), p );
			}
		};
		inline	GrayBitmapContext
		BWBitmapContext( CGImageRef p )
		{	GrayBitmapContext v( p );
			auto	w = (UInt8*)v.Data();
			auto	wBytesPerRow = v.Height() * v.BytesPerRow();
			for ( auto i = 0; i < wBytesPerRow; i++ ) w[ i ] = w[ i ] < 127 ? 0 : 255;
			return v;
		}
		inline	RGBBitmapContext
		BlurredBitmapContext( RGBBitmapContext const& p, uint32_t pKernelH, uint32_t pKernelW )
		{	vImage_Buffer wI;
			wI.height	= p.Height();
			wI.width	= p.Width();
			wI.rowBytes	= p.BytesPerRow();
			wI.data		= p.Data();

			RGBBitmapContext v( wI.width, wI.height );
			vImage_Buffer wO = wI;
			wO.data = v.Data();

			X( vImageBoxConvolve_ARGB8888( &wI, &wO, nullptr, 0, 0, pKernelH, pKernelW, nullptr, kvImageEdgeExtend ) );
			return v;
		}

		inline	RGBBitmapContext
		SaturatedBitmapContext( RGBBitmapContext const& p, double pDelta, int pDivisor = 256 )
		{	vImage_Buffer wI;
			wI.height	= p.Height();
			wI.width	= p.Width();
			wI.rowBytes	= p.BytesPerRow();
			wI.data		= p.Data();

			RGBBitmapContext v( wI.width, wI.height );
			vImage_Buffer wO = wI;
			wO.data = v.Data();

			int16_t w[] =
			{	(int16_t)round( ( 0.0722 + 0.9278 * pDelta ) * pDivisor )
			,	(int16_t)round( ( 0.0722 - 0.0722 * pDelta ) * pDivisor )
			,	(int16_t)round( ( 0.0722 - 0.0722 * pDelta ) * pDivisor )
			,	0
			,	(int16_t)round( ( 0.7152 - 0.7152 * pDelta ) * pDivisor )
			,	(int16_t)round( ( 0.7152 + 0.2848 * pDelta ) * pDivisor )
			,	(int16_t)round( ( 0.7152 - 0.7152 * pDelta ) * pDivisor )
			,	0
			,	(int16_t)round( ( 0.2126 - 0.2126 * pDelta ) * pDivisor )
			,	(int16_t)round( ( 0.2126 - 0.2126 * pDelta ) * pDivisor )
			,	(int16_t)round( ( 0.2126 + 0.7873 * pDelta ) * pDivisor )
			,	0
			,	0
			,	0
			,	0
			,	int16_t( 1 * pDivisor )
			};
			X( vImageMatrixMultiply_ARGB8888( &wI, &wO, w, pDivisor, nullptr, nullptr, kvImageNoFlags ) );
 			return v;
		}
	}
}

