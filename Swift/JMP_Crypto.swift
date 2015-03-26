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

func
DataCryptedByAES( operation: CCOperation, p: NSData, key: NSData, _ options: CCOptions = CCOptions( kCCOptionPKCS7Padding ), _ iv: NSData? = nil ) -> ( CCCryptorStatus, NSData ) {
	var	wKeyLength = 0
	if		key.length < kCCKeySizeAES128	{ assert( false ) }
	else if	key.length < kCCKeySizeAES192	{ wKeyLength = kCCKeySizeAES128 }
	else if	key.length < kCCKeySizeAES256	{ wKeyLength = kCCKeySizeAES192 }
	else									{ wKeyLength = kCCKeySizeAES256 }

	if iv != nil { assert( iv!.length == kCCBlockSizeAES128 ) }

	var	wLength = UInt( ( ( p.length + kCCBlockSizeAES128 - 1 ) / kCCBlockSizeAES128 ) * kCCBlockSizeAES128 )
	let	v = NSMutableData( length: Int( wLength ) )!
	let	s: CCCryptorStatus = CCCrypt(
		operation
	,	CCAlgorithm( kCCAlgorithmAES )
	,	options
	,	key.bytes
	,	UInt( wKeyLength )
	,	iv != nil ? iv!.bytes : nil
	,	p.bytes
	,	UInt( p.length )
	,	v.mutableBytes
	,	wLength
	,	&wLength
	)
	v.length = Int( wLength )
	return ( s, v )
}

func
DataEncryptedByAES( p: NSData, key: NSData, _ options: CCOptions = CCOptions( kCCOptionPKCS7Padding ), _ iv: NSData? = nil ) -> ( CCCryptorStatus, NSData ) {
	return DataCryptedByAES( CCOperation( kCCEncrypt ), p, key, options, iv )
}

func
DataDecryptedByAES( p: NSData, key: NSData, _ options: CCOptions = CCOptions( kCCOptionPKCS7Padding ), _ iv: NSData? = nil ) -> ( CCCryptorStatus, NSData ) {
	return DataCryptedByAES( CCOperation( kCCDecrypt ), p, key, options, iv )
}

func
DataCryptedByBlowfish( operation: CCOperation, p: NSData, key: NSData, _ options: CCOptions = CCOptions( kCCOptionPKCS7Padding ), _ iv: NSData? = nil ) -> ( CCCryptorStatus, NSData ) {
	if iv != nil { assert( iv!.length == kCCBlockSizeBlowfish ) }

	var	wLength = UInt( ( ( p.length + kCCBlockSizeBlowfish - 1 ) / kCCBlockSizeBlowfish ) * kCCBlockSizeBlowfish )
	let	v = NSMutableData( length: Int( wLength ) )!
	let	s: CCCryptorStatus = CCCrypt(
		operation
	,	CCAlgorithm( kCCAlgorithmBlowfish )
	,	options
	,	key.bytes
	,	UInt( key.length )
	,	iv != nil ? iv!.bytes : nil
	,	p.bytes
	,	UInt( p.length )
	,	v.mutableBytes
	,	wLength
	,	&wLength
	)
	v.length = Int( wLength )
	return ( s, v )
}

func
DataEncryptedByBlowfish( p: NSData, key: NSData, _ options: CCOptions = CCOptions( kCCOptionPKCS7Padding ), _ iv: NSData? = nil ) -> ( CCCryptorStatus, NSData ) {
	return DataCryptedByBlowfish( CCOperation( kCCEncrypt ), p, key, options, iv )
}

func
DataDecryptedByBlowfish( p: NSData, key: NSData, _ options: CCOptions = CCOptions( kCCOptionPKCS7Padding ), _ iv: NSData? = nil ) -> ( CCCryptorStatus, NSData ) {
	return DataCryptedByBlowfish( CCOperation( kCCDecrypt ), p, key, options, iv )
}

