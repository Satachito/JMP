//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

#import			"BitmapV.h"
using namespace JpMooParaiso::Apple;

@implementation
BitmapV
{	BitmapContext*	uBC;
}

-	(void)
dealloc
{	if ( uBC ) delete uBC;
	[ self removeObserver:self forKeyPath:@"bounds" ];
}

-	(BitmapContext&)
BitmapContext
{	return *uBC;
}

-	(void)
Sync
{	BitmapContext* w = new BitmapContext
	(	self.bounds.size.width * 2
	,	self.bounds.size.height * 2
	,	8
	,	0
	,	DeviceRGBColorSpace()
	,	kCGImageAlphaPremultipliedLast
	);
	if ( uBC )
	{	CGContextDrawImage( *w, CGRectMake( 0, 0, uBC->Width(), uBC->Height() ), Image( *uBC ) );
		delete uBC;
	}
//	[ self Clear ];
	uBC = w;
}

-	(void)
observeValueForKeyPath:(NSString*)pKeyPath
ofObject:(id)pObject
change:(NSDictionary*)pChange
context:(void*)pContext
{	if ( [ pKeyPath isEqualToString:@"bounds" ] ) [ self Sync ];
}

-	(void)
Setup
{	[ self Sync ];
	[ self addObserver:self forKeyPath:@"bounds" options:0 context:NULL ];
}

-	(id)
initWithCoder:(NSCoder*)p
{	if ( self = [ super initWithCoder:p ] ) [ self Setup ];
	return self;
}

-	(id)
initWithFrame:(CGRect)p
{	if ( self = [ super initWithFrame:p ] ) [ self Setup ];
	return self;
}

-	(void)
drawRect:(CGRect)p
{	CGContextDrawImage( UIGraphicsGetCurrentContext(), self.bounds, Image( *uBC ) );
}

@end

#include		<Accelerate/Accelerate.h>


@implementation
BitmapV(Oscilloscope)

-	(void)
Oscilloscope:(float*)p
NumFrames:(size_t)pNumFrames
{	BitmapContext&	wBitmapContext = self.BitmapContext;
	int	wStart = 0;
	NSUInteger	wWidth = wBitmapContext.Width();
	if ( pNumFrames <= wWidth ) wWidth = pNumFrames;
	else
	{	while ( wStart < pNumFrames - wWidth - 1 ) if ( p[ wStart++ ] > 0 ) break;
		while ( wStart < pNumFrames - wWidth )
		{	if ( p[ wStart ] < 0 ) break;
			wStart++;
		}
		if ( pNumFrames - wStart < wWidth ) wWidth = pNumFrames - wStart;
	}
//	Scale to draw
	float wTmp1[ wWidth ];
	NSUInteger	wHalfHeight = wBitmapContext.Height() / 2;
	float wScalar = wHalfHeight;
	vDSP_vsmul( (float*)p + wStart, 1, &wScalar, wTmp1, 1, wWidth );
	float wTmp2[ wWidth ];
	vDSP_vsadd( wTmp1, 1, &wScalar, wTmp2, 1, wWidth );
//	Draw
	wBitmapContext.Clear();
	for ( int x = 0; x < wWidth; x++ ) wBitmapContext.VLine( x, (int)wHalfHeight, wTmp2[ x ] + .5, 0xff0000ff );
	[ self performSelectorOnMainThread:@selector(setNeedsDisplay) withObject:nil waitUntilDone:NO ];
}

@end



