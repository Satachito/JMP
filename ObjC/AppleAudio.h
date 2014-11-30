#pragma once
//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

#include	"Exceptions.h"
#include	"Generic.h"

#import		<AudioToolBox/AudioToolBox.h>
#if	TARGET_OS_IPHONE
#import		<AVFoundation/AVFoundation.h>
#endif

namespace
JpMooParaiso
{	namespace
	Apple
	{
//	Dumper --------------------------------------------------------------
#ifdef	DEBUG
//		https://developer.apple.com/library/mac/technotes/tn2091/_index.html
		inline	const char*
		AudioUnitTypeString( OSType p )
		{	switch ( p )
			{
			case kAudioUnitType_Output:					return "kAudioUnitType_Output";
			case kAudioUnitType_MusicDevice:			return "kAudioUnitType_MusicDevice";
			case kAudioUnitType_MusicEffect:			return "kAudioUnitType_MusicEffect";
			case kAudioUnitType_FormatConverter:		return "kAudioUnitType_FormatConverter";
			case kAudioUnitType_Effect:					return "kAudioUnitType_Effect";
			case kAudioUnitType_Mixer:					return "kAudioUnitType_Mixer";
			case kAudioUnitType_Panner:					return "kAudioUnitType_Panner";
			case kAudioUnitType_Generator:				return "kAudioUnitType_Generator";
			case kAudioUnitType_OfflineEffect:			return "kAudioUnitType_OfflineEffect";
			case kAudioUnitType_MIDIProcessor:			return "kAudioUnitType_MIDIProcessor";
			default:	return "----";
			}
		}
#if	TARGET_OS_IPHONE
		inline	const char*
		AudioUnitSubTypeString( OSType p )
		{	switch ( p )
			{
			case kAudioUnitSubType_GenericOutput:		return "kAudioUnitSubType_GenericOutput";
			case kAudioUnitSubType_RemoteIO:			return "kAudioUnitSubType_RemoteIO";
			case kAudioUnitSubType_VoiceProcessingIO:	return "kAudioUnitSubType_VoiceProcessingIO";

			case kAudioUnitSubType_Sampler:				return "kAudioUnitSubType_Sampler";

			case kAudioUnitSubType_AUConverter:			return "kAudioUnitSubType_AUConverter";
			case kAudioUnitSubType_Varispeed:			return "kAudioUnitSubType_Varispeed";
			case kAudioUnitSubType_DeferredRenderer:	return "kAudioUnitSubType_DeferredRenderer";
			case kAudioUnitSubType_Splitter:			return "kAudioUnitSubType_Splitter";
			case kAudioUnitSubType_Merger:				return "kAudioUnitSubType_Merger";
			case kAudioUnitSubType_NewTimePitch:		return "kAudioUnitSubType_NewTimePitch";
			case kAudioUnitSubType_AUiPodTimeOther:		return "kAudioUnitSubType_AUiPodTimeOther";

			case kAudioUnitSubType_AUiPodTime:			return "kAudioUnitSubType_AUiPodTime";

			case kAudioUnitSubType_PeakLimiter:			return "kAudioUnitSubType_PeakLimiter";
			case kAudioUnitSubType_DynamicsProcessor:	return "kAudioUnitSubType_DynamicsProcessor";
			case kAudioUnitSubType_LowPassFilter:		return "kAudioUnitSubType_LowPassFilter";
			case kAudioUnitSubType_HighPassFilter:		return "kAudioUnitSubType_HighPassFilter";
			case kAudioUnitSubType_BandPassFilter:		return "kAudioUnitSubType_BandPassFilter";
			case kAudioUnitSubType_HighShelfFilter:		return "kAudioUnitSubType_HighShelfFilter";
			case kAudioUnitSubType_LowShelfFilter:		return "kAudioUnitSubType_LowShelfFilter";
			case kAudioUnitSubType_ParametricEQ:		return "kAudioUnitSubType_ParametricEQ";
			case kAudioUnitSubType_Distortion:			return "kAudioUnitSubType_Distortion";
			case kAudioUnitSubType_Delay:				return "kAudioUnitSubType_Delay";
			case kAudioUnitSubType_Reverb2:				return "kAudioUnitSubType_Reverb2";
			case kAudioUnitSubType_AUiPodEQ:			return "kAudioUnitSubType_AUiPodEQ";
			case kAudioUnitSubType_NBandEQ:				return "kAudioUnitSubType_NBandEQ";

			case kAudioUnitSubType_MultiChannelMixer:	return "kAudioUnitSubType_MultiChannelMixer";
			case kAudioUnitSubType_MatrixMixer:			return "kAudioUnitSubType_MatrixMixer";
			case kAudioUnitSubType_AU3DMixerEmbedded:	return "kAudioUnitSubType_AU3DMixerEmbedded";

			case kAudioUnitSubType_ScheduledSoundPlayer:return "kAudioUnitSubType_ScheduledSoundPlayer";
			case kAudioUnitSubType_AudioFilePlayer:		return "kAudioUnitSubType_AudioFilePlayer";

			default:	return "----";
			}
		}
#else
		inline	const char*
		AudioUnitSubTypeString( OSType p )
		{	switch ( p )
			{
			case kAudioUnitSubType_GenericOutput:		return "kAudioUnitSubType_GenericOutput";
			case kAudioUnitSubType_HALOutput:			return "kAudioUnitSubType_HALOutput";
			case kAudioUnitSubType_DefaultOutput:		return "kAudioUnitSubType_DefaultOutput";
			case kAudioUnitSubType_SystemOutput:		return "kAudioUnitSubType_SystemOutput";
			case kAudioUnitSubType_VoiceProcessingIO:	return "kAudioUnitSubType_VoiceProcessingIO";

			case kAudioUnitSubType_DLSSynth:			return "kAudioUnitSubType_DLSSynth";
			case kAudioUnitSubType_Sampler:				return "kAudioUnitSubType_Sampler";

			case kAudioUnitSubType_AUConverter:			return "kAudioUnitSubType_AUConverter";
			case kAudioUnitSubType_Varispeed:			return "kAudioUnitSubType_Varispeed";
			case kAudioUnitSubType_DeferredRenderer:	return "kAudioUnitSubType_DeferredRenderer";
			case kAudioUnitSubType_Splitter:			return "kAudioUnitSubType_Splitter";
			case kAudioUnitSubType_Merger:				return "kAudioUnitSubType_Merger";
			case kAudioUnitSubType_NewTimePitch:		return "kAudioUnitSubType_NewTimePitch";
			case kAudioUnitSubType_AUiPodTimeOther:		return "kAudioUnitSubType_AUiPodTimeOther";

			case kAudioUnitSubType_TimePitch:			return "kAudioUnitSubType_TimePitch/kAudioUnitSubType_Pitch";
			case kAudioUnitSubType_RoundTripAAC:		return "kAudioUnitSubType_RoundTripAAC";

			case kAudioUnitSubType_PeakLimiter:			return "kAudioUnitSubType_PeakLimiter";
			case kAudioUnitSubType_DynamicsProcessor:	return "kAudioUnitSubType_DynamicsProcessor";
			case kAudioUnitSubType_LowPassFilter:		return "kAudioUnitSubType_LowPassFilter";
			case kAudioUnitSubType_HighPassFilter:		return "kAudioUnitSubType_HighPassFilter";
			case kAudioUnitSubType_BandPassFilter:		return "kAudioUnitSubType_BandPassFilter";
			case kAudioUnitSubType_HighShelfFilter:		return "kAudioUnitSubType_HighShelfFilter";
			case kAudioUnitSubType_LowShelfFilter:		return "kAudioUnitSubType_LowShelfFilter";
			case kAudioUnitSubType_ParametricEQ:		return "kAudioUnitSubType_ParametricEQ";
			case kAudioUnitSubType_Distortion:			return "kAudioUnitSubType_Distortion";
			case kAudioUnitSubType_Delay:				return "kAudioUnitSubType_Delay";
			case kAudioUnitSubType_GraphicEQ:			return "kAudioUnitSubType_GraphicEQ";
			case kAudioUnitSubType_MultiBandCompressor:	return "kAudioUnitSubType_MultiBandCompressor";
			case kAudioUnitSubType_MatrixReverb:		return "kAudioUnitSubType_MatrixReverb";
		//	case kAudioUnitSubType_Pitch:				return "kAudioUnitSubType_TimePitch/kAudioUnitSubType_Pitch";
			case kAudioUnitSubType_AUFilter:			return "kAudioUnitSubType_AUFilter";
			case kAudioUnitSubType_NetSend:				return "kAudioUnitSubType_NetSend";
			case kAudioUnitSubType_SampleDelay:			return "kAudioUnitSubType_SampleDelay";
			case kAudioUnitSubType_RogerBeep:			return "kAudioUnitSubType_RogerBeepdefault";
			case kAudioUnitSubType_NBandEQ:				return "kAudioUnitSubType_NBandEQ";

			case kAudioUnitSubType_MultiChannelMixer:	return "kAudioUnitSubType_MultiChannelMixer";
			case kAudioUnitSubType_MatrixMixer:			return "kAudioUnitSubType_MatrixMixer";
			case kAudioUnitSubType_StereoMixer:			return "kAudioUnitSubType_StereoMixer";
			case kAudioUnitSubType_3DMixer:				return "kAudioUnitSubType_3DMixer";

			case kAudioUnitSubType_SphericalHeadPanner:	return "kAudioUnitSubType_SphericalHeadPanner";
			case kAudioUnitSubType_VectorPanner:		return "kAudioUnitSubType_VectorPanner";
			case kAudioUnitSubType_SoundFieldPanner:	return "kAudioUnitSubType_SoundFieldPanner";
			case kAudioUnitSubType_HRTFPanner:			return "kAudioUnitSubType_HRTFPanner";

			case kAudioUnitSubType_NetReceive:			return "kAudioUnitSubType_NetReceive";
			case kAudioUnitSubType_ScheduledSoundPlayer:return "kAudioUnitSubType_ScheduledSoundPlayer";
			case kAudioUnitSubType_AudioFilePlayer:		return "kAudioUnitSubType_AudioFilePlayer";

			default:	return "----";
			}
		}
#endif

		inline	const char*
		AudioUnitManufacturerString( OSType p )
		{	switch ( p )
			{
			case 	kAudioUnitManufacturer_Apple:		return "kAudioUnitManufacturer_Apple";
			default:	return "----";
			}
		}

		inline	void
		DumpAudioComponents( OSType pType = 0, OSType pSubType = 0, OSType pManufacturer = 0 )
		{	AudioComponentDescription	wACD;
			wACD.componentType			= pType;
			wACD.componentSubType		= pSubType;
			wACD.componentManufacturer	= pManufacturer;
			wACD.componentFlags			= 0;
			wACD.componentFlagsMask		= 0;

			printf( "#ofComponents:%zd\n", AudioComponentCount( &wACD ) );

			AudioComponent wAC = NULL;
			while ( ( wAC = AudioComponentFindNext( wAC, &wACD ) ) )
			{	CFStringRef wName;
				X( AudioComponentCopyName( wAC, &wName ) );
				UInt32 wVersion;
				X( AudioComponentGetVersion( wAC, &wVersion ) );
				printf( "%8x : %s\n", (unsigned int)wVersion, CFStringGetCStringPtr( wName, kCFStringEncodingMacRoman ) );
				
				AudioComponentDescription wACD;
				X( AudioComponentGetDescription( wAC, &wACD ) );
				printf( "            Type           %s:%s\n", OSTypeString( wACD.componentType )		, AudioUnitTypeString( wACD.componentType ) );
				printf( "            SubType        %s:%s\n", OSTypeString( wACD.componentSubType )		, AudioUnitSubTypeString( wACD.componentSubType ) );
				printf( "            Manufacturerer %s:%s\n", OSTypeString( wACD.componentManufacturer ), AudioUnitManufacturerString( wACD.componentManufacturer ) );
				printf( "            Flags          %x %x\n", (unsigned int)wACD.componentFlags, (unsigned int)wACD.componentFlagsMask );
			}
		}
		inline	void
		DumpFlags( UInt32 p )
		{	if ( p & 0x00000001 ) printf( "    kAudioFormatFlagIsFloat\n" );
			if ( p & 0x00000002 ) printf( "    kAudioFormatFlagIsBigEndian\n" );
			if ( p & 0x00000004 ) printf( "    kAudioFormatFlagIsSignedInteger\n" );
			if ( p & 0x00000008 ) printf( "    kAudioFormatFlagIsPacked\n" );
			if ( p & 0x00000010 ) printf( "    kAudioFormatFlagIsAlignedHigh\n" );
			if ( p & 0x00000020 ) printf( "    kAudioFormatFlagIsNonInterleaved\n" );
			if ( p & 0x00000040 ) printf( "    kAudioFormatFlagIsNonMixable\n" );
			if ( p & 0x80000000 ) printf( "    kAudioFormatFlagsAreAllClear\n" );
			
		}
		inline	void
		DumpPCMFlags( UInt32 p )
		{	if ( p & 0x00000001 ) printf( "    kLinearPCMFormatFlagIsFloat\n" );
			if ( p & 0x00000002 ) printf( "    kLinearPCMFormatFlagIsBigEndian\n" );
			if ( p & 0x00000004 ) printf( "    kLinearPCMFormatFlagIsSignedInteger\n" );
			if ( p & 0x00000008 ) printf( "    kLinearPCMFormatFlagIsPacked\n" );
			if ( p & 0x00000010 ) printf( "    kLinearPCMFormatFlagIsAlignedHigh\n" );
			if ( p & 0x00000020 ) printf( "    kLinearPCMFormatFlagIsNonInterleaved\n" );
			if ( p & 0x00000040 ) printf( "    kLinearPCMFormatFlagIsNonMixable\n" );
			if ( p & 0x80000000 ) printf( "    kLinearPCMFormatFlagsAreAllClear\n" );
			printf
			(	"    kLinearPCMFormatFlagsSampleFractionShift:%zd\n"
			,	( p & kLinearPCMFormatFlagsSampleFractionMask ) >> kLinearPCMFormatFlagsSampleFractionShift
			);
		}
		inline	void
		DumpAppleLosslessFlags( UInt32 p )
		{	switch( p )
			{
			case 1:	printf( "    kAppleLosslessFormatFlag_16BitSourceData\n" );
			case 2:	printf( "    kAppleLosslessFormatFlag_20BitSourceData\n" );
			case 3:	printf( "    kAppleLosslessFormatFlag_24BitSourceData\n" );
			case 4:	printf( "    kAppleLosslessFormatFlag_32BitSourceData\n" );
			}
		}
		inline	void
		Dump( AudioStreamBasicDescription const& p )
		{
			printf( "mSampleRate      :%f\n"	, p.mSampleRate );
			printf( "mFormatID        :%s\n"	, OSTypeString( p.mFormatID ) );
			printf( "mFormatFlags     :0x%x\n"	, (unsigned int)p.mFormatFlags );
			switch ( p.mFormatID )
			{
			case kAudioFormatLinearPCM:
				DumpPCMFlags( p.mFormatFlags );
				break;
			case kAudioFormatAppleLossless:
				DumpAppleLosslessFlags( p.mFormatFlags );
				break;
			default:
				DumpFlags( p.mFormatFlags );
				break;
			}
			printf( "mBytesPerPacket  :%zd\n"	, p.mBytesPerPacket );
			printf( "mFramesPerPacket :%zd\n"	, p.mFramesPerPacket );
			printf( "mBytesPerFrame   :%zd\n"	, p.mBytesPerFrame );
			printf( "mChannelsPerFrame:%zd\n"	, p.mChannelsPerFrame );
			printf( "mBitsPerChannel  :%zd\n"	, p.mBitsPerChannel );
		}
#endif
/*
		inline	AudioStreamBasicDescription
		FloatASBD( float p = 0 )
		{	AudioStreamBasicDescription	v;
			v.mSampleRate = p;
			v.mFormatID = kAudioFormatLinearPCM;
			v.mFormatFlags = kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked | kAudioFormatFlagIsFloat | kAudioFormatFlagIsNonInterleaved;
			v.mBytesPerPacket = 4;
			v.mFramesPerPacket = 1;
			v.mBytesPerFrame = 4;
			v.mChannelsPerFrame = 2;
			v.mBitsPerChannel = 32;
			v.mReserved = 0;
			return v;
		}
*/
//	Audio Queue --------------------------------------------------------------
		inline	AudioQueueRef
		NewOutput
		(	AudioStreamBasicDescription const&	pASBD 
		,	AudioQueueOutputCallback			pCB
		,	void*								pUserData = NULL
		,	CFRunLoopRef						pCallbackRunLoop = NULL		//	CFRunLoopGetCurrent()
		,	CFStringRef							pCallbackRunLoopMode = NULL	//	equiv. kCFRunLoopCommonModes
		,	UInt32								pFlags = 0
		)
		{	AudioQueueRef	v;
			X( AudioQueueNewOutput( &pASBD, pCB, pUserData, pCallbackRunLoop, pCallbackRunLoopMode, pFlags, &v ) );
			return v;
		}

		inline	AudioQueueRef
		NewInput
		(	AudioStreamBasicDescription const&	pASBD 
		,	AudioQueueInputCallback				pCB
		,	void*								pUserData = NULL
		,	CFRunLoopRef						pCallbackRunLoop = NULL
		,	CFStringRef							pCallbackRunLoopMode = NULL
		,	UInt32								pFlags = 0
		)
		{	AudioQueueRef	v;
			X( AudioQueueNewInput( &pASBD, pCB, pUserData, pCallbackRunLoop, pCallbackRunLoopMode, pFlags, &v ) );
			return v;
		}

		inline	void
		Dispose( AudioQueueRef pAQ, Boolean pImmediate = TRUE )	//	Boolean is MacOS's historic name of the type 'bool'
		{	X( AudioQueueDispose( pAQ, pImmediate ) );
		}

		inline	void
		Start( AudioQueueRef pAQ, const AudioTimeStamp* pStartTime = nullptr )
		{	X( AudioQueueStart( pAQ, pStartTime ) );
		}

		inline	void
		Stop( AudioQueueRef pAQ, Boolean pImmediate = FALSE )	//	Boolean is MacOS's historic name of the type 'bool'
		{	X( AudioQueueStop( pAQ, pImmediate ) );
		}

		inline	AudioQueueBufferRef
		AllocateBuffer( AudioQueueRef pAQ, int pBufferByteSize )
		{	AudioQueueBufferRef	v;
			X( AudioQueueAllocateBuffer( pAQ, pBufferByteSize, &v ) );
			return v;
		}

		inline	void
		EnqueueBuffer
		(	AudioQueueRef pAQ
		,	AudioQueueBufferRef pAQB
		,	UInt32 pNumPacketDescs = 0
		,	const AudioStreamPacketDescription* pASPD = NULL
		)
		{	X( AudioQueueEnqueueBuffer( pAQ, pAQB, pNumPacketDescs, pASPD ) );
		}

		inline	UInt32
		PropertySize( AudioQueueRef pAQ, AudioQueuePropertyID pID )
		{	UInt32	v;
			X( AudioQueueGetPropertySize( pAQ, pID, &v ) );
			return v;
		}

		template	<typename T>	T
		Property( AudioQueueRef pAQ, AudioQueuePropertyID pID )
		{	T v;
			UInt32	wSize = sizeof( v );
			X( AudioQueueGetProperty( pAQ, pID, &v, &wSize ) );
			assert( wSize == sizeof( v ) );
			return v;
		}

		template	<typename T>	void
		Property( AudioQueueRef pAQ, AudioQueuePropertyID pID ,T const& p )
		{	X( AudioQueueSetProperty( pAQ, pID, &p, sizeof( p ) ) );
		}

		inline	AudioStreamBasicDescription
		StreamDescription( AudioQueueRef p )
		{	return Property<AudioStreamBasicDescription>( p, kAudioQueueProperty_StreamDescription );
		}

		inline	UInt32
		IsRunning( AudioQueueRef p )
		{	return Property<UInt32>( p, kAudioQueueProperty_IsRunning );
		}
/*
		inline	NSData*
		MagicCookie( AudioQueueRef p )
		{	UInt32	wSize = PropertySize( p, kAudioQueueProperty_MagicCookie );
			if ( wSize )
			{	char w[ wSize ];
				X( AudioQueueGetProperty( p, kAudioQueueProperty_MagicCookie, w, &wSize ) );
				return [ NSData dataWithBytes:w length:wSize ];
			}
			else	// -- if the file format does support magic cookies, but this file doesn't have one.
			{	return nil;
			}
		}

		inline	void
		MagicCookie( AudioQueueRef pAQR, NSData* p )
		{	X( AudioQueueSetProperty( pAQR, kAudioQueueProperty_MagicCookie, p.bytes, (UInt32)p.length ) );
		}

		inline	void
		ChannelLayout( AudioQueueRef pAQR, NSData* p )
		{	X( AudioQueueSetProperty( pAQR, kAudioQueueProperty_ChannelLayout, p.bytes, (UInt32)p.length ) );
		}
*/
		inline	void
		AddIsRunningListener( AudioQueueRef pAQR, AudioQueuePropertyListenerProc p, void* pUserData )
		{	X( AudioQueueAddPropertyListener( pAQR, kAudioQueueProperty_IsRunning, p, pUserData ) );
		}

//	Ext Audio File --------------------------------------------------------------
		inline	ExtAudioFileRef
		ExtCreateWithURL
		(	CFURLRef							p
		,	AudioFileTypeID						pAFT
		,	AudioStreamBasicDescription const&	pASBD
		,	const AudioChannelLayout*			pACL = NULL
		,	UInt32								pFlags = kAudioFileFlags_EraseFile
		)
		{	ExtAudioFileRef		v;
			X( ExtAudioFileCreateWithURL( p, pAFT, &pASBD, pACL, pFlags, &v ) );
			return v;
		}

		inline	ExtAudioFileRef
		ExtOpenURL( CFURLRef p )
		{	ExtAudioFileRef		v;
			X( ExtAudioFileOpenURL( p, &v ) );
			return v;
		}

		inline	void
		Dispose( ExtAudioFileRef p )
		{	X( ExtAudioFileDispose( p ) );
		}

		inline	SInt64
		Tell( ExtAudioFileRef p )
		{	SInt64	v;
			X( ExtAudioFileTell( p, &v ) );
			return v;
		}

		inline	void
		Seek( ExtAudioFileRef pEAFR, SInt64 p )
		{	X( ExtAudioFileSeek( pEAFR, p ) );
		}

		inline	UInt32
		Read( ExtAudioFileRef pEAFR, UInt32 pNumFrames, AudioBufferList* p )
		{	X( ExtAudioFileRead( pEAFR, &pNumFrames, p ) );
			return pNumFrames;
		}

		inline	void
		Write( ExtAudioFileRef pEAFR, UInt32 pNumFrames, AudioBufferList const* p )
		{	X( ExtAudioFileWrite( pEAFR, pNumFrames, p ) );
		}

		inline	void
		WriteAsync( ExtAudioFileRef pEAFR, UInt32 pNumFrames, AudioBufferList const* p )
		{	X( ExtAudioFileWriteAsync( pEAFR, pNumFrames, p ) );
		}

		inline	UInt32
		PropertySize( ExtAudioFileRef pEAFR, ExtAudioFilePropertyID pID )
		{	UInt32	v;
			X( ExtAudioFileGetPropertyInfo( pEAFR, pID, &v, NULL ) );
			return	v;
		}

		template	<typename T>	T
		Property( ExtAudioFileRef pEAFR, ExtAudioFilePropertyID pID )
		{	T v;
			UInt32	wSize = sizeof( v );
			X( ExtAudioFileGetProperty( pEAFR, pID, &wSize, &v ) );
			assert( wSize == sizeof( v ) );
			return v;
		}

		template	<typename T>	void
		Property( ExtAudioFileRef pEAFR, ExtAudioFilePropertyID pID ,const T& p )
		{	X( ExtAudioFileSetProperty( pEAFR, pID, sizeof( p ), &p ) );
		}

		inline	SInt64
		FileLengthFrames( ExtAudioFileRef p )
		{	return Property<SInt64>( p, kExtAudioFileProperty_FileLengthFrames );
		}

		inline	AudioStreamBasicDescription
		FileDataFormat( ExtAudioFileRef p )
		{	return Property<AudioStreamBasicDescription>( p, kExtAudioFileProperty_FileDataFormat );
		}

		inline	AudioStreamBasicDescription
		ClientDataFormat( ExtAudioFileRef p )
		{	return Property<AudioStreamBasicDescription>( p, kExtAudioFileProperty_ClientDataFormat );
		}

		inline	void
		ClientDataFormat( ExtAudioFileRef pEAFR, AudioStreamBasicDescription const& p )
		{	Property<AudioStreamBasicDescription>( pEAFR, kExtAudioFileProperty_ClientDataFormat, p );
		}

		inline	AudioConverterRef
		AudioConverter( ExtAudioFileRef p )
		{	return Property<AudioConverterRef>( p, kExtAudioFileProperty_AudioConverter );
		}

		inline	AudioFileID
		AudioFile( ExtAudioFileRef p )
		{	return Property<AudioFileID>( p, kExtAudioFileProperty_AudioFile );
		}

#if	TARGET_OS_IPHONE
//	Audio Converter --------------------------------------------------------------
		inline	UInt32
		CanResumeFromInterruption( AudioConverterRef p )
		{	UInt32	v;
			UInt32	wSize = sizeof( v );
			X( AudioConverterGetProperty( p, kAudioConverterPropertyCanResumeFromInterruption, &wSize, &v ) );
			return v;
		}
#endif
//	Audio File --------------------------------------------------------------
		inline	AudioFileID
		CreateWithURL
		(	CFURLRef							pURL
		,	AudioFileTypeID						pAFT
		,	AudioStreamBasicDescription const&	pASBD
		,	UInt32								pFlags = kAudioFileFlags_EraseFile
		)
		{	AudioFileID	v;
			X( AudioFileCreateWithURL( pURL, pAFT, &pASBD, pFlags, &v ) );
			return v;
		}

		inline	AudioFileID
		OpenURL( CFURLRef pURL, SInt8 pPermissions = kAudioFileReadPermission, AudioFileTypeID pFileTypeHint = 0 )
		{	AudioFileID		v;
			X( AudioFileOpenURL( pURL, pPermissions, pFileTypeHint, &v ) );
			return v;
		}

		template	<typename T>	T
		Property( AudioFileID pAFID, ExtAudioFilePropertyID pID )
		{	T v;
			UInt32	wSize = sizeof( v );
			X( AudioFileGetProperty( pAFID, pID, &wSize, &v ) );
			assert( wSize == sizeof( v ) );
			return v;
		}

		template	<typename T>	void
		Property( AudioFileID pAFID, ExtAudioFilePropertyID pID ,const T& p )
		{	X( AudioFileSetProperty( pAFID, pID, sizeof( p ), &p ) );
		}

		inline	AudioStreamBasicDescription
		DataFormat( AudioFileID p )
		{	return Property<AudioStreamBasicDescription>( p, kAudioFilePropertyDataFormat );
		}

		inline	UInt32
		PacketSizeUpperBound( AudioFileID p )
		{	return Property<UInt32>( p, kAudioFilePropertyPacketSizeUpperBound );
		}

#if	TARGET_OS_IPHONE
		inline	NSTimeInterval
		EstimatedDuration( AudioFileID p )
		{	return Property<NSTimeInterval>( p, kAudioFilePropertyEstimatedDuration );
		}
#else
		inline	CFTimeInterval
		EstimatedDuration( AudioFileID p )
		{	return Property<CFTimeInterval>( p, kAudioFilePropertyEstimatedDuration );
		}
#endif
/*
		inline	void
		MagicCookieData( AudioFileID pAFI, NSData* p )
		{	X( AudioFileSetProperty( pAFI, kAudioFilePropertyMagicCookieData, (UInt32)p.length, p.bytes ) );
		}

		inline	NSData*
		MagicCookieData( AudioFileID p )
		{	UInt32	wSize;
			X( AudioFileGetPropertyInfo( p, kAudioFilePropertyMagicCookieData, &wSize, NULL ) );
			if ( wSize )
			{	char w[ wSize ];
				X( AudioFileGetProperty( p, kAudioFilePropertyMagicCookieData, &wSize, w ) );
				return [ NSData dataWithBytes:w length:wSize ];
			}
			else	// -- if the file format does support magic cookies, but this file doesn't have one.
			{	return nil;
			}
		}

		inline	NSData*
		ChannelLayout( AudioFileID p )
		{	UInt32	wSize;
			X( AudioFileGetPropertyInfo( p, kAudioFilePropertyChannelLayout, &wSize, NULL ) );
			if ( wSize )
			{	char w[ wSize ];
				X( AudioFileGetProperty( p, kAudioFilePropertyChannelLayout, &wSize, w ) );
				return [ NSData dataWithBytes:w length:wSize ];
			}
			else
			{	return nil;
			}
		}
*/
//	Audio Unit --------------------------------------------------------------
		inline	AudioUnit
		NewRemoteIO()
		{
			AudioComponentDescription wDesc;
			wDesc.componentType = kAudioUnitType_Output;
#if	TARGET_OS_IPHONE
			wDesc.componentSubType = kAudioUnitSubType_RemoteIO;
#else
			wDesc.componentSubType = kAudioUnitSubType_HALOutput;
#endif
			wDesc.componentManufacturer = kAudioUnitManufacturer_Apple;
			wDesc.componentFlags = 0;
			wDesc.componentFlagsMask = 0;

			AudioComponent wAC = AudioComponentFindNext( NULL, &wDesc );
			assert( wAC );

			AudioUnit v;
			X( AudioComponentInstanceNew( wAC, &v ) );

			return v;
		}

		inline	UInt32
		PropertySize( AudioUnit pAU, AudioUnitPropertyID pID, AudioUnitScope pScope, AudioUnitElement pElement )
		{	UInt32	v;
			X( AudioUnitGetPropertyInfo( pAU, pID, pScope, pElement, &v, NULL ) );
			return v;
		}

		template	<typename T>	T
		Property( AudioUnit pAU, AudioUnitPropertyID pID, AudioUnitScope pScope, AudioUnitElement pElement )
		{	T v;
			UInt32	wSize = sizeof( v );
			X( AudioUnitGetProperty( pAU, pID, pScope, pElement, &v, &wSize ) );
			assert( wSize == sizeof( v ) );
			return v;
		}

		template	<typename T>	void
		Property( AudioUnit pAU, AudioUnitPropertyID pID, AudioUnitScope pScope, AudioUnitElement pElement ,const T& p )
		{	X( AudioUnitSetProperty( pAU, pID, pScope, pElement, &p, sizeof( p ) ) );
		}

		inline	void
		MaximumFramesPerSlice
		(	AudioUnit pAU
		,	UInt32 const& p
		,	AudioUnitElement pElement = 0
		,	AudioUnitScope pScope = kAudioUnitScope_Global
		)
		{	Property<UInt32>( pAU, kAudioUnitProperty_MaximumFramesPerSlice, pScope, pElement, p );
		}

		inline	UInt32
		MaximumFramesPerSlice
		(	AudioUnit p
		,	AudioUnitElement pElement = 0
		,	AudioUnitScope pScope = kAudioUnitScope_Global
		)
		{	return Property<UInt32>( p, kAudioUnitProperty_MaximumFramesPerSlice, pScope , pElement );
		}

		inline	void
		StreamFormat
		(	AudioUnit pAU
		,	AudioStreamBasicDescription const& p
		,	AudioUnitElement pElement
		,	AudioUnitScope pScope
		)
		{	Property<AudioStreamBasicDescription>( pAU, kAudioUnitProperty_StreamFormat, pScope, pElement, p );
		}

		inline	AudioStreamBasicDescription
		StreamFormat
		(	AudioUnit p
		,	AudioUnitElement pElement
		,	AudioUnitScope pScope
		)
		{	return Property<AudioStreamBasicDescription>( p, kAudioUnitProperty_StreamFormat, pScope, pElement );
		}

		inline	void
		EnableIO
		(	AudioUnit pAU
		,	UInt32 const& p
		,	AudioUnitElement pElement = 1
		,	AudioUnitScope pScope = kAudioUnitScope_Input 
		)
		{	Property<UInt32>( pAU, kAudioOutputUnitProperty_EnableIO, pScope, pElement, p );
		}

		inline	void
		RenderCallback
		(	AudioUnit pAU
		,	AURenderCallbackStruct const& p
		,	AudioUnitElement pElement = 0
		,	AudioUnitScope pScope = kAudioUnitScope_Input
		)
		{	Property<AURenderCallbackStruct>( pAU, kAudioUnitProperty_SetRenderCallback, pScope, pElement, p );
		}

		inline	AURenderCallbackStruct
		RenderCallbackStruct( AURenderCallback pInputProc, void* pInputProcRefCon )
		{	AURenderCallbackStruct v;
			v.inputProc = pInputProc;
			v.inputProcRefCon = pInputProcRefCon;
			return v;
		};

		inline	JpMooParaiso::Array<AudioComponent>
		AudioComponents( OSType pType = 0, OSType pSubType = 0, OSType pManufacturer = 0 )
		{	Array<AudioComponent> v;
			AudioComponentDescription	wACD;
			wACD.componentType			= pType;
			wACD.componentSubType		= pSubType;
			wACD.componentManufacturer	= pManufacturer;
			wACD.componentFlags			= 0;
			wACD.componentFlagsMask		= 0;

			AudioComponent wAC = NULL;
			while ( ( wAC = AudioComponentFindNext( wAC, &wACD ) ) ) v.Append( wAC );
			return v;
		}
		struct
		ACI : JpMooParaiso::Managed<ACI*>
		{
			AudioComponentInstance u;

			ACI()
			:	Managed<ACI*>( ^( ACI* p ) { AudioComponentInstanceDispose( p->u ); } )
			{	AudioComponentDescription wAD;
				wAD.componentType = kAudioUnitType_Output;
#if	TARGET_OS_IPHONE
				wAD.componentSubType = kAudioUnitSubType_RemoteIO;
#else
				wAD.componentSubType = kAudioUnitSubType_HALOutput;
#endif
				wAD.componentManufacturer = kAudioUnitManufacturer_Apple;
				wAD.componentFlags = 0;
				wAD.componentFlagsMask = 0;

				X( AudioComponentInstanceNew( AudioComponentFindNext( NULL, &wAD ), &u ) );
			}
			operator
			AudioComponentInstance()
			{	return u;
			}
			void
			Initialize()
			{	X( AudioUnitInitialize( u ) );
			}
			void
			Start()
			{	X( AudioOutputUnitStart( u ) );
			}
			void
			Stop()
			{	X( AudioOutputUnitStop( u ) );
			}
			void
			EnableIO( UInt32 p = 1, AudioUnitElement pElement = 1, AudioUnitScope pScope = kAudioUnitScope_Input )
			{	X( AudioUnitSetProperty( u, kAudioOutputUnitProperty_EnableIO, pScope, pElement, &p, sizeof( p ) ) );
			}
			void
			SetRenderCallback( AURenderCallbackStruct p, AudioUnitElement pElement = 0, AudioUnitScope pScope = kAudioUnitScope_Input )
			{	X( AudioUnitSetProperty( u, kAudioUnitProperty_SetRenderCallback, pScope, pElement, &p, sizeof( p ) ) );
			}
			void
			StreamFormat( AudioStreamBasicDescription p, AudioUnitElement pElement, AudioUnitScope pScope )
			{	X( AudioUnitSetProperty( u, kAudioUnitProperty_StreamFormat, pScope, pElement, &p, sizeof( p ) ) );
			}
			AudioStreamBasicDescription
			StreamFormat( AudioUnitElement pElement, AudioUnitScope pScope )
			{	AudioStreamBasicDescription v;
				UInt32 wSize = sizeof( v );
				X( AudioUnitGetProperty( u, kAudioUnitProperty_StreamFormat, pScope, pElement, &v, &wSize ) );
				return v;
			}
		};

		struct
		ABL : Managed<ABL*>
		{	AudioBufferList*	u;
			ABL( AudioBufferList* p )
			:	Managed<ABL*>( ^( ABL* p ) { delete[] p->u; } )
			{	UInt32 wSize = (UInt32)offsetof( AudioBufferList, mBuffers[ p->mNumberBuffers ] );
				for ( UInt32 i = 0; i < p->mNumberBuffers; i++ ) wSize += p->mBuffers[ i ].mDataByteSize;
				u = (AudioBufferList*)new char[ wSize ];
				u->mNumberBuffers = p->mNumberBuffers;
				char* w = (char*)&u->mBuffers[ p->mNumberBuffers ];
				for ( UInt32 i = 0; i < p->mNumberBuffers; i++ )
				{	u->mBuffers[ i ].mNumberChannels = p->mBuffers[ i ].mNumberChannels;
					u->mBuffers[ i ].mDataByteSize = p->mBuffers[ i ].mDataByteSize;
					u->mBuffers[ i ].mData = (void*)w;
					w += p->mBuffers[ i ].mDataByteSize;
				}
			}
			operator
			AudioBufferList*() const
			{	return u;
			}
			float*
			FloatData( int p = 0 ) const
			{	return (float*)u->mBuffers[ p ].mData;
			}
		};

		inline	AudioStreamBasicDescription
		CanonicalASBD()	//	Canonical
		{	AudioStreamBasicDescription v;
			v.mSampleRate		= 44100;
			v.mFormatID		= kAudioFormatLinearPCM;
			v.mBytesPerPacket	= 4;
			v.mFramesPerPacket = 1;
			v.mBytesPerFrame = 4;
			v.mChannelsPerFrame = 2;
			v.mBitsPerChannel = 32;
			v.mFormatFlags = kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked | kAudioFormatFlagIsFloat | kAudioFormatFlagIsNonInterleaved;
			v.mReserved = 0;
			return v;
		}

		struct
		PeekingACI : ACI
		{
			OSStatus
			(^uInputCB)
			(	AudioUnitRenderActionFlags*	pActionFlags
			,	const AudioTimeStamp*		pTimeStamp
			,	UInt32						pBusNumber
			,	UInt32						pNumberFrames
			,	AudioBufferList*			pData
			);
		
			static	OSStatus
			PeekingProc
			(	void*						pRefCon
			,	AudioUnitRenderActionFlags*	pActionFlags
			,	const AudioTimeStamp*		pTimeStamp
			,	UInt32						pBusNumber
			,	UInt32						pNumberFrames
			,	AudioBufferList*			pData
			)
			{	return ((PeekingACI*)pRefCon)->uInputCB
				(	pActionFlags
				,	pTimeStamp
				,	pBusNumber
				,	pNumberFrames
				,	pData
				);
			}

			PeekingACI
			(	OSStatus
				(^pInputCB)
				(	AudioUnitRenderActionFlags*	pActionFlags
				,	const AudioTimeStamp*		pTimeStamp
				,	UInt32						pBusNumber
				,	UInt32						pNumberFrames
				,	AudioBufferList*			pData
				)
			,	AudioStreamBasicDescription const& pASBD = CanonicalASBD()
			)
			:	ACI()
			,	uInputCB( pInputCB )
			{
				EnableIO();

				SetRenderCallback( RenderCallbackStruct( PeekingProc, this ) );
#ifdef	DEBUG
Dump( CanonicalASBD() );
Dump( StreamFormat( 1, kAudioUnitScope_Output ) );
Dump( StreamFormat( 0, kAudioUnitScope_Input ) );
#endif
				StreamFormat( pASBD, 1, kAudioUnitScope_Output );
				StreamFormat( pASBD, 0, kAudioUnitScope_Input );
#ifdef	DEBUG
Dump( StreamFormat( 1, kAudioUnitScope_Output ) );
Dump( StreamFormat( 0, kAudioUnitScope_Input ) );
#endif
				Initialize();
			}
		};
	}
}
