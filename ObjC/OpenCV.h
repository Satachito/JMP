#pragma once
//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

#include		<opencv2/opencv.hpp>
#include		"AppleGraphics.h"

namespace
JpMooParaiso
{	namespace
	OpenCV
	{
		inline	cv::Mat
		RGBMat( CGImageRef p )
		{	size_t	w = CGImageGetWidth( p );
			size_t	h = CGImageGetHeight( p );
			cv::Mat v( (int)h, (int)w, CV_8UC4 ); // 8 bits per component, 4 channels
			Apple::BitmapContext wBC
			(	w
			,	h
			,	8
			,	v.step[ 0 ]
			,	Apple::DeviceRGBColorSpace()
			,	kCGImageAlphaPremultipliedLast
			,	v.data
			);
			CGContextDrawImage( wBC, CGRectMake( 0, 0, w, h ), p );
			return v;
		}

		inline	Apple::Image
		RGBImage( cv::Mat const& p )
		{	assert( p.elemSize() == 4 );
			return Apple::Image
			(	Apple::BitmapContext
				(	p.cols
				,	p.rows
				,	8
				,	p.step[ 0 ]
				,	Apple::DeviceRGBColorSpace()
				,	kCGImageAlphaPremultipliedLast
				,	p.data
				)
			);
		}

		inline	cv::Mat
		GrayMat( CGImageRef p )
		{	size_t	w = CGImageGetWidth( p );
			size_t	h = CGImageGetHeight( p );
			cv::Mat v( (int)h, (int)w, CV_8UC1 );
			Apple::BitmapContext wBC
			(	w
			,	h
			,	8
			,	v.step[ 0 ]
			,	Apple::DeviceGrayColorSpace()
			,	kCGImageAlphaNone
			,	v.data
			);
			CGContextDrawImage( wBC, CGRectMake( 0, 0, w, h ), p );
			return v;
		}

		inline	Apple::Image
		GrayImage( cv::Mat const& p )
		{	assert( p.elemSize() == 1 );
			return Apple::Image
			(	Apple::BitmapContext
				(	p.cols
				,	p.rows
				,	8
				,	p.step[ 0 ]
				,	Apple::DeviceGrayColorSpace()
				,	kCGImageAlphaNone
				,	p.data
				)
			);
		}
	}
}

