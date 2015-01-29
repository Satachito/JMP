#import "JMPPageControl.h"

@implementation
JMPPageControl

-	(void)
setCurrentPage:(NSInteger)p {	//	May be signaled in case of out of bounds;
//	[	UIView
//		animateWithDuration:.3
//		animations:^{
//			if ( _currentPage < _pageIconViews.count ) ((UIView*)_pageIconViews[ _currentPage ]).transform = CGAffineTransformIdentity;
//			_currentPage = p;
//			if ( _currentPage < _pageIconViews.count ) ((UIView*)_pageIconViews[ _currentPage ]).transform = CGAffineTransformMakeScale( 1.2, 1.2 );
//		}
//	];
	[	UIView
		animateWithDuration:.3
		delay:0
		usingSpringWithDamping:.1
		initialSpringVelocity:0
		options:UIViewAnimationOptionCurveEaseInOut
		animations:^{
			if ( _currentPage < _pageIconViews.count ) ((UIView*)_pageIconViews[ _currentPage ]).transform = CGAffineTransformIdentity;
			_currentPage = p;
			if ( _currentPage < _pageIconViews.count ) ((UIView*)_pageIconViews[ _currentPage ]).transform = CGAffineTransformMakeScale( 1.2, 1.2 );
		}
		completion:nil
	];

	[ self sendActionsForControlEvents:UIControlEventValueChanged ];
}


-	(void)
setPageIconViews:(NSArray*)p {
	for ( UIView* wV in _pageIconViews ) {
		[ wV removeFromSuperview ];
		[ self setNeedsLayout ];
	}
	_pageIconViews = p;
	for ( UIView* wV in _pageIconViews ) {
		[ self addSubview:wV ];
		[ self setNeedsLayout ];
	}
	
	if ( _pageIconViews.count ) {
		self.currentPage = _currentPage < _pageIconViews.count ? _currentPage : _pageIconViews.count - 1;
	}
}

-	(void)
layoutSubviews {

	[ super layoutSubviews ];
	
	CGFloat	wLeft = ( self.bounds.size.width - self.bounds.size.height * _pageIconViews.count ) / 2;
	for ( NSInteger i = 0; i < _pageIconViews.count; i++ ) {
		((UIView*)_pageIconViews[ i ]).center = CGPointMake(
			wLeft + ( i + .5 ) * self.bounds.size.height
		,	self.bounds.size.height / 2
		);
	}
}

-	(void)
touchesEnded:(NSSet*)touches
withEvent:(UIEvent*)event {

	[ super touchesEnded:touches withEvent:event ];

	CGPoint w = [ touches.anyObject locationInView:self ];
	if ( CGRectContainsPoint( self.bounds, w ) ) {
		CGFloat	wLeft = ( self.bounds.size.width - self.bounds.size.height * _pageIconViews.count ) / 2;
		if ( w.x < wLeft ) {
			if ( _currentPage ) self.currentPage = _currentPage - 1;
		} else {
			int	wPage = ( w.x - wLeft ) / self.bounds.size.height;
			if ( wPage < _pageIconViews.count ) {
				if ( wPage != _currentPage ) self.currentPage = wPage;
			} else {
				if ( _currentPage < _pageIconViews.count - 1 ) self.currentPage = _currentPage + 1;
			}
		}
	}
}

@end


@implementation
JMPPageControl(MultiColor)

-	(NSArray*)
pageIconColors {
	NSMutableArray* v = NSMutableArray.array;
	for ( UIView* wV in _pageIconViews ) [ v addObject:wV.backgroundColor ];
	return v;
}

-	(void)
setPageIconColors:(NSArray*)p {
	NSMutableArray* wA = NSMutableArray.new;
	for ( NSInteger i = 0; i < p.count; i++ ) {
		CGFloat wSize = self.bounds.size.height * .5;
		UIView* wV = [ UIView.alloc initWithFrame:CGRectMake( 0, 0, wSize, wSize ) ];
		wV.layer.cornerRadius = wSize / 2;
		wV.backgroundColor = p[ i ];
		[ wA addObject:wV ];
	}
	self.pageIconViews = wA;
}

@end



@implementation
JMPPageControl(Images)

-	(NSArray*)
pageIconImages {
	NSMutableArray* v = NSMutableArray.array;
	for ( UIImageView* wV in _pageIconViews ) {
		assert( [ wV isKindOfClass:UIImageView.class ] );
		[ v addObject:wV.image ];
	}
	return v;
}

-	(void)
setPageIconImages:(NSArray*)p {
	NSMutableArray* wA = NSMutableArray.new;
	for ( NSInteger i = 0; i < p.count; i++ ) {
		CGFloat wSize = self.bounds.size.height * .5;
		UIImageView* wV = [ UIImageView.alloc initWithFrame:CGRectMake( 0, 0, wSize, wSize ) ];
		wV.image = p[ i ];
		[ wA addObject:wV ];
	}
	self.pageIconViews = wA;
}

@end

