//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

#import "JMPAlertView.h"

@implementation
JMPAlertViewDelegate
{	void	(^uClicked)						( UIAlertView*, NSInteger );
	void	(^uCancel)						( UIAlertView* );
	BOOL	(^uShouldEnableFirstOtherButton)( UIAlertView* );
	void	(^uWillPresent)					( UIAlertView* );
	void	(^uDidPresent)					( UIAlertView* );
	void	(^uWillDismiss)					( UIAlertView*, NSInteger );
	void	(^uDidDismiss)					( UIAlertView*, NSInteger );
}

-	(id)
initWithClicked				:(void (^)( UIAlertView*, NSInteger ))	pClicked
Cancel						:(void (^)( UIAlertView* ))				pCancel
ShouldEnableFirstOtherButton:(BOOL (^)( UIAlertView* ))				pShouldEnableFirstOtherButton
WillPresent					:(void (^)( UIAlertView* ))				pWillPresent
DidPresent					:(void (^)( UIAlertView* ))				pDidPresent
WillDismiss					:(void (^)( UIAlertView*, NSInteger ))	pWillDismiss
DidDismiss					:(void (^)( UIAlertView*, NSInteger ))	pDidDismiss
{	if ( ( self = super.init ) )
	{	uClicked						= pClicked;
		uCancel							= pCancel;
		uShouldEnableFirstOtherButton	= pShouldEnableFirstOtherButton;
		uWillPresent					= pWillPresent;
		uDidPresent						= pDidPresent;
		uWillDismiss					= pWillDismiss;
		uDidDismiss						= pDidDismiss;
	}
	return self;
}

-	(void)
alertView								:(UIAlertView*)pAV
clickedButtonAtIndex					:(NSInteger)p
{	if ( uClicked )		uClicked( pAV, p );
}

-	(void)
alertViewCancel							:(UIAlertView*)pAV
{	if ( uCancel )		uCancel( pAV );
}

-	(BOOL)
alertViewShouldEnableFirstOtherButton	:(UIAlertView*)pAV
{	return uShouldEnableFirstOtherButton ? uShouldEnableFirstOtherButton( pAV ) : YES;
}

-	(void)
willPresentAlertView					:(UIAlertView*)pAV
{	if ( uWillPresent )	uWillPresent( pAV );
}

-	(void)
didPresentAlertView						:(UIAlertView*)pAV
{	if ( uDidPresent )	uDidPresent( pAV );
}

-	(void)
alertView								:(UIAlertView*)pAV
willDismissWithButtonIndex				:(NSInteger)p
{	if ( uWillDismiss )	uWillDismiss( pAV, p );
}

-	(void)
alertView								:(UIAlertView*)pAV
didDismissWithButtonIndex				:(NSInteger)p
{	if ( uDidDismiss )	uDidDismiss( pAV, p );
}

@end

