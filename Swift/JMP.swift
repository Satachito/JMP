import	Foundation
import	AddressBook
import	CoreGraphics

func
HexChar( p: Int ) -> Character {
	return [ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f" ][ p & 0x0f ]
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
Base64String( p: NSData, options: NSDataBase64EncodingOptions = NSDataBase64EncodingOptions( rawValue: 0 ) ) -> String {
	return p.base64EncodedStringWithOptions( options )
}

func
Base64Data( p: String, options: NSDataBase64DecodingOptions = NSDataBase64DecodingOptions( rawValue: 0 ) ) -> NSData? {
	return NSData( base64EncodedString: p, options: options )
}

func
EncodeJSON( p: AnyObject, options: NSJSONWritingOptions = NSJSONWritingOptions( rawValue: 0 ) ) throws -> NSData {
	return try NSJSONSerialization.dataWithJSONObject( p, options: options )
}

func
DecodeJSON( p: NSData, options: NSJSONReadingOptions = NSJSONReadingOptions( rawValue: 0 ) ) throws -> AnyObject {
	return try NSJSONSerialization.JSONObjectWithData( p, options: options )
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
	return nil
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
ResourcePath( resource: String, _ type: String = "" ) -> String? {
	return NSBundle.mainBundle().pathForResource( resource, ofType: type )
}

var
DocumentDirectoryURLs = NSFileManager.defaultManager().URLsForDirectory(
	.DocumentDirectory
,	inDomains:.UserDomainMask
) as [ NSURL ]

var
DocumentDirectoryPathes = NSSearchPathForDirectoriesInDomains(
	.DocumentDirectory
,	.UserDomainMask
,	true
) as [ String ]

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
BalancedPosition( p: NSData ) -> Int? {
	var	wBytes = UnsafePointer<UInt8>( p.bytes )
	var	wBalance = 0
	var	wInString = false
	var	wInBackSlash = false

	for i in 0 ..< p.length {
		if wInString {
			if wInBackSlash {
				wInBackSlash = false
			} else {
				switch wBytes[ i ] {
				case 0x5c:	//	\
					wInBackSlash = true
				case 0x22:
					wInString = false
				default:
					break
				}
			}
		} else {
			switch wBytes[ i ] {
			case 0x5b, 0x7b:	//	[	{
				wBalance++
			case 0x5d, 0x7d:	//	]	}
				if wBalance-- == 0 { return nil }
				if wBalance == 0 { return i + 1 }
			case 0x22:
				wInString = true
				wInBackSlash = false
			default:
				break
			}
		}
	}
	return nil
}

func
JSONForAll( data: NSMutableData, _ p: AnyObject -> () ) {
	while let wBP = BalancedPosition( data ) {
		let	wRange = NSMakeRange( 0, wBP )
		do {
			p( try DecodeJSON( data.subdataWithRange( wRange ) ) )
		} catch _ {
		}
		data.replaceBytesInRange( wRange, withBytes: nil, length: 0 )
	}
}

typealias	JSONDict = [ String: AnyObject ]

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