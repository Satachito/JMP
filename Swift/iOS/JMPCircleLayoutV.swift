//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

import	UIKit

class
JMPCircleLayoutV : UIView {
	var	uTimer		:	dispatch_source_t!
	var	uRotateΘ	:	Double = 0

	var	targetViews	:	[ UIView ] = [] {
		willSet	{ for wV in targetViews { wV.removeFromSuperview() } }
		didSet	{
			for wV in targetViews { addSubview( wV ) }
		}
	}

	func
	Rotate( p : Double ) {
		if uTimer != nil { dispatch_source_cancel( uTimer ) }
		uTimer = dispatch_source_create(
			DISPATCH_SOURCE_TYPE_TIMER
		,	0
		,	0
		,	dispatch_get_main_queue()
		)
		dispatch_source_set_timer( uTimer, dispatch_time( DISPATCH_TIME_NOW, 0 ), NSEC_PER_SEC / 30, 0 )
		dispatch_source_set_event_handler(
			uTimer
		,	{	self.uRotateΘ = self.uRotateΘ + p
				self.setNeedsLayout()
			}
		)
		dispatch_resume( uTimer )
	}

	override	func
	awakeFromNib() {
		super.awakeFromNib()
		Rotate( 0.05 )
	}

	func
	SetupTargetView( p : Int, _ pR : CGFloat, _ pCenter : CGPoint ) {
		let w = targetViews[ p ]
		w.frame = CGRectMake( 0, 0, pR * 2, pR * 2 )
		w.layer.cornerRadius = pR
		w.clipsToBounds = true
		w.center = pCenter
	}

	override	func
	layoutSubviews() {
		super.layoutSubviews();

		let wBoundsR = self.bounds.size.width < self.bounds.size.height
		?	Double( self.bounds.size.width / 2 )
		:	Double( self.bounds.size.height / 2 )

		let	wCenter = CGPointMake( CGRectGetMidX( self.bounds ), CGRectGetMidY( self.bounds ) )

		switch targetViews.count {
		case 0:
			break
		case 1:
			SetupTargetView( 0, CGFloat( wBoundsR ), wCenter )
		default:
			let	wCosΘ = cos( Double( targetViews.count - 2 ) * M_PI / Double( targetViews.count * 2 ) )
			let	wElementR = ( Double( wBoundsR ) * wCosΘ ) / ( wCosΘ + 1 )
			let	wTraceR = wBoundsR - wElementR
			for i in 0 ..< targetViews.count {
				let	wΘ = Double( i ) * M_PI * 2 / Double( targetViews.count )
				SetupTargetView(
					i
				,	CGFloat( wElementR )
				,	CGPointMake(
						CGFloat( Double( wCenter.x ) + wTraceR * cos( uRotateΘ + wΘ ) )
					,	CGFloat( Double( wCenter.y ) + wTraceR * sin( uRotateΘ + wΘ ) )
					)
				)
			}
		}
	}
}
