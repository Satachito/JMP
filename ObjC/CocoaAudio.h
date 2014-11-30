#pragma once
//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

#import		<AVFoundation/AVFoundation.h>

namespace
JpMooParaiso
{	namespace
	Apple
	{
#if	TARGET_OS_IPHONE
		inline	AVAudioPlayer*
		PreparedPlayer( NSString* pName, NSString* pExt, float pVolume, NSInteger pNumLoops = 0 )
		{	AVAudioPlayer* v =
			[	[ AVAudioPlayer alloc ]
				initWithContentsOfURL:[ NSBundle.mainBundle URLForResource:pName withExtension:pExt ]
				error:nil
			];
			v.numberOfLoops = pNumLoops;
			[ v prepareToPlay ];
			v.volume = pVolume;
			return v;
		}
		inline	void
		DumpAVAudioSession()
		{	AVAudioSession* w = AVAudioSession.sharedInstance;
			NSLog(   @"category:						\n												%@", w.category );
			NSLog(   @"categoryOptions:					\n												%zd", w.categoryOptions );
			NSLog(   @"mode:							\n												%@", w.mode );
			NSLog(   @"outputVolume:					\n												%f", w.outputVolume );
			NSLog(   @"inputGain:						\n												%f", w.inputGain );
			NSLog(   @"inputGainSettable:				\n												%d", w.inputGainSettable );
			NSLog(   @"inputLatency:					\n												%f", w.inputLatency );
			NSLog(   @"outputLatency:					\n												%f", w.outputLatency );
			NSLog(   @"sampleRate:						\n												%f", w.sampleRate );
			NSLog(   @"preferredSampleRate:				\n												%f", w.preferredSampleRate );
			NSLog(   @"IOBufferDuration:				\n												%f", w.IOBufferDuration );
			NSLog(   @"preferredIOBufferDuration:		\n												%f", w.preferredIOBufferDuration );
			NSLog(   @"inputNumberOfChannels:			\n												%zd", w.inputNumberOfChannels );
			NSLog(   @"maximumInputNumberOfChannels:	\n												%zd", w.maximumInputNumberOfChannels );
			NSLog(   @"preferredInputNumberOfChannels:	\n												%zd", w.preferredInputNumberOfChannels );
			NSLog(   @"outputNumberOfChannels:			\n												%zd", w.outputNumberOfChannels );
			NSLog(   @"maximumOutputNumberOfChannels:	\n												%zd", w.maximumOutputNumberOfChannels );
			NSLog(   @"preferredOutputNumberOfChannels:	\n												%zd", w.preferredOutputNumberOfChannels );
			NSLog(   @"currentRoute:					\n												%@", w.currentRoute );
			NSLog(   @"inputAvailable:					\n												%d", w.inputAvailable );
			NSLog(   @"otherAudioPlaying:				\n												%d", w.otherAudioPlaying );
			NSLog(   @"availableInputs:					\n												%@", w.availableInputs );
			NSLog(   @"preferredInput:					\n												%@", w.preferredInput );
			NSLog(   @"inputDataSources:				\n												%@", w.inputDataSources );
			NSLog(   @"inputDataSource:					\n												%@", w.inputDataSource );
			NSLog(   @"outputDataSources:				\n												%@", w.outputDataSources );
			NSLog(   @"outputDataSource:				\n												%@", w.outputDataSource );
		}
#endif
	}
}

