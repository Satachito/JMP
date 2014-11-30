#pragma once
//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

#import	<UIKit/UIKit.h>

@interface
JMPAlertViewDelegate : NSObject<UIAlertViewDelegate>

-	(id)
initWithClicked				:(void (^)( UIAlertView*, NSInteger ))	pClicked
Cancel						:(void (^)( UIAlertView* ))				pCancel
ShouldEnableFirstOtherButton:(BOOL (^)( UIAlertView* ))				pShouldEnableFirstOtherButton
WillPresent					:(void (^)( UIAlertView* ))				pWillPresent
DidPresent					:(void (^)( UIAlertView* ))				pDidPresent
WillDismiss					:(void (^)( UIAlertView*, NSInteger ))	pWillDismiss
DidDismiss					:(void (^)( UIAlertView*, NSInteger ))	pDidDismiss
;

@end

namespace	JpMooParaiso
{	namespace	Apple
	{
		inline	JMPAlertViewDelegate*
		AlertViewDelegate
		(	void	(^pClicked)						( UIAlertView*, NSInteger )	= nil
		,	void	(^pCancel)						( UIAlertView* )			= nil
		,	BOOL	(^pShouldEnableFirstOtherButton)( UIAlertView* )			= nil
		,	void	(^pWillPresent)					( UIAlertView* )			= nil
		,	void	(^pDidPresent)					( UIAlertView* )			= nil
		,	void	(^pWillDismiss)					( UIAlertView*, NSInteger )	= nil
		,	void	(^pDidDismiss)					( UIAlertView*, NSInteger )	= nil
		)
		{	return
			[	JMPAlertViewDelegate.alloc
				initWithClicked				:pClicked
				Cancel						:pCancel
				ShouldEnableFirstOtherButton:pShouldEnableFirstOtherButton
				WillPresent					:pWillPresent
				DidPresent					:pDidPresent
				WillDismiss					:pWillDismiss
				DidDismiss					:pDidDismiss
			];
		}

		inline	UIAlertView*
		AlertView
		(	NSString*	pTitle
		,	NSString*	pMessage													= nil
		,	NSString*	pCancelTitle												= nil
		,	NSArray*	pOtherTitleAndProcs											= nil
		,	BOOL		(^pShouldEnableFirstOtherButton)( UIAlertView* )			= nil
		,	void		(^pWillPresent)					( UIAlertView* )			= nil
		,	void		(^pDidPresent)					( UIAlertView* )			= nil
		,	void		(^pWillDismiss)					( UIAlertView*, NSInteger )	= nil
		,	void		(^pDidDismiss)					( UIAlertView*, NSInteger )	= nil
		)
		{
			static	JMPAlertViewDelegate* wAVD = nil;
			if ( ! wAVD )
			{	wAVD = AlertViewDelegate
				(	^( UIAlertView* pAV, NSInteger p )
					{	if ( pAV.cancelButtonIndex == p ) return;
						if ( pAV.cancelButtonIndex == 0 ) p--;
						void (^w)( UIAlertView* ) = pOtherTitleAndProcs[ p ][ 1 ];
						if ( w ) w( pAV );
					}
				);
			}

			UIAlertView* v =
			[	UIAlertView.alloc
				initWithTitle:pTitle
				message:pMessage
				delegate:wAVD
				cancelButtonTitle:pCancelTitle
				otherButtonTitles:nil
			];
			for ( NSArray* w in pOtherTitleAndProcs ) [ v addButtonWithTitle:w[ 0 ] ];
			return v;
		}
	}
}

/*	USAGE
	[	AlertView
		(	@"Title"
		,	@"Message"
		,	@"Cancel"
		,	@[	[ @"Option1",	^{ NSLog( @"Option1 Pressed" ); } ]
			,	@{	@"title":	@"Option2"
				,	@"proc"	:
					^{ NSLog( @"Option2 Pressed" );
					}
				}
			]
		)
		show
	];
*/