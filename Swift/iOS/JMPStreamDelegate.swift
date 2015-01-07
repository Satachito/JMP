import Foundation

class
JMPStreamDelegate: NSObject, NSStreamDelegate {

	let	queue			:	NSMutableData = NSMutableData()

	var	inputStream		:	NSInputStream
	var	outputStream	:	NSOutputStream

	var	openHandler		:	NSStream	-> ()
	var	dataHandler		:	NSData		-> ()
	var	errorHandler	:	NSStream	-> ()
	var	endHandler		:	NSStream	-> ()

	deinit {
		inputStream.close()
		outputStream.close()

		inputStream.delegate = nil
		outputStream.delegate = nil
	}

	func
	Setup( secure: Bool ) {
		CFReadStreamSetProperty( inputStream, kCFStreamPropertyShouldCloseNativeSocket, kCFBooleanTrue )
		CFWriteStreamSetProperty( outputStream, kCFStreamPropertyShouldCloseNativeSocket, kCFBooleanTrue )

		if secure {
			CFWriteStreamSetProperty(
				outputStream
			,	kCFStreamSocketSecurityLevelNegotiatedSSL
			,	kCFStreamPropertySocketSecurityLevel
			)

			CFWriteStreamSetProperty(
				outputStream
			,	kCFStreamPropertySSLSettings
			,	[ kCFStreamSSLValidatesCertificateChain as String: true ]	// false for ignoring
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
		host			:	String
	,	port			:	Int
	,	secure			:	Bool
	,	openHandler		:	NSStream	-> ()
	,	dataHandler		:	NSData		-> ()
	,	errorHandler	:	NSStream	-> ()
	,	endHandler		:	NSStream	-> ()
	) {
		self.openHandler	= openHandler
		self.dataHandler	= dataHandler
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

		Setup( secure )
	}

	init(
	_	socket			:	CFSocketNativeHandle
	,	secure			:	Bool
	,	openHandler		:	NSStream	-> ()
	,	dataHandler		:	NSData		-> ()
	,	errorHandler	:	NSStream	-> ()
	,	endHandler		:	NSStream	-> ()
	) {
		self.openHandler	= openHandler
		self.dataHandler	= dataHandler
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

		Setup( secure )
	}

	private	func
	_Write() {
		if queue.length != 0 {
			let	wLength = outputStream.write( UnsafePointer<UInt8>( queue.bytes ), maxLength:queue.length )
			if wLength > 0 { queue.replaceBytesInRange( NSMakeRange( 0, wLength ), withBytes:nil, length:0 ) }
			//	If wLength is less than zero, do nothing. The error will be reported to stream:handleEvent:
		}
	}

	func
	Write( p: NSData ) {
		queue.appendData( p )
		if outputStream.hasSpaceAvailable { _Write() }
	}

	func
	stream(
		stream	:	NSStream
	,	event	:	NSStreamEvent
	) {
		switch event {
		case NSStreamEvent.HasBytesAvailable:
			let	wData = NSMutableData( length: 4096 )!
			wData.length = ( stream as NSInputStream ).read( UnsafeMutablePointer<UInt8>( wData.mutableBytes ), maxLength:wData.length )
			dataHandler( wData )
		case NSStreamEvent.OpenCompleted:
			openHandler( stream )
		case NSStreamEvent.HasSpaceAvailable:
			_Write()
		case NSStreamEvent.ErrorOccurred:
			errorHandler( stream )
		case NSStreamEvent.EndEncountered:
			endHandler( stream )
		case NSStreamEvent.None:
			break
		default:
			assert( false )
		}
	}
}
//			var	wBuffer:	UnsafeMutablePointer<UnsafeMutablePointer<UInt8>> = nil
//			var	wLength:	UnsafeMutablePointer<Int> = nil
//			if inputStream.getBuffer( wBuffer, length: wLength ) { dataHandler( NSData( bytes:wBuffer.memory, length:wLength.memory ) ) }
