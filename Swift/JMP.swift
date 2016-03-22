//	Written by Satoru Ogura, Tokyo.
//
import	Foundation
import	AddressBook
import	CoreGraphics

//func
//HexChar( p: Int ) -> Character {
//	return [ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f" ][ p & 0x0f ]
//}

//func
//HexString( p: NSData ) -> String {
//	let	wBytes = UnsafePointer<Int8>( p.bytes )
//	var	v: String = ""
//	for i in 0 ..< p.length {
//		v.append( HexChar( Int( wBytes[ i ] ) >> 4 ) )
//		v.append( HexChar( Int( wBytes[ i ] ) ) )
//	}
//	return v
//}

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

//func
//ToArray<T>( start: UnsafePointer<()>, count: Int ) -> [ T ] {
//	return Array( UnsafeBufferPointer( start: UnsafePointer<T>( start ), count: count ) )
//}
//	USAGE:	let wArray : [ Int16 ] = ToArray( data.bytes, data.length / sizeof( Int16 ) )

func
Data( p: NSURL ) -> NSData? {
	return NSData( contentsOfURL: p )
}

func
UTF8Length( p: String ) -> Int {
	return p.lengthOfBytesUsingEncoding( NSUTF8StringEncoding )
}

func
UTF8Data( p: String ) -> NSData? {
	return p.dataUsingEncoding( NSUTF8StringEncoding )
}

func
UTF8String( p: NSData ) -> String? {
	return String( data:p, encoding: NSUTF8StringEncoding )
}

func
UTF8String( p: UnsafePointer<UInt8>, length: Int ) -> String? {
	return String( bytes: p, length: length, encoding: NSUTF8StringEncoding )
}

func
Base64String( p: NSData, _ options: NSDataBase64EncodingOptions = [] ) -> String {
	return p.base64EncodedStringWithOptions( options )
}

func
Base64Data( p: String, _ options: NSDataBase64DecodingOptions = [] ) -> NSData? {
	return NSData( base64EncodedString: p, options: options )
}

func
EncodeJSON( p: AnyObject, _ options: NSJSONWritingOptions = [] ) throws -> NSData {
	return try NSJSONSerialization.dataWithJSONObject( p, options: options )
}

func
DecodeJSON( p: NSData, _ options: NSJSONReadingOptions = [] ) throws -> AnyObject {
	return try NSJSONSerialization.JSONObjectWithData( p, options: options )
}

func
IsNull( p: AnyObject? ) -> Bool {
	if p == nil { return true }
	return p is NSNull
}

func
AsInt( p: AnyObject? ) -> Int? {
	if let w = p where w is NSNumber || w is String {
		return w.integerValue
	}
	return nil
}

class
Reader< T > {
	var
	_unread : T?
	func
	_Read() -> T? { assert( false ); return nil }
	func
	Read() -> T? {
		if let v = _unread { _unread = nil; return v }
		return _Read()
	}
	func
	Unread( p: T ) { _unread = p; }
}

class
StdinUnicodeReader: Reader< UnicodeScalar > {
	var
	u	= String.UnicodeScalarView()
	override func
	_Read() -> UnicodeScalar? {
		while u.count == 0 {
			if let w = readLine( stripNewline: false ) { u = w.unicodeScalars } else {
				return nil
			}
		}
		let v = u.first
		u = u.dropFirst()
		return v
	}
}

class
StdinCharacterReader: Reader< Character > {
	var
	u	= String.CharacterView()
	override func
	_Read() -> Character? {
		while u.count == 0 {
			if let w = readLine( stripNewline: false ) { u = w.characters } else {
				return nil
			}
		}
		let v = u.first
		u = u.dropFirst()
		return v
	}
}

class
StringUnicodeReader	: Reader< UnicodeScalar > {
	var
	u	: String.UnicodeScalarView
	init( _ p: String ) { u = p.unicodeScalars }
	override func
	_Read() -> UnicodeScalar? {
		if u.count == 0 { return nil }
		let v = u.first
		u = u.dropFirst()
		return v
	}
}


class
StringCharacterReader: Reader< Character > {
	var
	u	: String.CharacterView
	init( _ p: String ) { u = p.characters }
	override func
	_Read() -> Character? {
		if u.count == 0 { return nil }
		let v = u.first
		u = u.dropFirst()
		return v
	}
}

class
Cell<T>	{
	var
	u			:	T
	let
	next		:	Cell?
	init(	_ p	:	T, _ pNext: Cell? = nil ) { u = p; next = pNext }
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
Sub( ed: () -> () ) {
	dispatch_async( dispatch_get_global_queue( DISPATCH_QUEUE_PRIORITY_DEFAULT, 0 ), ed )
}

func
Delay( p: NSTimeInterval, ed: () -> () ) -> NSTimer {
	return NSTimer.scheduledTimerWithTimeInterval(
		NSTimeInterval( p )
	,	target:	NSBlockOperation( block: { ed() } )
	,	selector: "main"
	,	userInfo: nil
	,	repeats: false
	)
}

func
Periodical( p: NSTimeInterval, ed: () -> () ) -> NSTimer {
	let v = NSTimer.scheduledTimerWithTimeInterval(
		NSTimeInterval( p )
	,	target:	NSBlockOperation( block: { ed() } )
	,	selector: "main"
	,	userInfo: nil
	,	repeats: true
	)
	return v
}

func
Delay( p: NSTimeInterval, _ queue: dispatch_queue_t, ed: () -> () ) {
	dispatch_after(
		dispatch_time( DISPATCH_TIME_NOW, Int64( p * NSTimeInterval( NSEC_PER_SEC ) ) )
	,	queue
	,	ed
	)
}

func
ResourcePath( resource: String, _ type: String = "" ) -> String? {
	return NSBundle.mainBundle().pathForResource( resource, ofType: type )
}

func
ResourceURL( resource: String, _ type: String = "" ) -> NSURL? {
	return NSBundle.mainBundle().URLForResource( resource, withExtension: type )
}

func
DocumentDirectoryURLs() -> [ NSURL ] {
	return NSFileManager.defaultManager().URLsForDirectory(
						.DocumentDirectory
	,	inDomains	:	.UserDomainMask
	) as [ NSURL ]
}

func
DocumentDirectoryPathes() -> [ String ] {
	return NSSearchPathForDirectoriesInDomains(
		.DocumentDirectory
	,	.UserDomainMask
	,	true
	) as [ String ]
}

func
Dist2( left: CGPoint, _ right: CGPoint ) -> Double {
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
	let	wBytes = UnsafePointer<UInt8>( p.bytes )
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
				wBalance = wBalance + 1
			case 0x5d, 0x7d:	//	]	}
				if wBalance == 0 { return nil }
				wBalance = wBalance - 1
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
		} catch {
		}
		data.replaceBytesInRange( wRange, withBytes: nil, length: 0 )
	}
}

func
HTML(
	uri		: String
, _	method	: String
, _	body	: NSData? = nil
, _	er		: ( NSError ) -> () = { e in }
, _	ex		: ( NSHTTPURLResponse, NSData ) -> () = { r, d in }
, _	ed		: NSData -> () = { p in }
) {
	let	wR = NSMutableURLRequest( URL: NSURL( string: uri )! )
	wR.HTTPMethod = method
	if body != nil { wR.HTTPBody = body! }
	NSURLSession.sharedSession().dataTaskWithRequest( wR ) { d, r, e in
		if let wE = e { er( wE ) }
		else {
			if let
				wR = r as? NSHTTPURLResponse
			,	wD = d {
				switch wR.statusCode {
				case 200:
					ed( wD )
				default:
					ex( wR, wD )
				}
			} else {
				assert( false )
			}
		}
	}
}

func
JSON(
	uri		: String
, _	method	: String
, _	json	: AnyObject? = nil
, _	er		: ( NSError ) -> () = { e in }
, _	ex		: ( NSHTTPURLResponse, NSData ) -> () = { r, d in }
, _	ed		: AnyObject -> () = { p in }
) {
	do {
		var	wBody	:	NSData?
		if let wJSON = json { wBody = try EncodeJSON( wJSON ) }
		HTML( uri, method, wBody, er, ex ) { p in
			do {
				ed( try DecodeJSON( p ) )
			} catch let e as NSError {
				er( e )
			} catch {
				assert( false )
			}
		}
	} catch let e as NSError {
		er( e )
	} catch {
		assert( false )
	}
}

typealias	JSONDict = [ String: AnyObject ]


func
ShowSharedCookies() {
	if let wCs = NSHTTPCookieStorage.sharedHTTPCookieStorage().cookies {
		for w in wCs { print( w ) }
	}
}

func
DeleteSharedCookies() {
	let	wCS = NSHTTPCookieStorage.sharedHTTPCookieStorage()
	if let wCs = wCS.cookies {
		for w in wCs { wCS.deleteCookie( w ) }
	}
}

func
Request( p: String ) -> NSURLRequest? {
	if let w = NSURL( string: p ) {
		return NSURLRequest( URL: w )
	} else {
		return nil
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
