//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

@interface
LazyIV : UIImageView

-	(void)
Load:(NSURL*)p
Completion:(void (^)())pCompletion
ActivityIndicatorView:(UIActivityIndicatorView*)pAIV
ErrorHandler:(void (^)( NSError* ))pErrorHandler
ResponseHandler:(void (^)( NSURLResponse* ))pResponseHandler
;
-	(IBAction)
Cancel
;
@end

inline	void
LoadImage
(	LazyIV* pNIV
,	NSURL* p
,	void (^pCompletion)() = nil
,	UIActivityIndicatorView* pAIV = nil
,	void (^pErrorHandler)( NSError* ) = nil
,	void (^pResponseHandler)( NSURLResponse* ) = nil
)
{	[	pNIV
		Load					:p
		Completion				:pCompletion ? pCompletion : ^(){}
		ActivityIndicatorView	:pAIV ? pAIV : [ UIActivityIndicatorView.alloc initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge ]
		ErrorHandler			:pErrorHandler ? pErrorHandler : ^( NSError* ){}
		ResponseHandler			:pResponseHandler ? pResponseHandler : ^( NSURLResponse* ){}
	];
}
