//	To use those functions below
//	You need to import CommonCrypto.h in your bridging header.
//		#import <CommonCrypto/CommonCrypto.h>

import	Foundation

func
SHA1( p: NSData ) -> NSData {
	var w = [ UInt8 ]( count: Int( CC_SHA1_DIGEST_LENGTH ), repeatedValue: 0 )
	CC_SHA1( p.bytes, CC_LONG( p.length ), &w )
	return NSData( bytes: w, length: Int( CC_SHA1_DIGEST_LENGTH ) )
}

