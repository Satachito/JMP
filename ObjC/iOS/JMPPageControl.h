#import <UIKit/UIKit.h>

@interface
JMPPageControl : UIControl

@property	(nonatomic, retain)	NSArray*	pageIconViews;
@property	(nonatomic)			NSInteger	currentPage;

@end

@interface
JMPPageControl(MultiColor)

@property	(nonatomic, retain)	NSArray*	pageIconColors;

@end

@interface
JMPPageControl(Images)

@property	(nonatomic, retain)	NSArray*	pageIconImages;

@end

