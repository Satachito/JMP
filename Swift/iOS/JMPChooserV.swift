//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

import	UIKit

class
JMPChooserV : UIScrollView, UIScrollViewDelegate
{	var	choosen	:	(( UIView ) -> Void)!
	var	tapped	:	(( UIView ) -> Void)!

	var	choice	:	UIView! = nil {
		didSet	{
			if choice != nil {
				if self.contentOffset.x == choice.center.x - self.bounds.size.width / 2 {
					if choosen != nil { choosen( choice ) }
				} else {
					self.setContentOffset(
						CGPointMake( choice.center.x - self.bounds.size.width / 2, self.contentOffset.y )
					,	animated:true
					)
				}
			}
		}
	}
	
	var	targetViews	:	[ UIView ] = [] {
		willSet	{ for wV in targetViews { wV.removeFromSuperview() } }
		didSet	{
			if targetViews.count != 0 {
				for wV in targetViews {
					self.addSubview( wV )
					wV.addGestureRecognizer( UITapGestureRecognizer( target:self, action:"Tapped:" ) )
					wV.userInteractionEnabled = true;
				}
				var	wCenter = CGPointMake( self.bounds.size.width / 2, self.bounds.size.height / 2 )
				targetViews[ 0 ].center = wCenter;
				for i in 1 ..< targetViews.count {
					let wPrevV = targetViews[ i - 1 ]
					let wV = targetViews[ i ]
					wCenter.x += wPrevV.frame.size.width / 2 + wV.frame.size.width / 2
					wV.center = wCenter;
				}
				
				self.contentSize = CGSizeMake( wCenter.x + self.bounds.size.width / 2, self.bounds.size.height );
			}
		}
	}

	required
	init( coder p: NSCoder ) {
		super.init( coder: p )
		self.delegate = self;
	}

	override
	init( frame p: CGRect ) {
		super.init( frame: p )
		self.delegate = self
	}

	func
	Tapped( p : UITapGestureRecognizer ) {
		if self.contentOffset.x == p.view!.center.x - self.bounds.size.width / 2 {
			if tapped != nil { tapped( p.view! ) }
		}
		self.choice = p.view
	}

	func
	scrollViewDidEndDecelerating( UIScrollView! ) {
		if choosen != nil { choosen( choice ) }
	}

	func
	scrollViewDidEndDragging( scrollView: UIScrollView!, willDecelerate: Bool ) {
		if willDecelerate { return }
		if choosen != nil { choosen( choice ) }
	}

	func
	scrollViewDidEndScrollingAnimation( UIScrollView! ) {
		if choosen != nil { choosen( choice ) }
	}

	func
	scrollViewWillEndDragging(
		UIScrollView!
	,	withVelocity		:	CGPoint
	,	targetContentOffset	:	UnsafeMutablePointer<CGPoint>
	) {
		if targetViews.count != 0 {
			let wTargetX = UnsafePointer<CGPoint>(targetContentOffset).memory.x + self.bounds.size.width / 2
			choice = targetViews[ 0 ]
			for i in 1 ..< targetViews.count {
				if 	fabs( Double( targetViews[ i ].center.x - wTargetX ) )
				<	fabs( Double( choice.center.x - wTargetX ) )
				{	choice = targetViews[ i ]
				}
			}
			targetContentOffset.memory = CGPointMake(
				choice.center.x - self.bounds.size.width / 2
			,	self.contentOffset.y
			)
		}
	}
}
