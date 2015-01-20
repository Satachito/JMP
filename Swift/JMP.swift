import	UIKit
import	AddressBook

func
HexChar( p: Int ) -> Character {
	return [ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f" ][ p & 0x0f ];
}

func
HexString( p: NSData ) -> String {
	let	wBytes = UnsafePointer<Int8>( p.bytes )
	var	v: String = ""
	for i in 0 ..< p.length {
		v.append( HexChar( Int( wBytes[ i ] ) >> 4 ) )
		v.append( HexChar( Int( wBytes[ i ] ) ) )
	}
	return v
}

func
RandomData( p: Int ) -> NSData {
	let wFD = open( "/dev/random", O_RDONLY )
	assert( wFD > 2 )
	var w = [ UInt8 ]( count: p, repeatedValue: 0 )
	read( wFD, &w, UInt( p ) )
	close( wFD )
	return NSData( bytes: w, length: p )
}

func
UTF8Data( p: String ) -> NSData? {
	return p.dataUsingEncoding( NSUTF8StringEncoding )
}

func
UTF8String( p: NSData ) -> String? {
	return NSString( data:p, encoding: NSUTF8StringEncoding )
}

func
UTF8String( p: UnsafePointer<UInt8>, length: Int ) -> String? {
	return NSString( bytes: p, length: length, encoding: NSUTF8StringEncoding )
}

func
Base64String( p: NSData, options: NSDataBase64EncodingOptions = nil ) -> String {
	return p.base64EncodedStringWithOptions( options )
}

func
Base64Data( p: String, options: NSDataBase64DecodingOptions = .IgnoreUnknownCharacters ) -> NSData? {
	return NSData( base64EncodedString: p, options: options )
}

func
EncodeJSON( p: AnyObject, options: NSJSONWritingOptions = nil, error: NSErrorPointer = nil ) -> NSData? {
	return NSJSONSerialization.dataWithJSONObject( p, options: options, error: error )
}

func
DecodeJSON( p: NSData, options: NSJSONReadingOptions = nil, error: NSErrorPointer = nil ) -> AnyObject? {
	return NSJSONSerialization.JSONObjectWithData( p, options: options, error: error )
}

func
IsNull( p: AnyObject? ) -> Bool {
	if p == nil { return true }
	return p is NSNull
}

func
Integer( p: AnyObject? ) -> Int {
	if p == nil { return 0 }
	if p is NSNumber { return ( p as NSNumber ).integerValue }
	if p is String { return ( p as NSString ).integerValue }
	return 0
}

func
Dist2( left: CGPoint, right: CGPoint ) -> Double {
	let w = Double( right.x - left.x )
	let h = Double( right.y - left.y )
	return w * w + h * h
}

func
Notification( name: String, p: ( NSNotification! -> () )!, queue: NSOperationQueue! = nil ) -> NSObjectProtocol! {
	return NSNotificationCenter.defaultCenter().addObserverForName(
		name
	,	object				:	nil
	,	queue				:	queue
	,	usingBlock			:	p
	)
}


////////////////////////////////////////////////////////////////	iOS

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
	wAC.addTextFieldWithConfigurationHandler() { p in
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
Animation(
	animations	:	() -> ()
,	duration	:	NSTimeInterval			= 0.25
,	completion	:	( Bool -> () )!			= nil
,	delay		:	NSTimeInterval			= 0
,	options		:	UIViewAnimationOptions	= .CurveEaseInOut
) {
	UIView.animateWithDuration	(	duration
	,	delay					:	delay
	,	options					:	options
	,	animations				:	animations
	,	completion				:	completion
	)
}

func
SpringAnimation(
	animations	:	() -> ()
,	duration	:	NSTimeInterval			= 0.25
,	completion	:	( Bool -> () )!			= nil
,	delay		:	NSTimeInterval			= 0
,	options		:	UIViewAnimationOptions	= .CurveEaseInOut
,	damping		:	CGFloat					= 0.5
,	velocity	:	CGFloat					= 0.1
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
) {
	UIApplication.sharedApplication().keyWindow!.rootViewController!.presentViewController(
		UIAlertController(
			title			:	title
		,	message			:	message
		,	preferredStyle	:	.Alert
		)
	,	animated	:	true
	,	completion	:	nil
	)
}

func
Alert(
  _	title		:	String! = nil
, _	message		:	String! = nil
, _ handler		:	() -> () = {}
) {
	let wAC = UIAlertController(
		title			:	title
	,	message			:	message
	,	preferredStyle	:	.Alert
	)
	wAC.addAction( UIAlertAction( title: "OK", style: .Cancel, handler: { p in handler() } ) )
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
, _ handler		:	() -> () = {}
) {
	let wAC = UIAlertController(
		title			:	title
	,	message			:	message
	,	preferredStyle	:	.Alert
	)
	wAC.addAction( UIAlertAction( title: "OK", style: .Default, handler: { p in handler() } ) )
	wAC.addAction( UIAlertAction( title: "Cancel", style: .Cancel, handler: nil ) )
	UIApplication.sharedApplication().keyWindow!.rootViewController!.presentViewController(
		wAC
	,	animated	:	true
	,	completion	:	nil
	)
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

/*
class
JMPSAddressBook : NSObject {
	var	u	:	Unmanaged<ABAddressBookRef>! = nil

//	deinit {
//		if u { CFRelease( u.takeUnretainedValue() ) }
//	}


	func
	grant(
		granted	:	( JMPSAddressBook -> () )!
	,	denied	:	( () -> () )!
	,	error	:	( ( NSError! ) -> () )!
	) {
		switch ( ABAddressBookGetAuthorizationStatus() )
		{
		case	.Restricted,	.Denied:
			denied();
		case	.Authorized:
			var e :  UnsafePointer<Unmanaged<CFError>?> = nil
			u = ABAddressBookCreateWithOptions( nil, e )
			if e { error( e.owner as NSError ) }
			if u { granted( self ) }
		case	.NotDetermined:
			var e :  CMutablePointer<Unmanaged<CFError>?> = nil
			u = ABAddressBookCreateWithOptions( nil, e )
			if e { error( e.owner as NSError ) }
			if u {
				ABAddressBookRequestAccessWithCompletion(
					nil
				,	{	( qGranted: Bool , qError: CFError!  ) in
						dispatch_async(
							dispatch_get_main_queue()
						,	{	if qGranted { granted( self ) }
								else {
									if qError	{ error( e.owner as NSError ) }
									else		{ denied() }
								}
							}
						)
					}
				);
			}
		}
	}

	func
	granted(
		granted		: ( ( JMPSAddressBook ) -> Void )!
	) {
		grant(
			granted
		,	{	BlockAlert(
					"設定であなたの連絡帳にアクセスする権限を与えてください。"
				,	"Permission was not granted for Contacts."
				)
			}
		,	{	( p: NSError! ) in
				BlockAlert(
					p.description
				,	"Unknown Error"
				)
			}
		)
	}
}
*/