import Foundation

enum
TLSMode: Int {
	case ValidateCertificate	=	1
	case BypassValidation		=	2
}

class
JMPStreamSession: NSObject, NSStreamDelegate {

	let	buffer			:	NSMutableData = NSMutableData()

	var	inputStream		:	NSInputStream
	var	outputStream	:	NSOutputStream

	var	openHandler		:	NSStream		-> ()
	var	inputHandler	:	NSInputStream	-> ()
	var	errorHandler	:	NSStream		-> ()
	var	endHandler		:	NSStream		-> ()

	private func
	_Write() -> Int {
		var	v = 0
		if buffer.length > 0 {
			v = outputStream.write( UnsafePointer<UInt8>( buffer.bytes ), maxLength:buffer.length )
			if v > 0 {
				buffer.replaceBytesInRange( NSMakeRange( 0, v ), withBytes:nil, length:0 )
			}
		}
		return v
	}

	func
	Write( p: NSData ) {
		buffer.appendData( p )
		if outputStream.hasSpaceAvailable { _Write() }
	}

	var	uClosed = false
	func
	Close() {
		inputStream.close()
//		while buffer.length > 0 {
//			if _Write() < 0 { break }
//		}
		outputStream.close()

		inputStream.delegate = nil
		outputStream.delegate = nil

		uClosed = true
	}
	deinit {
//println( "deinit" )
		if !uClosed { Close() }
	}

	func
	Setup( tlsMode: TLSMode? ) {
		CFReadStreamSetProperty( inputStream, kCFStreamPropertyShouldCloseNativeSocket, kCFBooleanTrue )
		CFWriteStreamSetProperty( outputStream, kCFStreamPropertyShouldCloseNativeSocket, kCFBooleanTrue )

CFReadStreamSetProperty(inputStream, kCFStreamPropertySocketSecurityLevel, kCFStreamSocketSecurityLevelNegotiatedSSL);
CFWriteStreamSetProperty(outputStream, kCFStreamPropertySocketSecurityLevel, kCFStreamSocketSecurityLevelNegotiatedSSL);
/*
		if let wTLSMode = tlsMode {
			CFWriteStreamSetProperty(
				outputStream
			,	kCFStreamSocketSecurityLevelNegotiatedSSL
			,	kCFStreamPropertySocketSecurityLevel
			)
			CFWriteStreamSetProperty(
				outputStream
			,	kCFStreamPropertySSLSettings
			,	[ kCFStreamSSLValidatesCertificateChain as String: kCFBooleanTrue ]	//	wTLSMode == .ValidateCertificate ]
			)
		}
*/
		inputStream.delegate = self
		outputStream.delegate = self

		inputStream.scheduleInRunLoop( NSRunLoop.currentRunLoop(), forMode: NSDefaultRunLoopMode )
		outputStream.scheduleInRunLoop( NSRunLoop.currentRunLoop(), forMode: NSDefaultRunLoopMode )

//println( "Opening" )
		inputStream.open()
		outputStream.open()
	}

	init(
		host			:	String
	,	port			:	Int
	,	tlsMode			:	TLSMode? = nil
	,	openHandler		:	NSStream		-> () = { _ in }
	,	errorHandler	:	NSStream		-> () = { _ in }
	,	endHandler		:	NSStream		-> () = { _ in }
	, 	inputHandler	:	NSInputStream	-> ()
	) {
		self.openHandler	=	openHandler
		self.inputHandler	=	inputHandler
		self.errorHandler	=	errorHandler
		self.endHandler		=	endHandler

		var inputStream		:	NSInputStream?
		var	outputStream	:	NSOutputStream?

		NSStream.getStreamsToHostWithName(
								host
		,	port			:	port
		,	inputStream		:	&inputStream
		,	outputStream	:	&outputStream
		)

		self.inputStream	=	inputStream!
		self.outputStream	=	outputStream!

		super.init()

		Setup( tlsMode )
	}

	init(
		socket			:	CFSocketNativeHandle
	, _	tlsMode			:	TLSMode?
	, _	openHandler		:	NSStream		-> ()
	, _	errorHandler	:	NSStream		-> ()
	, _	endHandler		:	NSStream		-> ()
	,	inputHandler	:	NSInputStream	-> ()
	) {
		self.openHandler	=	openHandler
		self.inputHandler	=	inputHandler
		self.errorHandler	=	errorHandler
		self.endHandler		=	endHandler

		var	readStream	:	Unmanaged<CFReadStream>?
		var	writeStream	:	Unmanaged<CFWriteStream>?

		CFStreamCreatePairWithSocket(
			nil
		,	socket
		,	&readStream
		,	&writeStream
		)

		self.inputStream	=	readStream!.takeRetainedValue()
		self.outputStream	=	writeStream!.takeRetainedValue()

		super.init()

		Setup( tlsMode )
	}

	func
	stream(
		theStream	:	NSStream
	,	handleEvent	:	NSStreamEvent
	) {
		switch handleEvent {
		case NSStreamEvent.HasBytesAvailable:
			assert( theStream == inputStream )
			inputHandler( inputStream )
		case NSStreamEvent.OpenCompleted:
			openHandler( theStream )
		case NSStreamEvent.HasSpaceAvailable:
			_Write()
		case NSStreamEvent.ErrorOccurred:
			errorHandler( theStream )
		case NSStreamEvent.EndEncountered:
			endHandler( theStream )
		case NSStreamEvent.None:
			break
		default:
			assert( false )
		}
	}
}

