import Foundation

enum
TLSMode: Int {
	case ValidateCertificate	= 1
	case BypassValidation		= 2
}

class
JMPStreamDelegate: NSObject, NSStreamDelegate {

	let	buffer			: NSMutableData = NSMutableData()

	var	inputStream		: NSInputStream
	var	outputStream	: NSOutputStream

	var	openHandler		: NSStream		-> ()
	var	inputHandler	: NSInputStream	-> ()
	var	errorHandler	: NSStream		-> ()
	var	endHandler		: NSInputStream	-> ()

	deinit {
		inputStream.close()
		outputStream.close()

		inputStream.delegate = nil
		outputStream.delegate = nil
	}

	func
	Setup( tlsMode: TLSMode? ) {
		CFReadStreamSetProperty( inputStream, kCFStreamPropertyShouldCloseNativeSocket, kCFBooleanTrue )
		CFWriteStreamSetProperty( outputStream, kCFStreamPropertyShouldCloseNativeSocket, kCFBooleanTrue )

		if let wTLSMode = tlsMode {
			CFWriteStreamSetProperty(
				outputStream
			,	kCFStreamSocketSecurityLevelNegotiatedSSL
			,	kCFStreamPropertySocketSecurityLevel
			)
			let wS: NSString = kCFStreamSSLValidatesCertificateChain	//	Sit outside the function below for XCode6.3Beta1
			CFWriteStreamSetProperty(
				outputStream
			,	kCFStreamPropertySSLSettings
			,	[ wS: wTLSMode == .ValidateCertificate ]
			)
		}

		inputStream.delegate = self
		outputStream.delegate = self

		inputStream.scheduleInRunLoop( NSRunLoop.currentRunLoop(), forMode: NSDefaultRunLoopMode )
		outputStream.scheduleInRunLoop( NSRunLoop.currentRunLoop(), forMode: NSDefaultRunLoopMode )

		inputStream.open()
		outputStream.open()
	}

	init(
		host			: String
	,	port			: Int
	,	tlsMode			: TLSMode?
	,	openHandler		: NSStream		-> ()
	,	inputHandler	: NSInputStream	-> ()
	,	errorHandler	: NSStream		-> ()
	,	endHandler		: NSInputStream	-> ()
	) {
		self.openHandler	= openHandler
		self.inputHandler	= inputHandler
		self.errorHandler	= errorHandler
		self.endHandler		= endHandler

		var inputStream		: NSInputStream?
		var	outputStream	: NSOutputStream?

		NSStream.getStreamsToHostWithName(
								host
		,	port			:	port
		,	inputStream		:	&inputStream
		,	outputStream	:	&outputStream
		)

		self.inputStream	= inputStream!
		self.outputStream	= outputStream!

		super.init()

		Setup( tlsMode )
	}

	init(
		socket			: CFSocketNativeHandle
	,	tlsMode			: TLSMode?
	,	openHandler		: NSStream		-> ()
	,	inputHandler	: NSInputStream	-> ()
	,	errorHandler	: NSStream		-> ()
	,	endHandler		: NSInputStream	-> ()
	) {
		self.openHandler	= openHandler
		self.inputHandler	= inputHandler
		self.errorHandler	= errorHandler
		self.endHandler		= endHandler

		var	readStream:		Unmanaged<CFReadStream>?
		var	writeStream:	Unmanaged<CFWriteStream>?

		CFStreamCreatePairWithSocket(
			nil
		,	socket
		,	&readStream
		,	&writeStream
		)

		self.inputStream	= readStream!.takeRetainedValue()
		self.outputStream	= writeStream!.takeRetainedValue()

		super.init()

		Setup( tlsMode )
	}

	private	func
	_Write() {
		if buffer.length != 0 {
			let	wLength = outputStream.write( UnsafePointer<UInt8>( buffer.bytes ), maxLength:buffer.length )
			if wLength > 0 { buffer.replaceBytesInRange( NSMakeRange( 0, wLength ), withBytes:nil, length:0 ) }
			//	If wLength is less than zero, do nothing. The error will be reported to stream:handleEvent:
		}
	}

	func
	Write( p: NSData ) {
		buffer.appendData( p )
		if outputStream.hasSpaceAvailable { _Write() }
	}

	func
	stream(
		theStream	:	NSStream
	,	handleEvent	:	NSStreamEvent
	) {
		switch handleEvent {
		case NSStreamEvent.HasBytesAvailable:
			inputHandler( inputStream )
		case NSStreamEvent.OpenCompleted:
			openHandler( theStream )
		case NSStreamEvent.HasSpaceAvailable:
			_Write()
		case NSStreamEvent.ErrorOccurred:
			errorHandler( theStream )
		case NSStreamEvent.EndEncountered:
			endHandler( inputStream )
		case NSStreamEvent.None:
			break
		default:
			assert( false )
		}
	}
}

