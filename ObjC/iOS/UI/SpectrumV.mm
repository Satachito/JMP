#import			"SpectrumV.h"

#include		<JpMooParaiso/Arithmetics.h>
using namespace JpMooParaiso;
#include		<JpMooParaiso/AppleGraphics.h>
using namespace JpMooParaiso::Apple;

@implementation
SpectrumV
{	FFTSetup	uSetup;
	
	size_t		uLog2N;

	size_t		uNumFrames;
	size_t		uNumFramesPlus2;
	size_t		uHalfNumFrames;
	size_t		uHalfNumFramesPlus1;

	float		uNormFactor;
}

-	(void)
dealloc
{	if ( uSetup ) vDSP_destroy_fftsetup( uSetup );
}

-	(void)
Spectrum:(float*)p
NumFrames:(size_t)pNumFrames
{	if ( uNumFrames != pNumFrames )
	{	if ( uSetup ) vDSP_destroy_fftsetup( uSetup );

		uLog2N = Log2N( pNumFrames );
		assert( ( 1 << uLog2N ) == pNumFrames );
		uSetup  = vDSP_create_fftsetup( uLog2N, FFT_RADIX2 );

		uNumFrames = pNumFrames;
		uNumFramesPlus2 = pNumFrames + 2;
		uHalfNumFrames = pNumFrames / 2;
		uHalfNumFramesPlus1 = pNumFrames / 2 + 1;

		uNormFactor = 1. / uNumFrames;

	}
	if ( ! uSetup ) return;

	BitmapContext&	wBitmapContext = self.BitmapContext;

	float	wPlaceholder[ uNumFramesPlus2 ];

	COMPLEX_SPLIT wComplexSplit = { wPlaceholder, wPlaceholder + uHalfNumFramesPlus1 };
	vDSP_ctoz( (DSPComplex*)p, 2, &wComplexSplit, 1, uHalfNumFrames );
	vDSP_fft_zrip( uSetup, &wComplexSplit, 1, uLog2N, FFT_FORWARD );
	vDSP_vsmul( wPlaceholder, 1, &uNormFactor, wPlaceholder, 1, uNumFramesPlus2 );
	wComplexSplit.realp[ uHalfNumFrames ] = wComplexSplit.imagp[ 0 ];
	wComplexSplit.imagp[ 0 ] = 0;
	wComplexSplit.imagp[ uHalfNumFrames ] = 0;

	float wDBs[ uHalfNumFramesPlus1 ];
	static	float	sOne = 1;
//Power
	vDSP_zvmags( &wComplexSplit, 1, wDBs, 1, uHalfNumFramesPlus1 );
	vDSP_vdbcon( wDBs, 1, &sOne, wDBs, 1, uHalfNumFramesPlus1, 0 );
//Amp
//	vDSP_zvabs( &wComplexSplit, 1, wDBs, 1, uHalfNumFramesPlus1 );
//	vDSP_vdbcon( wDBs, 1, &sOne, wDBs, 1, uHalfNumFramesPlus1, 1 );

	auto	wWidth = wBitmapContext.Width();
	auto	wHeight = wBitmapContext.Height();
	if ( uHalfNumFramesPlus1 < wWidth ) wWidth = uHalfNumFramesPlus1;
//	Draw
	wBitmapContext.Clear();
	for ( int x = 0; x < wWidth; x++ ) wBitmapContext.VLine( x, 0, wDBs[ x ] + wHeight, 0xff0000ff );
	[ self performSelectorOnMainThread:@selector(setNeedsDisplay) withObject:nil waitUntilDone:NO ];

//	CHECK
#if 1

#endif
}
/*
inline	double
Dist( double pR, double pI )
{	return sqrt( pR * pR + pI * pI );
}

inline	double
Abs( double p )
{	return p < 0 ? -p : p;
}

Timer w1;
	{	BitmapContext& w = oParamV.BitmapContext;
		float wHeight = w.Height() / 2 * Log2N( NUM_SAMPLES );
		w.VLine( 0, 0, Abs( wSplitComplex.realp[ 0 ] ) * wHeight / 2, 0xff000000 );	//	Must divide by 2
		for ( int i = 1; i < NUM_SAMPLES / 2; i++ ) w.VLine( i, 0, Dist( wSplitComplex.realp[ i ], wSplitComplex.imagp[ i ] ) * wHeight, 0xff000000 );
		w.VLine( NUM_SAMPLES / 2, 0, Abs( wSplitComplex.imagp[ 0 ] ) * wHeight / 2, 0xff000000 );	//	Must divide by 2
	};
NSLog( @"%zd", w1.Nano() );
Timer w2;
	{	BitmapContext& w = oFFTV.BitmapContext;
		wSplitComplex.realp[ 0 ] /= 2;
		wSplitComplex.imagp[ 0 ] = 0;
		Array<float>	w2( NUM_SAMPLES );
		vDSP_zvabs( &wSplitComplex, 1, w2.u, 1, NUM_SAMPLES / 2 );
		float wHeight = w.Height() / 2 * Log2N( NUM_SAMPLES );
		for ( int i = 0; i < NUM_SAMPLES / 2; i++ ) w.VLine( i, 0, w2.u[ i ] * wHeight, 0xff000000 );
	};
NSLog( @"%zd", w2.Nano() );
	{	BitmapContext& w = oFFTV.BitmapContext;
		wSplitComplex.realp[ 0 ] /= 2;
		wSplitComplex.imagp[ 0 ] = 0;
		Array<float>	w2( NUM_SAMPLES );
		vDSP_zvmags( &wSplitComplex, 1, w2.u, 1, NUM_SAMPLES / 2 );
		float wOne = 1;
		vDSP_vdbcon( w2.u, 1, &wOne, w2.u, 1, NUM_SAMPLES, 0 );
		float wHeight = w.Height();
		for ( int i = 0; i < NUM_SAMPLES / 2; i++ ) w.VLine( i, 0, wHeight + w2.u[ i ], 0xff000000 );
	};




	Array<float>	v( NUM_SAMPLES );
	vDSP_fft_zrip( wSetup, &wSplitComplex, 1, Log2N( NUM_SAMPLES ), FFT_INVERSE );
	vDSP_ztoc( &wSplitComplex, 1, (DSPComplex*)v.u, 2, NUM_SAMPLES / 2 );
	wNormFactor = 1. / 2.;
	vDSP_vsmul( v.u, 1, &wNormFactor, v.u, 1, NUM_SAMPLES );

	{	BitmapContext& w = oOutputV.BitmapContext;
		int wHalfHeight = w.Height() / 2 + .5;
		for ( int i = 0; i < NUM_SAMPLES; i++ ) w.VLine( i, wHalfHeight, wHalfHeight + v.u[ i ] * wHalfHeight + .5, 0xff000000 );
	};
	for ( int i = 0; i < NUM_SAMPLES; i++ ) if ( Abs( v.u[ i ] - p.u[ i ] ) > .000001 ) NSLog( @"------------------------ %d:%f", i, v.u[ i ] - p.u[ i ] );


*/
@end
