import UIKit

func
FrameX( p: UIView, _ x: CGFloat ) {
	var	w = p.frame
	w.origin.x = x
	p.frame = w
}

func
FrameY( p: UIView, _ y: CGFloat ) {
	var	w = p.frame
	w.origin.y = y
	p.frame = w
}


func
InputBox(
	title		:	String
, _	message		:	String? = nil
, _	config		:	UITextField -> () = { _ in }
,	ed			:	UITextField -> ()
) {
	let wAC = UIAlertController(
		title			:	title
	,	message			:	message
	,	preferredStyle	:	.Alert
	)
	var wTF : UITextField!
	wAC.addTextFieldWithConfigurationHandler { p in
		config( p )
		wTF = p
	}
	wAC.addAction( UIAlertAction( title: "Cancel", style: .Cancel, handler:nil ) )
	wAC.addAction( UIAlertAction( title: "OK", style: .Default ) { _ in ed( wTF ) } )
	wAC.view.setNeedsLayout()
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
	title	:	String? = nil
, _	message	:	String? = nil
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
	title		:	String? = nil
, _	message		:	String? = nil
, _	handler		:	UIAlertAction -> () = { _ in }
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
, _	handler		:	UIAlertAction! -> () = { _ in }
) {
	Alert( "\(p.domain):\(p.code)", p.localizedDescription, handler )
}

func
HTMLAlert(
	r			:	NSHTTPURLResponse
, _	d			:	NSData
, _	handler		:	UIAlertAction! -> () = { _ in }
 ) {
	Alert( r.description, nil, handler )
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

func
HTML_iOS(
	uri		: String
, _	method	: String
, _	body	: NSData? = nil
, _	er		: ( NSError ) -> () = { e in ErrorAlert( e ) }
, _	ex		: ( NSHTTPURLResponse, NSData ) -> () = { r, d in HTMLAlert( r, d ) }
, _	ed		: NSData -> () = { p in }
) {
	HTML( uri, method, body, er, ex, ed )
}

func
JSON_iOS(
	uri		: String
, _	method	: String
, _	json	: AnyObject? = nil
, _	er		: ( NSError ) -> () = { e in ErrorAlert( e ) }
, _	ex		: ( NSHTTPURLResponse, NSData ) -> () = { r, d in HTMLAlert( r, d ) }
, _	ed		: AnyObject -> ()
) {
	JSON( uri, method, json, er, ex, ed )
}

func
Image_iOS(
	uri	: String
, _	er	: ( NSError ) -> () = { e in ErrorAlert( e ) }
, _	ex	: ( NSHTTPURLResponse, NSData ) -> () = { r, d in HTMLAlert( r, d ) }
, _	ed	: UIImage -> ()
) {
	HTML( uri, "GET", nil, er, ex ) { p in
		if let wImage = UIImage( data: p ) {
			ed( wImage )
		} else {
			assert( false )
		}
	}
}

class
ImageV	:	UIImageView {
	var
	aiStyle	=	UIActivityIndicatorViewStyle.WhiteLarge
	let
	label	=	UILabel()
	
	func
	ShowMessage( p: String ) {
		label.text = p
		label.numberOfLines = 0
		label.frame = bounds
		addSubview( label )
	}
	
	var
	uri		:	String? {
		didSet {
			label.removeFromSuperview()
			let	wAIV = UIActivityIndicatorView( activityIndicatorStyle: aiStyle )
			addSubview( wAIV )
			wAIV.center = Center( bounds )
			wAIV.startAnimating()
			Image_iOS(
				uri!
			,	{	e in
					wAIV.removeFromSuperview()
					self.ShowMessage( e.description )
				}
			,	{	r, d in
					wAIV.removeFromSuperview()
					self.ShowMessage( r.description )
				}
			) { p in
				wAIV.removeFromSuperview()
				self.image = p
			}
		}
	}
}


