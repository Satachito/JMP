//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

#import "JMPActionSheet.h"

@implementation
JMPActionSheetDelegate
{	void	(^uClicked)		( UIActionSheet*, NSInteger );
	void	(^uCanceled)	( UIActionSheet* );
	void	(^uWillPresent)	( UIActionSheet* );
	void	(^uDidPresent)	( UIActionSheet* );
	void	(^uWillDismiss)	( UIActionSheet*, NSInteger );
	void	(^uDidDismiss)	( UIActionSheet*, NSInteger );
}

-	(id)
initWithClicked	:(void (^)( UIActionSheet*, NSInteger ))	pClicked
Canceled		:(void (^)( UIActionSheet* ))				pCanceled
WillPresent		:(void (^)( UIActionSheet* ))				pWillPresent
DidPresent		:(void (^)( UIActionSheet* ))				pDidPresent
WillDismiss		:(void (^)( UIActionSheet*, NSInteger ))	pWillDismiss
DidDismiss		:(void (^)( UIActionSheet*, NSInteger ))	pDidDismiss
{	if ( ( self = super.init ) )
	{	uClicked		= pClicked;
		uCanceled		= pCanceled;
		uWillPresent	= pWillPresent;
		uDidPresent		= pDidPresent;
		uWillDismiss	= pWillDismiss;
		uDidDismiss		= pDidDismiss;
	}
	return self;
}

-	(void)
actionSheet:(UIActionSheet*)pAS
clickedButtonAtIndex:(NSInteger)p
{	if ( uClicked )		uClicked( pAS, p );
}

-	(void)
actionSheetCancel:(UIActionSheet*)pAS
{	if ( uCanceled )	uCanceled( pAS );
}

-	(void)
willPresentActionSheet:(UIActionSheet*)pAS
{	if ( uWillPresent )	uWillPresent( pAS );
}

-	(void)
didPresentActionSheet:(UIActionSheet*)pAS
{	if ( uDidPresent )	uDidPresent( pAS );
}

-	(void)
actionSheet:(UIActionSheet*)pAS
willDismissWithButtonIndex:(NSInteger)p
{	if ( uWillDismiss )	uWillDismiss( pAS, p );
}

-	(void)
actionSheet:(UIActionSheet*)pAS
didDismissWithButtonIndex:(NSInteger)p
{	if ( uDidDismiss )	uDidDismiss( pAS, p );
}

@end

