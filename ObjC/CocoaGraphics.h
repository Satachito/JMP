#pragma once
//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

#include	"AppleGraphics.h"

namespace
JpMooParaiso
{	namespace
	Apple
	{
#if	TARGET_OS_IPHONE
		inline	UIImage*
		Snapshot( UIView* p, BOOL pAfterScreenUpdates = NO )
		{	UIGraphicsBeginImageContextWithOptions( p.bounds.size, NO, 0 );
			[ p drawViewHierarchyInRect:p.bounds afterScreenUpdates:pAfterScreenUpdates ];
			UIImage* v = UIGraphicsGetImageFromCurrentImageContext();
			UIGraphicsEndImageContext();
			return v;
		}
		inline	UIImage*
		GrayImage( UIImage* p )
		{	return
			[	UIImage
				imageWithCGImage:Image( GrayBitmapContext( p.CGImage ) )
				scale:p.scale
				orientation:p.imageOrientation
			];
		}
		inline	UIImage*
		RGBImage( UIImage* p )
		{	return
			[	UIImage
				imageWithCGImage:Image( RGBBitmapContext( p.CGImage ) )
				scale:p.scale
				orientation:p.imageOrientation
			];
		}

		inline	UIImage*
		BWImage( UIImage* p )
		{	return
			[	UIImage
				imageWithCGImage:Image( BWBitmapContext( p.CGImage ) )
				scale:p.scale
				orientation:p.imageOrientation
			];
		}

		inline	UIImage*
		BlurredImage( UIImage* p, uint32_t pKernelH, uint32_t pKernelW )
		{	return
			[	UIImage
				imageWithCGImage:Image( BlurredBitmapContext( p.CGImage, pKernelH, pKernelW ) )
				scale:p.scale
				orientation:p.imageOrientation
			];
		}

		inline	UIImage*
		SaturatedImage( UIImage* p, double pDelta )
		{	return
			[	UIImage
				imageWithCGImage:Image( SaturatedBitmapContext( p.CGImage, pDelta ) )
				scale:p.scale
				orientation:p.imageOrientation
			];
		}
#endif
	}
}

