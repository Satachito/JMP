//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

#import "JMPFitLayoutV.h"

@implementation
JMPFitLayoutV

//	letter d, D stands for Diameter

-	(void)
SetupTargetView:(NSUInteger)p
D:(CGFloat)pD
Center:(CGPoint)pCenter
{	UIView* w = _targetViews[ p ];
	w.frame = CGRectMake( 0, 0, pD, pD );
	w.layer.cornerRadius = pD / 2;
	w.clipsToBounds = true;
	w.center = pCenter;
}

-	(void)
setTargetViews:(NSArray*)p
{	for ( UIView* wV in _targetViews ) [ wV removeFromSuperview ];
	_targetViews = p;
	for ( UIView* wV in _targetViews ) [ self addSubview:wV ];

	CGFloat	wBoundD = self.bounds.size.width < self.bounds.size.height
	?	self.bounds.size.width
	:	self.bounds.size.height
	;
	
	CGPoint	o = CGPointMake( CGRectGetMidX( self.bounds ), CGRectGetMidY( self.bounds ) );

	for ( NSInteger i = 0; i < _targetViews.count; i++ ) ((UIView*)_targetViews[ i ]).hidden = false;
	switch ( _targetViews.count ) {
	case 0:
		break;
	case 1:
		{	[ self SetupTargetView:0 D:wBoundD Center:o ];
		}
		break;
	case 2:
		{	CGFloat d = wBoundD / ( sqrt( 2 ) + 1 );
			CGFloat w = ( wBoundD - d ) / 2;
			[ self SetupTargetView:0 D:d Center:CGPointMake( o.x - w, o.y - w ) ];
			[ self SetupTargetView:1 D:d Center:CGPointMake( o.x + w, o.y + w ) ];
		}
		break;
	case 3:
		{	CGFloat d = wBoundD  / 2;
			CGFloat r = d / 2;
			[ self SetupTargetView:0 D:d Center:CGPointMake( o.x, o.y - r ) ];
			CGFloat w = ( 2 / sqrt( 3 ) - 1 + 1 / sqrt( 3 ) ) * r;
			[ self SetupTargetView:1 D:d Center:CGPointMake( o.x - r, o.y + w ) ];
			[ self SetupTargetView:2 D:d Center:CGPointMake( o.x + r, o.y + w ) ];
		}
		break;
	default:
		{	CGFloat d = wBoundD / 2;
			CGFloat w = d / 2;
			[ self SetupTargetView:0 D:d Center:CGPointMake( o.x - w, o.y - w ) ];
			[ self SetupTargetView:1 D:d Center:CGPointMake( o.x + w, o.y - w ) ];
			[ self SetupTargetView:2 D:d Center:CGPointMake( o.x - w, o.y + w ) ];
			[ self SetupTargetView:3 D:d Center:CGPointMake( o.x + w, o.y + w ) ];
		}
		for ( NSInteger i = 4; i < _targetViews.count; i++ ) ((UIView*)_targetViews[ i ]).hidden = true;
		break;
	}
}

@end


