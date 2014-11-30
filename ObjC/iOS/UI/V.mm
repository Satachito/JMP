#import "V.h"

@implementation
V

-	(id)
initWithCoder:(NSCoder*)p
{	if ( self = [ super initWithCoder:p ] ) _drawProc = ^(CGRect const&){};
	return self;
}
-	(id)
initWithFrame:(CGRect)p
{	if ( self = [ super initWithFrame:p ] ) _drawProc = ^(CGRect const&){};
	return self;
}
-	(void)
drawRect:(CGRect)p
{	_drawProc( p );
}
@end
