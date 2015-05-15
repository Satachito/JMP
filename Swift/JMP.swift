import	Foundation
import	AddressBook
import	CoreGraphics

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
	read( wFD, &w, Int( p ) )
	close( wFD )
	return NSData( bytes: w, length: p )
}

func
RandomIndices( p: Int ) -> [ Int ] {
	var	v = [ Int ]()
	for i in 0 ..< p { v.append( i ) }
	for i in 0 ..< p {
		let j = Int( arc4random_uniform( UInt32( p - i ) ) ) + i
		( v[ i ], v[ j ] ) = ( v[ j ], v[ i ] )
	}
	return v
}

func
UTF8Data( p: String ) -> NSData? {
	return p.dataUsingEncoding( NSUTF8StringEncoding )
}

func
UTF8String( p: NSData ) -> String? {
	return NSString( data:p, encoding: NSUTF8StringEncoding ) as? String
}

func
UTF8String( p: UnsafePointer<UInt8>, length: Int ) -> String? {
	return NSString( bytes: p, length: length, encoding: NSUTF8StringEncoding ) as? String
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
AsInt( p: AnyObject? ) -> Int? {
	if p is NSNumber { return ( p as! NSNumber ).integerValue }
	if p is String { return ( p as! NSString ).integerValue }
	return nil;
}

func
Notify( name: String, ed: NSNotification! -> () ) -> NSObjectProtocol {
	return NSNotificationCenter.defaultCenter().addObserverForName(
		name
	,	object				:	nil
	,	queue				:	nil
	,	usingBlock			:	ed
	)
}

func
Main( ed: () -> () ) {
	dispatch_async( dispatch_get_main_queue(), ed )
}

func
DocumentDirectoryURLs() -> [ NSURL ] {
	return NSFileManager.defaultManager().URLsForDirectory(
		.DocumentDirectory
	,	inDomains:.UserDomainMask
	) as! [ NSURL ]
}

func
DocumentDirectoryPathes() -> [ String ] {
	return NSSearchPathForDirectoriesInDomains(
		.DocumentDirectory
	,	.UserDomainMask
	,	true
	) as! [ String ]
}

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