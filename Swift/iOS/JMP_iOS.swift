import UIKit

func
Dist2( left: CGPoint, right: CGPoint ) -> Double {
	let w = Double( right.x - left.x )
	let h = Double( right.y - left.y )
	return w * w + h * h
}

func
Center( p: CGRect ) -> CGPoint {
	return CGPointMake( CGRectGetMidX( p ), CGRectGetMidY( p ) )
}

func
InputBox(
	title		:	String!
, _	message		:	String! = nil
, _	config		:	( UITextField! -> () )! = nil
,	ok			:	( UITextField! -> () )
) {
	let wAC = UIAlertController(
		title			:	title
	,	message			:	message
	,	preferredStyle	:	.Alert
	)
	var wTF : UITextField!
	wAC.addTextFieldWithConfigurationHandler { p in
		if config != nil { config( p ) }
		wTF = p
	}
	wAC.addAction( UIAlertAction( title: "Cancel", style: .Cancel, handler:nil ) )
	wAC.addAction( UIAlertAction( title: "OK", style: .Default ) { _ in ok( wTF ) } )
	UIApplication.sharedApplication().keyWindow!.rootViewController!.presentViewController(
		wAC
	,	animated:true
	,	completion:nil
	)
}
	
func
Animate( p: () -> () ) {
	UIView.animateWithDuration( 0.25, animations: p )
}

func
SpringAnimate(
	duration	:	NSTimeInterval			= 0.25
,	delay		:	NSTimeInterval			= 0
,	damping		:	CGFloat					= 0.5
,	velocity	:	CGFloat					= 0.1
,	options		:	UIViewAnimationOptions	= .CurveEaseInOut
,	completion	:	( Bool -> () )!			= nil
,	animations	:	() -> ()
) {
	UIView.animateWithDuration	(	duration
	,	delay					:	delay
	,	usingSpringWithDamping	:	damping
	,	initialSpringVelocity	:	velocity
	,	options					:	options
	,	animations				:	animations
	,	completion				:	completion
	)
}

func
BlockAlert(
  _	title	:	String! = nil
, _	message	:	String! = nil
) -> UIAlertController {
	let	v = UIAlertController(
		title			:	title
	,	message			:	message
	,	preferredStyle	:	.Alert
	)

	UIApplication.sharedApplication().keyWindow!.rootViewController!.presentViewController(
		v
	,	animated	:	true
	,	completion	:	nil
	)

	return v
}

func
Alert(
  _	title		:	String! = nil
, _	message		:	String! = nil
, _ handler		:	( UIAlertAction! -> () )! = nil
) {
	let wAC = UIAlertController(
		title			:	title
	,	message			:	message
	,	preferredStyle	:	.Alert
	)
	wAC.addAction( UIAlertAction( title: "OK", style: .Cancel, handler: handler ) )
	UIApplication.sharedApplication().keyWindow!.rootViewController!.presentViewController(
		wAC
	,	animated	:	true
	,	completion	:	nil
	)
}

func
ErrorAlert( p: NSError ) {
	let wAC = UIAlertController(
		title			:	"\(p.domain):\(p.code)"
	,	message			:	p.localizedDescription
	,	preferredStyle	:	.Alert
	)
	wAC.addAction( UIAlertAction( title: "OK", style: .Cancel, handler: nil ) )
	UIApplication.sharedApplication().keyWindow!.rootViewController!.presentViewController(
		wAC
	,	animated	:	true
	,	completion	:	nil
	)
}

func
Confirmation(
  _	title		:	String! = nil
, _	message		:	String! = nil
,	handler		:	UIAlertAction! -> ()
) {
	let wAC = UIAlertController(
		title			:	title
	,	message			:	message
	,	preferredStyle	:	.Alert
	)
	wAC.addAction( UIAlertAction( title: "OK", style: .Default, handler: handler ) )
	wAC.addAction( UIAlertAction( title: "Cancel", style: .Cancel, handler: nil ) )
	UIApplication.sharedApplication().keyWindow!.rootViewController!.presentViewController(
		wAC
	,	animated	:	true
	,	completion	:	nil
	)
}

func
Image( path: String ) -> UIImage! {
	let w = NSData( contentsOfFile: path )
	return w != nil ? UIImage( data: w! ) : nil
}

class
V: UIView {
	var	draw: CGRect -> () = { p in }
	override func
	drawRect( p: CGRect ) {
		super.drawRect( p )
		draw( p )
	}
}

