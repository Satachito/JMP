import Foundation

class
JMPSession: NSObject, NSStreamDelegate {

	let	buffer			:	NSMutableData = NSMutableData()

	var	input			:	NSInputStream?
	var	output			:	NSOutputStream?

	var	openHandler		:	NSStream		-> ()
	var	inputHandler	:	NSInputStream	-> ()
	var	errorHandler	:	NSStream		-> ()
	var	endHandler		:	NSStream		-> ()

	deinit {
		if let wI = input {
			wI.close()
			wI.delegate = nil
			input = nil
		}
		if let wO = output {
			wO.close()
			wO.delegate = nil
			output = nil
		}
	}
	
	private func
	_Write() -> Int {
		var	v = 0
		if buffer.length > 0 {
			v = output!.write( UnsafePointer<UInt8>( buffer.bytes ), maxLength:buffer.length )
			if v > 0 {
				buffer.replaceBytesInRange( NSMakeRange( 0, v ), withBytes:nil, length:0 )
			}
		}
		return v
	}

	func
	Write( p: NSData ) {
		buffer.appendData( p )
		if output!.hasSpaceAvailable { _Write() }
	}

	init(
		host			:	String
	,	port			:	Int
	,	tlsMode			:	TLSMode?				= nil
	,	openHandler		:	NSStream		-> ()	= { _ in }
	,	errorHandler	:	NSStream		-> ()	= { _ in }
	,	endHandler		:	NSStream		-> ()	= { _ in }
	, 	inputHandler	:	NSInputStream	-> ()
	) {
		
		self.openHandler	=	openHandler
		self.inputHandler	=	inputHandler
		self.errorHandler	=	errorHandler
		self.endHandler		=	endHandler

		super.init()

		NSStream.getStreamsToHostWithName(
			host
		,	port: port
		,	inputStream: &input
		,	outputStream: &output
		)

		CFReadStreamSetProperty( input, kCFStreamPropertyShouldCloseNativeSocket, kCFBooleanTrue )
		CFWriteStreamSetProperty( output, kCFStreamPropertyShouldCloseNativeSocket, kCFBooleanTrue )

		input!.delegate = self
		CFReadStreamSetProperty(
			input
		,	kCFStreamPropertySSLSettings
		,	[	kCFStreamSSLLevel as String		:	NSStreamSocketSecurityLevelTLSv1
			,	kCFStreamSSLPeerName as String	:	host
			]
		)
		input!.scheduleInRunLoop( NSRunLoop.currentRunLoop(), forMode: NSDefaultRunLoopMode )
		input!.open()

		output!.delegate = self
		output!.scheduleInRunLoop( NSRunLoop.currentRunLoop(), forMode: NSDefaultRunLoopMode )
		output!.open()
	}

	func
	stream(
		theStream	:	NSStream
	,	handleEvent	:	NSStreamEvent
	) {
		switch handleEvent {
		case NSStreamEvent.None:
			break
		case NSStreamEvent.OpenCompleted:
			openHandler( theStream )
		case NSStreamEvent.HasBytesAvailable:
			inputHandler( input! )
		case NSStreamEvent.HasSpaceAvailable:
			_Write()
		case NSStreamEvent.ErrorOccurred:
			errorHandler( theStream )
		case NSStreamEvent.EndEncountered:
			endHandler( theStream )
		default:
			assert( false )
		}
	}
}

class
JMPHTTPSession {
	var	message		=	CFHTTPMessageCreateEmpty( nil, false ).takeRetainedValue()
	var	response	=	NSMutableData()
	var	data		=	NSMutableData()

	var	error		:	NSError? = nil
	
	var	session		:	JMPSession? = nil
	
	var	cookies		=	[ NSHTTPCookie ]()

	func
	Fetch(
		request	:	NSURLRequest
	,	tlsMode	:	TLSMode? = nil
	,	handler	:	() -> ()
	) {
		message	=	CFHTTPMessageCreateEmpty( nil, false ).takeRetainedValue()
		data	=	NSMutableData()

		error	=	nil
		
		var	wPort: Int = 0
		if request.URL!.port != nil {
			wPort = request.URL!.port!.integerValue
		} else {
			switch request.URL!.scheme {
			case "http"		:	wPort = 80
			case "https"	:	wPort = 443
			default			:	break
			}
		}
		
		session = JMPSession(
			host			:	request.URL!.host!
		,	port			:	wPort
		,	openHandler		:	{ p in
//				print( ( "openHandler", p ) )
			}
		,	errorHandler	:	{ p in
//				print( ( "errorHandler", p ) )
				self.error = p.streamError
			}
		,	endHandler		:	{ p in
//				print( ( "endHandler", p ) )
				handler()
			}
		) {	p in
		 	var w = [ UInt8 ]( count: 1024, repeatedValue: 0 )
			let	wRead = p.read( &w, maxLength: w.count )
			var	wIndex = 0
			while !CFHTTPMessageIsHeaderComplete( self.message ) && wIndex < wRead {
				if !CFHTTPMessageAppendBytes( self.message, &w[ wIndex++ ], 1 ) { assert( false ) }
			}
			if wIndex > 0 {
				let	w2 = w[ 0 ..< wIndex ]
				self.response.appendBytes( Array( w2 ), length: w2.count )
				let	w3 = w[ wIndex ..< wRead ]
				self.data.appendBytes( Array( w3 ), length: w3.count )
			} else {
				self.data.appendBytes( w, length: wRead )
			}
		}
		
		let	wRequest = CFHTTPMessageCreateRequest(
			nil
		,	request.HTTPMethod!
		,	request.URL!
		,	kCFHTTPVersion1_1
		).takeRetainedValue()

		if let wAllHTTPHeaderFields = request.allHTTPHeaderFields {
			for ( wKey, wValue ) in wAllHTTPHeaderFields {
				CFHTTPMessageSetHeaderFieldValue( wRequest, wKey, wValue )
			}
		}
		CFHTTPMessageSetHeaderFieldValue( wRequest, "Host", request.URL!.host! )
		CFHTTPMessageSetHeaderFieldValue( wRequest, "User-Agent", NSUserDefaults.standardUserDefaults().stringForKey( "UserAgent" ) )
		CFHTTPMessageSetHeaderFieldValue( wRequest, "Accept", "*/*" )
		CFHTTPMessageSetHeaderFieldValue( wRequest, "Connection", "close" )
		
		var wCStr = ""
		for w in cookies {
			wCStr = "\(wCStr)\(w.name)=\(w.value);"
		}
		CFHTTPMessageSetHeaderFieldValue( wRequest, "Cookie", wCStr )

		if let wBody = request.HTTPBody { CFHTTPMessageSetBody( wRequest, wBody ) }
		
		self.session!.Write( CFHTTPMessageCopySerializedMessage( wRequest )!.takeRetainedValue() )
//print( UTF8String( CFHTTPMessageCopySerializedMessage( wRequest )!.takeRetainedValue() )! )
	}
}
