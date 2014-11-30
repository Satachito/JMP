#pragma once
//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

#import	<UIKit/UIKit.h>

@interface
JMPActionSheetDelegate : NSObject<UIActionSheetDelegate>

-	(id)
initWithClicked	:(void (^)( UIActionSheet*, NSInteger ))	pClicked
Canceled		:(void (^)( UIActionSheet* ))				pCanceled
WillPresent		:(void (^)( UIActionSheet* ))				pWillPresent
DidPresent		:(void (^)( UIActionSheet* ))				pDidPresent
WillDismiss		:(void (^)( UIActionSheet*, NSInteger ))	pWillDismiss
DidDismiss		:(void (^)( UIActionSheet*, NSInteger ))	pDidDismiss
;

@end

namespace	JpMooParaiso
{	namespace	Apple
	{
		inline	JMPActionSheetDelegate*
		ActionSheetDelegate
		(	void	(^pClicked)		( UIActionSheet*, NSInteger )	= nil
		,	void	(^pCanceled)	( UIActionSheet* )				= nil
		,	void	(^pWillPresent)	( UIActionSheet* )				= nil
		,	void	(^pDidPresent)	( UIActionSheet* )				= nil
		,	void	(^pWillDismiss)	( UIActionSheet*, NSInteger )	= nil
		,	void	(^pDidDismiss)	( UIActionSheet*, NSInteger )	= nil
		)
		{	return
			[	JMPActionSheetDelegate.alloc
				initWithClicked	:pClicked
				Canceled		:pCanceled
				WillPresent		:pWillPresent
				DidPresent		:pDidPresent
				WillDismiss		:pWillDismiss
				DidDismiss		:pDidDismiss
			];
		}

		inline	UIActionSheet*
		ActionSheet
		(	NSString*	pTitle
		,	NSString*	pCancelTitle													= nil
		,	NSArray*	pDestructiveButton												= nil
		,	NSArray*	pOtherTitleAndProcs												= nil
		,	BOOL		(^pShouldEnableFirstOtherButton)( UIActionSheet* )				= nil
		,	void		(^pWillPresent)					( UIActionSheet* )				= nil
		,	void		(^pDidPresent)					( UIActionSheet* )				= nil
		,	void		(^pWillDismiss)					( UIActionSheet*, NSInteger )	= nil
		,	void		(^pDidDismiss)					( UIActionSheet*, NSInteger )	= nil
		)
		{
			static	JMPActionSheetDelegate* wASD = nil;
			if ( ! wASD )
			{	wASD = ActionSheetDelegate
				(	^( UIActionSheet* pAS, NSInteger p )
					{//	NSLog( @"%zd cancelButtonIndex:%zd", p, pAS.cancelButtonIndex );
						if ( p < pOtherTitleAndProcs.count )
						{	void (^w)() = pOtherTitleAndProcs[ p ][ 1 ];
							if ( w ) w();
						}
						else if ( p == pAS.destructiveButtonIndex )
						{	void (^w)() = pDestructiveButton[ 1 ];
							if ( w ) w();
						}
					}
				);
			}

			UIActionSheet* v =
			[	UIActionSheet.alloc
				initWithTitle:pTitle
				delegate:wASD
				cancelButtonTitle:nil
				destructiveButtonTitle:nil
				otherButtonTitles:nil
			];
			for ( NSArray* w in pOtherTitleAndProcs ) [ v addButtonWithTitle:w[ 0 ] ];
			NSInteger wIndex = pOtherTitleAndProcs.count;
			if ( pDestructiveButton )
			{	[ v addButtonWithTitle:pDestructiveButton[ 0 ] ];
				v.destructiveButtonIndex = wIndex++;
			}
			if ( pCancelTitle )
			{	[ v addButtonWithTitle:pCancelTitle ];
				v.cancelButtonIndex = wIndex;
			}
			return v;
		}
	}
}
/*	USAGE
	[	ActionSheet
		(	@"Title"
		,	@"Cancel"
		,	@{	@"title":	@"Destructive"
			,	@"proc"	:
				^{	NSLog( @"Destructive Pressed" );
				}
			}
		,	@[	@{	@"title":	@"Option1"
				,	@"proc"	:
					^{	NSLog( @"Option1 Pressed" );
					}
				}
			,	@{	@"title":	@"Option2"
				,	@"proc"	:
					^{ NSLog( @"Option2 Pressed" );
					}
				}
			]
		)
		showInView:self.view
	];
*/
