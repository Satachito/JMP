//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

#include	<JpMooParaiso/AppleGraphics.h>

@interface
#if	TARGET_OS_IPHONE
BitmapV : UIView
#else
BitmapV : NSView
#endif

-	(JpMooParaiso::Apple::BitmapContext&)
BitmapContext
;
@end

@interface
BitmapV(Oscilloscope)

-	(void)
Oscilloscope:(float*)p
NumFrames:(size_t)pNumFrames
;
@end
