//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

import	UIKit

class
JMPDiceLayoutV : UIView {
	func
	SetupTargetView( p : Int, _ pR : CGFloat, _ pCenter : CGPoint ) {
		let w = targetViews[ p ]
		w.frame = CGRectMake( 0, 0, pR * 2, pR * 2 )
		w.layer.cornerRadius = pR
		w.clipsToBounds = true
		w.center = pCenter
	}

	var	targetViews	:	[ UIView ] = [] {
		willSet	{ for wV in targetViews { wV.removeFromSuperview() } }
		didSet	{
			for wV in targetViews { self.addSubview( wV ) }

			let wBoundsR = self.bounds.size.width < self.bounds.size.height
			?	self.bounds.size.width / 2
			:	self.bounds.size.height / 2

			let	o = CGPointMake( CGRectGetMidX( self.bounds ), CGRectGetMidY( self.bounds ) )

			switch targetViews.count {
			case 0:
				break
			case 1:
				SetupTargetView( 0, wBoundsR, o )
			case 2:
				let r = CGFloat( sqrt( 2 ) * Double( wBoundsR ) / ( sqrt( 2 ) + 1.0 ) )
				let w = wBoundsR - r
				SetupTargetView( 0, r, CGPointMake( o.x - w, o.y - w ) )
				SetupTargetView( 1, r, CGPointMake( o.x + w, o.y + w ) )

			case 3:
				let r = wBoundsR / 2
				let	w = CGFloat( Double( wBoundsR ) * ( 2.0 - sqrt( 3 ) ) / 4 )
				SetupTargetView( 0, r, CGPointMake( o.x, o.y - r + w ) )
				SetupTargetView( 1, r, CGPointMake( o.x - r, o.y + r - w ) )
				SetupTargetView( 2, r, CGPointMake( o.x + r, o.y + r - w ) )
			case 4:
				let r = wBoundsR / 2
				SetupTargetView( 0, r, CGPointMake( o.x - r, o.y - r ) )
				SetupTargetView( 1, r, CGPointMake( o.x + r, o.y - r ) )
				SetupTargetView( 2, r, CGPointMake( o.x - r, o.y + r ) )
				SetupTargetView( 3, r, CGPointMake( o.x + r, o.y + r ) )
			case 5:
				let r = CGFloat( Double( wBoundsR ) / ( 1 + sqrt( 2 ) ) )
				SetupTargetView( 0, r, CGPointMake( o.x - wBoundsR + r, o.y - wBoundsR + r ) )
				SetupTargetView( 1, r, CGPointMake( o.x + wBoundsR - r, o.y - wBoundsR + r ) )
				SetupTargetView( 2, r, o )
				SetupTargetView( 3, r, CGPointMake( o.x - wBoundsR + r, o.y + wBoundsR - r ) )
				SetupTargetView( 4, r, CGPointMake( o.x + wBoundsR - r, o.y + wBoundsR - r ) )
			case 6:
				let r = CGFloat( wBoundsR / 3 )
				let x = CGFloat( cos( M_PI / 6.0 ) * Double( r * 2 ) )
				let y = CGFloat( sin( M_PI / 6.0 ) * Double( r * 2 ) )
				SetupTargetView( 0, r, CGPointMake( o.x, o.y - r * 2 ) )
				SetupTargetView( 1, r, CGPointMake( o.x - x, o.y - y ) )
				SetupTargetView( 2, r, CGPointMake( o.x + x, o.y - y ) )
				SetupTargetView( 3, r, CGPointMake( o.x - x, o.y + y ) )
				SetupTargetView( 4, r, CGPointMake( o.x + x, o.y + y ) )
				SetupTargetView( 5, r, CGPointMake( o.x, o.y + r * 2 ) )
	//		case 7:
			default:
				let r = CGFloat( wBoundsR / 3 )
				let x = CGFloat( cos( M_PI / 6.0 ) * Double( r * 2 ) )
				let y = CGFloat( sin( M_PI / 6.0 ) * Double( r * 2 ) )
				SetupTargetView( 0, r, o )
				SetupTargetView( 1, r, CGPointMake( o.x, o.y - r * 2 ) )
				SetupTargetView( 2, r, CGPointMake( o.x - x, o.y - y ) )
				SetupTargetView( 3, r, CGPointMake( o.x + x, o.y - y ) )
				SetupTargetView( 4, r, CGPointMake( o.x - x, o.y + y ) )
				SetupTargetView( 5, r, CGPointMake( o.x + x, o.y + y ) )
				SetupTargetView( 6, r, CGPointMake( o.x, o.y + r * 2 ) )
				for i in 7 ..< targetViews.count { targetViews[ i ].hidden = true }
			}
		}
	}

/*

		let wSize = self.bounds.size.width < self.bounds.size.height
		?	self.bounds.size.width
		:	self.bounds.size.height

		let	o = CGPointMake( CGRectGetMidX( self.bounds ), CGRectGetMidY( self.bounds ) )

		switch targetViews.count {
		case 1:
			SetupTargetView( 0, CGFloat( wSize / 2 ), o )
		case 2:
			SetupTargetView( 0, wSize / 3, CGPointMake( o.x - wSize / 5, o.y - wSize / 5 ) )
			SetupTargetView( 1, wSize / 3, CGPointMake( o.x + wSize / 5, o.y + wSize / 5 ) )
		case 3:
			SetupTargetView( 0, wSize / 4, CGPointMake( o.x - wSize / 4, o.y - wSize / 4 ) )
			SetupTargetView( 1, wSize / 4, o )
			SetupTargetView( 2, wSize / 4, CGPointMake( o.x + wSize / 4, o.y + wSize / 4 ) )
		case 4:
			SetupTargetView( 0, wSize / 3, CGPointMake( o.x - wSize / 4, o.y - wSize / 4 ) )
			SetupTargetView( 1, wSize / 3, CGPointMake( o.x + wSize / 4, o.y - wSize / 4 ) )
			SetupTargetView( 2, wSize / 3, CGPointMake( o.x - wSize / 4, o.y + wSize / 4 ) )
			SetupTargetView( 3, wSize / 3, CGPointMake( o.x + wSize / 4, o.y + wSize / 4 ) )
		case 5:
			SetupTargetView( 0, wSize / 8, CGPointMake( o.x - wSize / 4, o.y - wSize / 4 ) )
			SetupTargetView( 1, wSize / 8, CGPointMake( o.x + wSize / 4, o.y - wSize / 4 ) )
			SetupTargetView( 2, wSize / 8, o )
			SetupTargetView( 3, wSize / 8, CGPointMake( o.x - wSize / 4, o.y + wSize / 4 ) )
			SetupTargetView( 4, wSize / 8, CGPointMake( o.x + wSize / 4, o.y + wSize / 4 ) )
		case 6:
			SetupTargetView( 0, wSize / 4, CGPointMake( o.x - wSize / 4, o.y - wSize / 3 ) )
			SetupTargetView( 1, wSize / 4, CGPointMake( o.x + wSize / 4, o.y - wSize / 3 ) )
			SetupTargetView( 2, wSize / 4, CGPointMake( o.x - wSize / 4, o.y ) )
			SetupTargetView( 3, wSize / 4, CGPointMake( o.x + wSize / 4, o.y ) )
			SetupTargetView( 4, wSize / 4, CGPointMake( o.x - wSize / 4, o.y + wSize / 3 ) )
			SetupTargetView( 5, wSize / 4, CGPointMake( o.x + wSize / 4, o.y + wSize / 3 ) )
		case 7:
			SetupTargetView( 0, wSize / 4, CGPointMake( o.x - wSize / 3, o.y - wSize / 3 ) )
			SetupTargetView( 1, wSize / 4, CGPointMake( o.x            , o.y - wSize / 3 ) )
			SetupTargetView( 2, wSize / 4, CGPointMake( o.x + wSize / 3, o.y - wSize / 3 ) )
			SetupTargetView( 3, wSize / 4, CGPointMake( o.x - wSize / 3, o.y ) )
			SetupTargetView( 4, wSize / 4, CGPointMake( o.x            , o.y ) )
			SetupTargetView( 5, wSize / 4, CGPointMake( o.x + wSize / 3, o.y ) )
			SetupTargetView( 6, wSize / 4, CGPointMake( o.x - wSize / 3, o.y + wSize / 3 ) )
		case 8:
			SetupTargetView( 0, wSize / 4, CGPointMake( o.x - wSize / 3, o.y - wSize / 3 ) )
			SetupTargetView( 1, wSize / 4, CGPointMake( o.x            , o.y - wSize / 3 ) )
			SetupTargetView( 2, wSize / 4, CGPointMake( o.x + wSize / 3, o.y - wSize / 3 ) )
			SetupTargetView( 3, wSize / 4, CGPointMake( o.x - wSize / 3, o.y ) )
			SetupTargetView( 4, wSize / 4, CGPointMake( o.x            , o.y ) )
			SetupTargetView( 5, wSize / 4, CGPointMake( o.x + wSize / 3, o.y ) )
			SetupTargetView( 6, wSize / 4, CGPointMake( o.x - wSize / 3, o.y + wSize / 3 ) )
			SetupTargetView( 7, wSize / 4, CGPointMake( o.x            , o.y + wSize / 3 ) )
		case 9:
			SetupTargetView( 0, wSize / 4, CGPointMake( o.x - wSize / 3, o.y - wSize / 3 ) )
			SetupTargetView( 1, wSize / 4, CGPointMake( o.x            , o.y - wSize / 3 ) )
			SetupTargetView( 2, wSize / 4, CGPointMake( o.x + wSize / 3, o.y - wSize / 3 ) )
			SetupTargetView( 3, wSize / 4, CGPointMake( o.x - wSize / 3, o.y ) )
			SetupTargetView( 4, wSize / 4, CGPointMake( o.x            , o.y ) )
			SetupTargetView( 5, wSize / 4, CGPointMake( o.x + wSize / 3, o.y ) )
			SetupTargetView( 6, wSize / 4, CGPointMake( o.x - wSize / 3, o.y + wSize / 3 ) )
			SetupTargetView( 7, wSize / 4, CGPointMake( o.x            , o.y + wSize / 3 ) )
			SetupTargetView( 8, wSize / 4, CGPointMake( o.x + wSize / 3, o.y + wSize / 3 ) )
			for i in 9..targetViews.count { targetViews[ i ].hidden = true }
		default:
			assert( false )
		}
*/
}
