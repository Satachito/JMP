import UIKit

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
	title	:	String! = nil
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
	title		:	String! = nil
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
ErrorAlert(
	p			:	NSError
, _ handler		:	( UIAlertAction! -> () )! = nil
) {
	Alert( "\(p.domain):\(p.code)", p.localizedDescription, handler )
}

func
Confirmation(
	title		:	String! = nil
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
	var	draw: CGRect -> () = { _ in }
	override func
	drawRect( p: CGRect ) {
		super.drawRect( p )
		draw( p )
	}
}

func
BoundingRectWithWidth( p: NSAttributedString, width: CGFloat ) -> CGRect {
	return p.boundingRectWithSize(
		CGSizeMake( width, .max )
	,	options	:	.UsesLineFragmentOrigin
	,	context	:	nil
	)
}

func
AdjustHeight( p: UITextView ) {
	var wRect = BoundingRectWithWidth(
		p.attributedText
	,	width: p.bounds.size.width - p.textContainerInset.left - p.textContainerInset.right - p.textContainer.lineFragmentPadding * 2
	)
	wRect.size.width += p.textContainerInset.left + p.textContainerInset.right + p.textContainer.lineFragmentPadding * 2
	wRect.size.height += p.textContainerInset.top + p.textContainerInset.bottom
	p.bounds = wRect
}


