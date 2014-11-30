import	UIKit
import	AddressBook

func
InputBox(
	title		:	String!
,	message		:	String! = nil
,	config		:	((UITextField!) -> Void)! = nil
,	ok			:	((UITextField!) -> Void)
) -> UIAlertController {
	var v = UIAlertController(
		title			:	title
	,	message			:	message
	,	preferredStyle	:	.Alert
	)
	var wTF : UITextField!
	v.addTextFieldWithConfigurationHandler() { p in
		if config != nil { config( p ) }
		wTF = p
	}
	v.addAction( UIAlertAction( title: "Cancel", style: .Cancel, handler:nil ) )
	v.addAction( UIAlertAction( title: "OK", style: .Default ) { _ in ok( wTF ) } )
	return v
}
	
func
Animation(
	animations	:	( () -> Void )
,	duration	:	NSTimeInterval			= 0.25
,	completion	:	( ( Bool ) -> Void )!	= nil
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
	animations	:	( () -> Void )
,	duration	:	NSTimeInterval			= 0.25
,	completion	:	( ( Bool ) -> Void )!	= nil
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
,	_	message	:	String! = nil
) {
	UIApplication.sharedApplication().keyWindow!.rootViewController!.presentViewController(
		UIAlertController(
			title			:	title
		,	message			:	message
		,	preferredStyle	:	.Alert
		)
	,	animated:true
	,	completion:nil
	)
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
		granted	:	( ( JMPSAddressBook ) -> Void )!
	,	denied	:	( () -> Void )!
	,	error	:	( ( NSError! ) -> Void )!
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