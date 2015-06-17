//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

//
//	View contoroller which moves view which contains UITextView along keyboard synchronously
//
//	Assumption	Jul. 5, 2014
//	This class assumes there is no bottom tool bar nor tabbar.
//

import	UIKit

func
DumpTV( p: UIScrollView ) {
	NSLog( "-------------------------------" );
	NSLog( "frame:%@", NSStringFromCGRect( p.frame ) );
	NSLog( "bounds:%@", NSStringFromCGRect( p.bounds ) );
	NSLog( "contentSize:%@", NSStringFromCGSize( p.contentSize ) );
	NSLog( "contentOffset:%@", NSStringFromCGPoint( p.contentOffset ) );
	NSLog( "contentInset:%@", NSStringFromUIEdgeInsets( p.contentInset ) );
	NSLog( "Current Bottom:%f", CGRectGetMaxY( p.bounds ) - p.contentInset.bottom );
}

class
JMPScrollControlVC: UIViewController {

	@IBOutlet	var	oContainerV					:	UIView! = nil

	@IBOutlet	var	oControlV					:	UIView! = nil
	@IBOutlet	var	oInputV						:	UITextView! = nil

				var	uInputAccessorySuperview	:	UIView! = nil

	weak		var	uWillShow					:	NSObjectProtocol! = nil
	weak		var	uDidChange					:	NSObjectProtocol! = nil
	weak		var	uWillHide					:	NSObjectProtocol! = nil

//	#pragma	mark	----------------------------------------------------------------	Stubs to be overridden

	func
	ScrollView() -> UIScrollView {
		return oContainerV.subviews[ 0 ] as! UIScrollView
	}

//	#pragma	mark	----------------------------------------------------------------	Utilities

	@IBAction	func
	Test() {
		DumpTV( self.ScrollView() )
	}

	@IBAction	func
	DoEndEditing() {
		self.view.endEditing( true )
	}

//	#pragma	mark	----------------------------------------------------------------	Lifecycle

	deinit {
		if uInputAccessorySuperview != nil { uInputAccessorySuperview.removeObserver( self, forKeyPath: "frame" ) }
	}

	override	func
	viewDidLoad() {
		super.viewDidLoad()
		
		oInputV.inputAccessoryView = UIView()
		oInputV.inputAccessoryView!.frame = CGRectMake( 0, 0, 320, 0 )
		oInputV.inputAccessoryView!.autoresizingMask = .FlexibleWidth

		oInputV.scrollsToTop = false
	}

	func
	AdjustWithKB( p: NSNotification ) {
		var	wRect = ( ( p.userInfo as! [ NSString: AnyObject ] )[ UIKeyboardFrameEndUserInfoKey ] as! NSValue ).CGRectValue()
		if !CGRectIsNull( wRect ) {
			wRect = self.view.convertRect( wRect, fromView: self.view.window )	//	KeyboardRect

			oControlV.frame = CGRectMake (
				wRect.origin.x
			,	wRect.origin.y - oControlV.frame.size.height
			,	wRect.size.width
			,	oControlV.frame.size.height
			)

			let	wSV = self.ScrollView()
			let	wVisibleMax = CGRectGetMaxY( wSV.bounds ) - wSV.contentInset.bottom;
			//	Scroll so that the bottom part of SV fits to the top of KB
			wSV.contentOffset = CGPointMake (
				wSV.contentOffset.x
			,	( wVisibleMax < wSV.contentSize.height ? wVisibleMax : wSV.contentSize.height ) - wRect.origin.y
			)
		}
	}

	func
	AdjustWithKBSynchedToKB( p: NSNotification ) {	//	Used only in case of iOS 7
		UIView.animateWithDuration	(	( ( p.userInfo as! [ NSString: AnyObject ] )[ UIKeyboardAnimationDurationUserInfoKey ] as! NSNumber ).doubleValue
		,	delay					:	0
		,	options					:	UIViewAnimationOptions(
				rawValue: UInt(
					( ( p.userInfo as! [ NSString: AnyObject ] )[ UIKeyboardAnimationCurveUserInfoKey ] as! NSNumber ).unsignedIntValue << 16
				)
			)
		,	animations				:	{}
		,	completion				:	nil
		)
	}
	
	func
	AdjustWithAccessoryV() {
		var wRect = oInputV.inputAccessoryView!.superview!.frame;
		wRect = self.view.convertRect( wRect, fromView:self.view.superview )	//	KeyboardRect
		wRect.origin.y -= oControlV.frame.size.height;
		wRect.size.height = oControlV.frame.size.height;
		oControlV.frame = wRect;
	}

	override	func
	viewWillAppear( p: Bool ) {
		super.viewWillAppear( p )

		uDidChange = Notify( "UIKeyboardCandidateCorrectionDidChangeNotification" ) {
			p in if let _ = self.oInputV.inputAccessoryView!.superview { self.AdjustWithAccessoryV() }
		}
		uWillShow = Notify( UIKeyboardWillShowNotification ) {
			p in self.AdjustWithKB( p )
		}
		uWillHide = Notify( UIKeyboardWillHideNotification ) {
			p in self.AdjustWithKB( p )
		}
	}

	func
	observeValueForKeyPath(
		keyPath: String!
	,	ofObject: AnyObject!
	,	change: NSDictionary!
	,	context: UnsafePointer<Void>!
	) {
		if ofObject as? UIView == oInputV.inputAccessoryView!.superview { AdjustWithAccessoryV() }
	}

	override	func
	viewWillDisappear( p: Bool ) {
		super.viewWillDisappear( p )

		NSNotificationCenter.defaultCenter().removeObserver( uDidChange )
		NSNotificationCenter.defaultCenter().removeObserver( uWillShow )
		NSNotificationCenter.defaultCenter().removeObserver( uWillHide )
	}
}
