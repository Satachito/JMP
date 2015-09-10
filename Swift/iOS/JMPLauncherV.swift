//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

import	UIKit
import	QuartzCore

class
JMPLauncherV : UIScrollView, UIScrollViewDelegate {
	var
	outerMarginH	:	CGFloat = 10 { didSet { SyncLayout() } }
	var
	outerMarginV	:	CGFloat = 10 { didSet { SyncLayout() } }
	var
	cellWidth		:	CGFloat = 80 { didSet { SyncLayout() } }
	var
	cellHeight		:	CGFloat = 96 { didSet { SyncLayout() } }
	var
	cellMarginV		:	CGFloat = 10 { didSet { SyncLayout() } }
	override	var
	bounds			:	CGRect { didSet { SyncLayout() } }

	var
	changed			:	(() -> Void)?

	var
	editMode		:	Bool = false { didSet { SyncEditMode() } }

	var
	targetViews		:	[ UIView ] = [] {
		willSet {
			for wV in targetViews {
				wV.superview!.removeFromSuperview()
				wV.removeFromSuperview()
			}
		}
		didSet {
			let	wRect = CGRectMake( 0, 0, cellWidth, cellHeight )
			for wV in targetViews {
				let wTrayV = UIView( frame:wRect )
				wTrayV.userInteractionEnabled = true
				wTrayV.addGestureRecognizer( UILongPressGestureRecognizer( target:self, action:"LongPressed:" ) )
				addSubview( wTrayV )

				wV.frame = wRect
				wTrayV.addSubview( wV )
				
				let wDeleteIV = UIImageView( image:UIImage( named:"delete" ) )
				wDeleteIV.userInteractionEnabled = true
				wDeleteIV.addGestureRecognizer( UITapGestureRecognizer( target:self, action:"Delete:" ) )
				wTrayV.addSubview( wDeleteIV )

			}
			SyncLayout()
			SyncEditMode()
		}
	}

	var	uCellNumH		:	Int = 0
	var	uCellMarginH	:	CGFloat = 0

	func
	SyncLayout() {
		let wExtentH = CGRectGetWidth( self.bounds ) - outerMarginH * 2
		uCellNumH = Int( wExtentH / cellWidth )
		uCellMarginH = ( ( wExtentH - CGFloat( uCellNumH ) * cellWidth ) / ( CGFloat( uCellNumH ) - 1 ) )
		let wNumV = ( targetViews.count + uCellNumH - 1 ) / uCellNumH
		self.contentSize = CGSizeMake(
			CGRectGetWidth( self.bounds )
		,	outerMarginV * 2 + cellHeight * CGFloat( wNumV ) + cellMarginV * CGFloat( wNumV - 1 )
		)
		for i in 0 ..< targetViews.count { targetViews[ i ].superview!.center = self.SlotCenter( i ) }
	}
	
	func
	SyncEditMode() {
		if editMode {
			for wV in targetViews {
				( wV.superview!.subviews[ 1 ] as UIView ).hidden = false
			
				let wBA = CABasicAnimation( keyPath:"transform.rotation" )
				wBA.duration = 0.1
				wBA.repeatCount = 100000000
				wBA.autoreverses = true
				wBA.beginTime = 0.2 * CFTimeInterval( rand() % 100 ) / 100.0
				wBA.timingFunction = CAMediaTimingFunction( name:kCAMediaTimingFunctionEaseOut )
				wBA.fromValue = 0.05
				wBA.toValue = -0.05
				wBA.removedOnCompletion = true
				wBA.fillMode = kCAFillModeForwards
				wV.superview!.layer.addAnimation( wBA, forKey:"shake" )
			}
		} else {
			for wV in targetViews {
				( wV.superview!.subviews[ 1 ] as UIView ).hidden = true
				wV.superview!.layer.removeAllAnimations()
			}
		}
	}
	
	required
	init( coder p: NSCoder ) {
		super.init( coder: p )!
		addGestureRecognizer( UITapGestureRecognizer( target:self, action:"Tapped:" ) )
	}

	override
	init( frame p: CGRect ) {
		super.init( frame: p )
		addGestureRecognizer( UITapGestureRecognizer( target:self, action:"Tapped:" ) )
	}

	@IBAction	func
	Tapped( p: UITapGestureRecognizer ) {
		if ( p.state == .Ended ) { self.editMode = false }
	}

	func
	SlotFrame( p: Int ) -> CGRect {
		return CGRectMake(
			outerMarginH + ( cellWidth + uCellMarginH ) * CGFloat( p % uCellNumH )
		,	outerMarginV + ( cellHeight + cellMarginV ) * CGFloat( p / uCellNumH )
		,	cellWidth
		,	cellHeight
		)
	}

	func
	SlotCenter( p: Int ) -> CGPoint {
		let w = SlotFrame( p )
		return CGPointMake( CGRectGetMidX( w ), CGRectGetMidY( w ) )
	}

	func
	FindSlot( p: UILongPressGestureRecognizer ) -> Int? {
		for i in 0 ..< targetViews.count {
			if CGRectContainsPoint( SlotFrame( i ), p.view!.center ) { return i; }
		}
		return nil;
	}

	func
	LongPressed( p: UILongPressGestureRecognizer ) {
		switch p.state {
		case .Began:
			self.editMode = true
			bringSubviewToFront( p.view! )
			for i in 0 ..< targetViews.count { targetViews[ i ].superview!.tag = i }
		case .Changed:
			p.view!.center = p.locationInView( p.view!.superview )
			let	wTargetIndex = FindSlot( p )
			if wTargetIndex > 0 {
				let	wSourceIndex = p.view!.tag
				Animate() {
					for i in 0 ..< self.targetViews.count {
						if i != wSourceIndex {
							var w = i;
							if w > wSourceIndex { w-- }
							if w >= wTargetIndex { w++ }
							self.targetViews[ i ].superview!.center = self.SlotCenter( w )
						}
					}
				}
			}
		case .Ended:
			let	wTargetIndex = FindSlot( p )
			if (wTargetIndex != nil) {
				let	wSourceIndex = p.view!.tag
				if wSourceIndex != wTargetIndex {
					let wV = targetViews.removeAtIndex( wSourceIndex )
					targetViews.insert( wV, atIndex:wTargetIndex! )
					if changed != nil { changed!() }
				}
			}
			AnimateToProperPosition()
		default:
			AnimateToProperPosition()
		}
	}

	func
	AnimateToProperPosition() {
		Animate() {
			for i in 0 ..< self.targetViews.count {
				self.targetViews[ i ].superview!.center = self.SlotCenter( i )
			}
		}
	}

	func
	Delete( p: UITapGestureRecognizer ) {
		sendSubviewToBack( p.view!.superview! )
		for i in 0 ..< targetViews.count { targetViews[ i ].superview!.tag = i }
		let	wTag = p.view!.superview!.tag	//	tag will be cleared next removeFromSuperview
		targetViews.removeAtIndex( wTag )
		p.view!.superview!.removeFromSuperview()
		AnimateToProperPosition()
		if changed != nil { changed!() }
	}
}

