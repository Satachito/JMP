//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

#import 			"LazyBGColorV.h"
#include			<JpMooParaiso/Cocoa.h>
using	namespace	JpMooParaiso::Apple;

@implementation
LazyBGColorV
{	NSURLSessionTask*	uST;
}

-	(void)
Load:(NSURL*)p
Completion:(void (^)())pCompletion
ActivityIndicatorView:(UIActivityIndicatorView*)pAIV
ErrorHandler:(void (^)( NSError* ))pErrorHandler
ResponseHandler:(void (^)( NSURLResponse* ))pResponseHandler
{
	if ( uST ) [ uST cancel ];

	pAIV.center = [ self convertPoint:self.center fromView:self.superview ];
	[ self addSubview:pAIV ];
	[ pAIV startAnimating ];

	uST = DataTask
	(	p
	,	^()
		{	[ pAIV stopAnimating ];
			[ pAIV removeFromSuperview ];
			uST = nil;
		}
	,	^( NSData* q )
		{	self.backgroundColor = ToColor( ToImage( q ) );
			pCompletion();
		}
	,	pErrorHandler
	,	pResponseHandler
	);
	[ uST resume ];
}

-	(IBAction)
Cancel
{	[ uST cancel ];
}

@end

