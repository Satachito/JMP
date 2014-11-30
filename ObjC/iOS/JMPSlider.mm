//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

#import "JMPSlider.h"

@implementation
JMPSlider

-	(void)
Setup
{	[ self addTarget:self action:@selector(ValueChanged:) forControlEvents:UIControlEventValueChanged ];
}

-	(instancetype)
init
{	if ( ( self = [ super init ] ) ) [ self Setup ];
	return self;
}
-	(instancetype)
initWithFrame:(CGRect)p
{	if ( ( self = [ super initWithFrame:p ] ) ) [ self Setup ];
	return self;
}
-	(instancetype)
initWithCoder:(NSCoder*)p
{	if ( ( self = [ super initWithCoder:p ] ) ) [ self Setup ];
	return self;
}

-	(void)
ValueChanged:(id)p
{	if ( _slided ) _slided( self, self.value );
}

@end

