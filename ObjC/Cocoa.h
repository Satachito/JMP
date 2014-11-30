#pragma once
//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

#import		<CommonCrypto/CommonCrypto.h>
#import		<QuartzCore/QuartzCore.h>
#import		<AddressBook/AddressBook.h>

#define	Dump( p )	NSLog( @"%s:%@", #p, p )

#include <sys/types.h>
#include <sys/sysctl.h>

#include	"Apple.h"

namespace
JpMooParaiso
{	namespace
	Apple
	{
		inline	NSString*
		HwMachine()
		{	char w[ 256 ];	//	If there is a day when this buffer will be insufficient, I will be needed to be adjust then.
			size_t	wSize = sizeof( w );
  			sysctlbyname( "hw.machine", w, &wSize, NULL, 0 );
			return [ NSString stringWithCString:w encoding:NSUTF8StringEncoding ];
		}

#if	TARGET_OS_IPHONE
		inline	void
		SimpleAlert( NSString* pMessage, NSString* pTitle = nil, NSString* pCancelButtonTitle = @"OK" )
		{	[	[	[ UIAlertView alloc ]
					initWithTitle:pTitle
					message:pMessage
					delegate:nil
					cancelButtonTitle:pCancelButtonTitle
					otherButtonTitles:nil
				]
				show
			];
		}
		inline	void
		BlockAlert( NSString* pMessage, NSString* pTitle = @"" )
		{	[	[	[ UIAlertView alloc ]
					initWithTitle:pTitle
					message:pMessage
					delegate:nil
					cancelButtonTitle:nil
					otherButtonTitles:nil
				]
				show
			];
		}
		inline	void
		Transit( CALayer* pLayer, NSString* pType = kCATransitionFade, NSString* pSubtype = nil )
		{	CATransition* wT = [ [ CATransition alloc ] init ];
			wT.type = pType;
			wT.subtype = pSubtype;
			[ pLayer addAnimation:wT forKey:pType ];
		}

		inline	UIImage*
		ToImage( NSData* p, CGFloat pScale = 2 )
		{	return [ UIImage imageWithData:p scale:pScale ];
		}

		inline	UIColor*
		ToColor( UIImage* p )
		{	return [ UIColor colorWithPatternImage:p ];
		}

		inline	NSURLSessionTask*
		DataTask
		(	NSURL*	p
		,	void	(^pCompletion)()
		,	void	(^pDataHandler)( NSData* )
		,	void	(^pErrorHandler)( NSError* )
		,	void	(^pResponseHandler)( NSURLResponse* )
		)
		{	return
			[	NSURLSession.sharedSession
				dataTaskWithURL:p
				completionHandler:^( NSData* q, NSURLResponse* qResponse, NSError* qError )
				{	dispatch_async
					(	dispatch_get_main_queue()
					,	^{
							try
							{
								if ( qError )
								{	pErrorHandler( qError );
								}
								else
								{	if
									(	[ qResponse isKindOfClass:NSHTTPURLResponse.class ]
									&&	((NSHTTPURLResponse*)qResponse).statusCode == 200
									)
									{	pDataHandler( q );
									}
									else
									{	pResponseHandler( qResponse );
									}
								}
							}
							catch (...)
							{	pCompletion();
								throw;
							}
							pCompletion();
						}
					);
				}
			];
		}
#endif
		inline	NSString*
		ToString( NSData* p )
		{	return [ [ NSString alloc ] initWithData:p encoding:NSUTF8StringEncoding ];
		}

		inline	NSString*
		ToString( void* p, NSUInteger pLength )
		{	return [ [ NSString alloc ] initWithBytes:p length:pLength encoding:NSUTF8StringEncoding ];
		}

		inline	NSData*
		ToData( NSString* p )
		{	return [ p dataUsingEncoding:NSUTF8StringEncoding ];
		}

		inline	NSData*
		ToData( void* p, NSUInteger pLength )
		{	return [ NSData dataWithBytes:p length:pLength ];
		}

		inline	NSArray*
		Tokenize( NSString* p, NSCharacterSet* pCS = NSCharacterSet.whitespaceCharacterSet )
		{	NSMutableArray* v = NSMutableArray.array;
			for ( NSString* w in [ p componentsSeparatedByCharactersInSet:pCS ] ) if ( w.length ) [ v addObject:w ];
			return v;
		}

		inline	NSString*
		Base64Encode( NSData* p )
		{	size_t	wNumBits = p.length * 8;
			char	wBuffer[ ( wNumBits + 23 ) / 24 * 4 ];
			size_t	wNumDataBytes = ( wNumBits + 5 ) / 6;

			for ( size_t i = 0; i < wNumDataBytes; i++ ) wBuffer[ i ] = 0;
			for ( size_t i = wNumDataBytes; i < sizeof( wBuffer ); i++ ) wBuffer[ i ] = '=';

			for ( size_t i = 0; i < wNumBits; i++ )
			{	if ( ((unsigned char*)p.bytes)[ i / 8 ] & ( 0x80 >> ( i % 8 ) ) )
				{	wBuffer[ i / 6 ] |= 0x20 >> ( i % 6 );
				}
			}
			for ( size_t i = 0; i < wNumDataBytes; i++ )
			{	wBuffer[ i ] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[ wBuffer[ i ] ];
			}
			return ToString( wBuffer, sizeof( wBuffer ) );
		}

		inline	NSData*
		SHA1( NSData* p )
		{	uint8_t w[ CC_SHA1_DIGEST_LENGTH ];

			CC_SHA1( p.bytes, (CC_LONG)p.length, w );
			return ToData( w, sizeof( w ) );
		}

		inline	NSString*
		DocumentDirPath()
		{	return NSSearchPathForDirectoriesInDomains
			(	NSDocumentDirectory
			,	NSUserDomainMask
			,	YES
			).lastObject;
		}

		inline	NSURL*
		DocumentDirURL()
		{	return
			[	NSFileManager.defaultManager
				URLsForDirectory:NSDocumentDirectory
				inDomains:NSUserDomainMask
			].lastObject;
		}

		inline	NSURL*
		ToURL( NSString* p )
		{	return [ NSURL URLWithString:p ];
		}

		inline	NSURLRequest*
		ToURLRequest( NSURL* p )
		{	return [ NSURLRequest requestWithURL:p ];
		}

		inline	NSURLRequest*
		ToURLRequest( NSString* p )
		{	return ToURLRequest( ToURL( p ) );
		}

		inline	NSMutableURLRequest*
		ToMutableURLRequest( NSURL* p )
		{	return [ NSMutableURLRequest requestWithURL:p ];
		}

		inline	NSMutableURLRequest*
		ToMutableURLRequest( NSString* p )
		{	return ToMutableURLRequest( ToURL( p ) );
		}

		inline	NSMutableURLRequest*
		ToMutableURLRequest( CFHTTPMessageRef p )
		{	NSMutableURLRequest* v = [ NSMutableURLRequest requestWithURL:CFBridgingRelease( CFHTTPMessageCopyRequestURL( p ) ) ];
			v.HTTPMethod = CFBridgingRelease( CFHTTPMessageCopyRequestMethod( p ) );
			v.allHTTPHeaderFields = CFBridgingRelease( CFHTTPMessageCopyAllHeaderFields( p ) );
			v.HTTPBody = CFBridgingRelease( CFHTTPMessageCopyBody( p ) );
			return v;
		}

		inline	NSData*
		ToData( NSURL* p )
		{	return [ NSData dataWithContentsOfURL:p ];
		}

		inline	NSTimer*
		ToTimer( NSTimeInterval pTI, void (^pBlock)( void ), BOOL pRepeats = YES )
		{	return
			[	NSTimer
				scheduledTimerWithTimeInterval:pTI
				target:[ NSBlockOperation blockOperationWithBlock:pBlock ]
				selector:@selector(main)
				userInfo:nil
				repeats:pRepeats
			];
		}

		inline	BOOL
		FileExists( NSString* p )
		{	return [ NSFileManager.defaultManager fileExistsAtPath:p ];
		}

		inline	NSDictionary*
		ToJSONObject( NSData* p, NSJSONReadingOptions pOptions = 0 )
		{	NSError* wError;
			NSDictionary* v = [ NSJSONSerialization JSONObjectWithData:p options:pOptions error:&wError ];
			if ( ! v ) throw wError;
			return v;
		}
		
#pragma	mark	----------------------------------------------------------------	Animation
		
		inline	void
		Animation
		(	void					(^p)()
		,	NSTimeInterval			pDuration = .2
		,	void					(^pCompletion)( BOOL ) = nil
		,	NSTimeInterval			pDelay = 0
		,	UIViewAnimationOptions	pOptions = 0
		)
		{	[	UIView
				animateWithDuration:pDuration
				delay:pDelay
				options:pOptions
				animations:p
				completion:pCompletion
			];
		}

		inline	void
		SpringAnimation
		(	void					(^p)()
		,	NSTimeInterval			pDuration = .2
		,	void					(^pCompletion)( BOOL ) = nil
		,	NSTimeInterval			pDelay = 0
		,	CGFloat					pDamping = .5
		,	CGFloat					pInitialSpringVelocity = .1
		,	UIViewAnimationOptions	pOptions = 0
		)
		{	[	UIView
				animateWithDuration:pDuration
				delay:pDelay
				usingSpringWithDamping:pDamping
				initialSpringVelocity:pInitialSpringVelocity
				options:pOptions
				animations:p
				completion:pCompletion
			];
		}

#pragma	mark	----------------------------------------------------------------	AddressBook

		inline	NSString*
		LocalizedLabel( CFStringRef p )
		{	return (__bridge_transfer NSString*)ABAddressBookCopyLocalizedLabel( p );
		}

		inline	NSMutableArray*
		AttributeArray( ABMultiValueRef p )
		{	NSMutableArray* v = NSMutableArray.array;
			for ( CFIndex i = 0; i < ABMultiValueGetCount( p ); i++ )
			{	NSString* wValue = (__bridge_transfer NSString*)ABMultiValueCopyValueAtIndex( p, i );
				NSString* wLabel = LocalizedLabel( ABMultiValueCopyLabelAtIndex( p, i ) );
				[	v
					addObject:
					@{	@"value" : wValue ? wValue : @""
					,	@"label" : wLabel ? wLabel : @""
					,	@"identifier" : [ NSNumber numberWithInt:ABMultiValueGetIdentifierAtIndex( p, i ) ]
					}
				];
			}
			return v;
		}

		inline	NSArray*
		Addresses( ABRecordRef p )
		{	return (__bridge_transfer NSArray*)ABMultiValueCopyArrayOfAllValues
			(	Type<ABMultiValueRef>( ABRecordCopyValue( p, kABPersonAddressProperty ) )
			);
		}
		inline	NSArray*
		Mails( ABRecordRef p )
		{	return (__bridge_transfer NSArray*)ABMultiValueCopyArrayOfAllValues
			(	Type<ABMultiValueRef>( ABRecordCopyValue( p, kABPersonEmailProperty ) )
			);
		}
		inline	NSArray*
		Phones( ABRecordRef p )
		{	return (__bridge_transfer NSArray*)ABMultiValueCopyArrayOfAllValues
			(	Type<ABMultiValueRef>( ABRecordCopyValue( p, kABPersonPhoneProperty ) )
			);
		}
		inline	NSMutableArray*
		AttributedAddresses( ABRecordRef p )
		{	return AttributeArray( ABRecordCopyValue( p, kABPersonAddressProperty ) );
		}
		inline	NSMutableArray*
		AttributedPhones( ABRecordRef p )
		{	return AttributeArray( ABRecordCopyValue( p, kABPersonPhoneProperty ) );
		}
		inline	NSMutableArray*
		AttributedMails( ABRecordRef p )
		{	return AttributeArray( ABRecordCopyValue( p, kABPersonEmailProperty ) );
		}
		inline	NSString*
		CompositeName( ABRecordRef p )
		{	return (__bridge_transfer NSString*)ABRecordCopyCompositeName( p );
		}
		inline	NSString*
		FirstName( ABRecordRef p )
		{	return (__bridge_transfer NSString*)ABRecordCopyValue( p, kABPersonFirstNameProperty );
		}
		inline	NSString*
		LastName( ABRecordRef p )
		{	return (__bridge_transfer NSString*)ABRecordCopyValue( p, kABPersonLastNameProperty );
		}
		inline	NSString*
		MiddleName( ABRecordRef p )
		{	return (__bridge_transfer NSString*)ABRecordCopyValue( p, kABPersonMiddleNameProperty );
		}
		inline	NSString*
		Prefix( ABRecordRef p )
		{	return (__bridge_transfer NSString*)ABRecordCopyValue( p, kABPersonPrefixProperty );
		}
		inline	NSString*
		Suffix( ABRecordRef p )
		{	return (__bridge_transfer NSString*)ABRecordCopyValue( p, kABPersonSuffixProperty );
		}
		inline	NSString*
		Nickname( ABRecordRef p )
		{	return (__bridge_transfer NSString*)ABRecordCopyValue( p, kABPersonNicknameProperty );
		}
		inline	NSString*
		FirstNamePhonetic( ABRecordRef p )
		{	return (__bridge_transfer NSString*)ABRecordCopyValue( p, kABPersonFirstNamePhoneticProperty );
		}
		inline	NSString*
		LastNamePhonetic( ABRecordRef p )
		{	return (__bridge_transfer NSString*)ABRecordCopyValue( p, kABPersonLastNamePhoneticProperty );
		}
		inline	NSString*
		MiddleNamePhonetic( ABRecordRef p )
		{	return (__bridge_transfer NSString*)ABRecordCopyValue( p, kABPersonMiddleNamePhoneticProperty );
		}
		inline	NSString*
		Organization( ABRecordRef p )
		{	return (__bridge_transfer NSString*)ABRecordCopyValue( p, kABPersonOrganizationProperty );
		}
		inline	NSString*
		JobTitle( ABRecordRef p )
		{	return (__bridge_transfer NSString*)ABRecordCopyValue( p, kABPersonJobTitleProperty );
		}
		inline	NSString*
		Department( ABRecordRef p )
		{	return (__bridge_transfer NSString*)ABRecordCopyValue( p, kABPersonDepartmentProperty );
		}
		inline	NSDate*
		Birthday( ABRecordRef p )
		{	return (__bridge_transfer NSDate*)ABRecordCopyValue( p, kABPersonBirthdayProperty );
		}
		inline	NSString*
		Note( ABRecordRef p )
		{	return (__bridge_transfer NSString*)ABRecordCopyValue( p, kABPersonNoteProperty );
		}
		inline	NSData*
		ImageData( ABRecordRef p )
		{	return (__bridge_transfer NSData*)ABPersonCopyImageData( p );
		}
		inline	NSData*
		ImageDataThumbnail( ABRecordRef p )
		{	return (__bridge_transfer NSData*)ABPersonCopyImageDataWithFormat( p, kABPersonImageFormatThumbnail );
		}
		inline	NSString*
		GroupName( ABRecordRef p )
		{	return (__bridge_transfer NSString*)ABRecordCopyValue( p, kABGroupNameProperty );
		}
		inline	bool
		GroupName( ABRecordRef pGroup, NSString* p, CFErrorRef* pError )
		{	return ABRecordSetValue( pGroup, kABGroupNameProperty, (__bridge CFStringRef)p, pError );
		}
		inline	NSString*
		SourceName( ABRecordRef p )
		{	return (__bridge_transfer NSString*)ABRecordCopyValue( p, kABSourceNameProperty );
		}
		inline	NSNumber*
		SourceType( ABRecordRef p )
		{	return (__bridge_transfer NSNumber*)ABRecordCopyValue( p, kABSourceTypeProperty );
		}

		inline	NSArray*
		Members( ABRecordRef p )
		{	return (__bridge_transfer NSArray*)ABGroupCopyArrayOfAllMembers( p );
		}
		inline	NSArray*
		MembersWithSortOrdering( ABRecordRef p, ABPersonSortOrdering pPSO )
		{	return (__bridge_transfer NSArray*)ABGroupCopyArrayOfAllMembersWithSortOrdering( p, pPSO );
		}

		inline	void
		GrantedAddressBook
		(	void (^pGrantBlock)( ABAddressBookRef )
		,	void (^pNoGrantBlock)() = ^
			{	BlockAlert
				(	@"設定であなたの連絡帳にアクセスする権限を与えてください。"
				,	@"Permission was not granted for Contacts."
				);
			}
		,	void (^pErrorBlock)( NSError* ) = ^( NSError* p )
			{	BlockAlert( p.description, @"Unknown Error" );
			}
		)
		{	switch ( ABAddressBookGetAuthorizationStatus() )
			{
			case	kABAuthorizationStatusRestricted:
			case	kABAuthorizationStatusDenied:
				pNoGrantBlock();
				return;
			case	kABAuthorizationStatusAuthorized:
				{	CFErrorRef wE = nil;
					ABAddressBookRef w = ABAddressBookCreateWithOptions( nullptr, &wE );
					if ( w )
					{	pGrantBlock( w );
						CFRelease( w );
					}
					if ( wE ) pErrorBlock( (__bridge_transfer NSError*)wE );
				}
				break;
			case	kABAuthorizationStatusNotDetermined:
				{	CFErrorRef wE = nil;
					ABAddressBookRef w = ABAddressBookCreateWithOptions( nullptr, &wE );
					if ( w ) ABAddressBookRequestAccessWithCompletion
					(	w
					,	^( bool pGranted, CFErrorRef pError )
						{	dispatch_async
							(	dispatch_get_main_queue()
							,	^{	if ( pGranted ) pGrantBlock( w );
									else			pNoGrantBlock();
									if	( pError )	pErrorBlock( (__bridge_transfer NSError*)pError );
									CFRelease( w );
								}
							);
						}
					);
					if ( wE ) pErrorBlock( (__bridge_transfer NSError*)wE );
				}
				break;
			}
		}

		inline	bool
		HasUnsavedChanges( ABAddressBookRef p )
		{	return ABAddressBookHasUnsavedChanges( p );
		}
		inline	void
		Save
		(	ABAddressBookRef p
		,	void (^pErrorBlock)( NSError* ) = ^( NSError* p )
			{	JpMooParaiso::Apple::BlockAlert( p.description, @"Unknown Error" );
			}
		)
		{	CFErrorRef wError;
			if ( ! ABAddressBookSave( p, &wError ) ) pErrorBlock( (__bridge_transfer NSError*)wError );
		}
		inline	void
		Revert( ABAddressBookRef p )
		{	ABAddressBookRevert( p );
		}
		inline	ABRecordRef
		Person( ABAddressBookRef pAB, ABRecordID p )
		{	return ABAddressBookGetPersonWithRecordID( pAB, p );
		}
		inline	ABRecordRef
		Group( ABAddressBookRef pAB, ABRecordID p )
		{	return ABAddressBookGetGroupWithRecordID( pAB, p );
		}
		inline	NSArray*
		People( ABAddressBookRef p )
		{	return (__bridge_transfer NSArray*)ABAddressBookCopyArrayOfAllPeople( p );
		}
		inline	NSArray*
		PeopleSorted( ABAddressBookRef p )
		{	return (__bridge_transfer NSArray*)ABAddressBookCopyArrayOfAllPeople( p );
		}
		inline	NSArray*
		Groups( ABAddressBookRef p )
		{	return (__bridge_transfer NSArray*)ABAddressBookCopyArrayOfAllGroups( p );
		}
		inline	NSInteger
		GroupCount( ABAddressBookRef p )
		{	return ABAddressBookGetGroupCount( p );
		}
		inline	NSArray*
		Sources( ABAddressBookRef p )
		{	return (__bridge_transfer NSArray*)ABAddressBookCopyArrayOfAllSources( p );
		}
		inline	ABRecordRef
		LocalSource( ABAddressBookRef p )
		{	for ( id w in Sources( p ) )
			{	ABRecordRef v = (__bridge ABRecordRef)w;
				if ( SourceType( v ).intValue == kABSourceTypeLocal ) return v;
			}
			return NULL;
		}
/*
		struct
		AddressBook : Type<ABAddressBookRef>
		{	AddressBook( ABAddressBookRef p )
			:	Type( p, YES )
			{
			}
			bool
			HasUnsavedChanges()
			{	return ABAddressBookHasUnsavedChanges( u );
			}
			void
			Save
			(	void (^pErrorBlock)( NSError* ) = ^( NSError* p )
				{	JpMooParaiso::Apple::BlockAlert( p.description, @"Unknown Error" );
				}
			)
			{	CFErrorRef wError;
				if ( ! ABAddressBookSave( u, &wError ) ) pErrorBlock( (__bridge_transfer NSError*)wError );
			}
			void
			Revert()
			{	ABAddressBookRevert( u );
			}
			ABRecordRef
			Person( ABRecordID p )
			{	return ABAddressBookGetPersonWithRecordID( u, p );
			}
			NSArray*
			AllPeople()
			{	return (__bridge_transfer NSArray*)ABAddressBookCopyArrayOfAllPeople( u );
			}
			NSArray*
			AllPeopleSorted()
			{	return (__bridge_transfer NSArray*)ABAddressBookCopyArrayOfAllPeople( u );
			}
			NSArray*
			AllGroups()
			{	return (__bridge_transfer NSArray*)ABAddressBookCopyArrayOfAllGroups( u );
			}
			NSArray*
			AllSources()
			{	return (__bridge_transfer NSArray*)ABAddressBookCopyArrayOfAllSources( u );
			}
			ABRecordRef
			LocalSource()
			{	for ( id w in AllSources() )
				{	ABRecordRef v = (__bridge ABRecordRef)w;
					if ( SourceType( v ).intValue == kABSourceTypeLocal ) return v;
				}
				return NULL;
			}

		};
*/
#define	FORMATTING
#ifdef	FORMATTING
		inline	NSString*
		FormatID( NSObject* p, int pIndentLevel = 0 );

		inline	NSString*
		FormatData( NSData* p, int pIndentLevel = 0 )
		{	NSMutableString*	v = NSMutableString.string;
			[ v appendFormat:@"<%@>", [ [ NSString alloc ] initWithData:p encoding:NSUTF8StringEncoding ] ];
			return v;
		}

		inline	NSString*
		Tabs( int p )
		{	NSMutableString* v = NSMutableString.string;
			for ( int i = 0; i < p; i++ ) [ v appendString:@"\t" ];
			return v;
		}

		inline	NSString*
		FormatString( NSString* p,int pIndentLevel = 0 )
		{	return [ NSString stringWithFormat:@"@\"%@\"", p ];
		}

		inline	NSString*
		FormatDictionary( NSDictionary* p, int pIndentLevel = 0 )
		{	NSMutableString*	v = NSMutableString.string;
			[ v appendString:@"@{" ];
			if ( p.allKeys.count )
			{	[ v appendFormat:@"\t%@", FormatID( p.allKeys[ 0 ], pIndentLevel + 1 ) ];
				NSString* wValueString = FormatID( p[ p.allKeys[ 0 ] ], pIndentLevel + 1 );
				if ( [ wValueString rangeOfCharacterFromSet:NSCharacterSet.newlineCharacterSet ].length )
				{	[ v appendFormat:@"\n%@:%@\n", Tabs( pIndentLevel + 1 ), wValueString ];
				}
				else
				{	[ v appendFormat:@":\t%@\n", wValueString ];
				}
				for ( int i = 1; i < p.allKeys.count; i++ )
				{	[ v appendFormat:@"%@,\t%@", Tabs( pIndentLevel ), FormatID( p.allKeys[ i ], pIndentLevel + 1 ) ];
					NSString* wValueString = FormatID( p[ p.allKeys[ i ] ], pIndentLevel + 1 );
					if ( [ wValueString rangeOfCharacterFromSet:NSCharacterSet.newlineCharacterSet ].length )
					{	[ v appendFormat:@"\n%@:%@\n", Tabs( pIndentLevel + 1 ), wValueString ];
					}
					else
					{	[ v appendFormat:@":\t%@\n", wValueString ];
					}
				}
			}
			else
			{	[ v appendString:@"\n" ];
			}
			[ v appendFormat:@"%@}", Tabs( pIndentLevel ) ];
			return v;
		}

		inline	NSString*
		FormatArray( NSArray* p, int pIndentLevel = 0 )
		{	NSMutableString*	v = NSMutableString.string;
			[ v appendString:@"@[" ];
			if ( p.count )
			{	[ v appendFormat:@"\t%@\n", FormatID( p[ 0 ], pIndentLevel + 1 ) ];
				for ( int i = 1; i < p.count; i++ )
				{	[ v appendFormat:@"%@,\t%@\n", Tabs( pIndentLevel ), FormatID( p[ i ], pIndentLevel + 1 ) ];
				}
			}
			else
			{	[ v appendString:@"\n" ];
			}
			[ v appendFormat:@"%@]", Tabs( pIndentLevel ) ];
			return v;
		}

		inline	NSString*
		FormatID( NSObject* p, int pIndentLevel )
		{	if ( [ p isKindOfClass:NSDictionary.class ] )	return FormatDictionary	( (NSDictionary*)p	, pIndentLevel );
			if ( [ p isKindOfClass:NSArray.class ] )		return FormatArray		( (NSArray*)p		, pIndentLevel );
			if ( [ p isKindOfClass:NSData.class ] )			return FormatData		( (NSData*)p		, pIndentLevel );
			if ( [ p isKindOfClass:NSString.class ] )		return FormatString		( (NSString*)p		, pIndentLevel );
			else
			{	NSMutableString*	v = NSMutableString.string;
				[ v appendString:p.description ];
				return v;
			}
		}
#endif
	}
}

