/*
#import "JMPExileLayoutV.h"

#include	<JpMooParaiso/Cocoa.h>
using namespace JpMooParaiso::Apple;

@implementation
JMPExileLayoutV
{	CGFloat	uRotateΘ;
}

-	(void)
Rotate:(CGFloat)p
{	ToTimer
	(	1. / 30.
	,	^{	uRotateΘ += p;
			[ self setNeedsLayout ];
		}
	,	YES
	);
}

-	(void)
awakeFromNib
{	[ super awakeFromNib ];
	[ self Rotate:1./30. ];
}

-	(void)
setViews:(NSArray*)p
{	RemoveTaggedViews( self );

	CGFloat wSize = self.bounds.size.width < self.bounds.size.height
	?	self.bounds.size.width
	:	self.bounds.size.height
	;
	NSInteger	wTag = 0;
	for ( UIView* wV in p )
	{	wV.tag = ++wTag;
 		wV.frame = CGRectMake( 0, 0, wSize / 3, wSize / 3 );
		wV.layer.cornerRadius = wSize / 6;
		wV.clipsToBounds = YES;
		[ self addSubview: wV ];
	}
}

-	(void)
layoutSubviews;
{	[ super layoutSubviews ];

	CGFloat wSize = self.bounds.size.width < self.bounds.size.height
	?	self.bounds.size.width
	:	self.bounds.size.height
	;
 
	CGFloat wΘ = uRotateΘ;
	for ( UIView* wV in TaggedViews( self ) )
	{	wV.center = CGPointMake
		(	CGRectGetMidX( self.bounds ) + wSize / 3 * cos( wΘ )
		,	CGRectGetMidY( self.bounds ) - wSize / 3 * sin( wΘ )
		);
		wΘ += M_PI / 4;
	}
}

@end
*/