#pragma once
//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

#include	"Generic.h"
#include	<Accelerate/Accelerate.h>

//	In older OSX version of Accelerate framework
//	Functions below can't take const float* as 1st or 3rd parameter
//	vDSP_mmov
//	vDSP_vneg
//	vDSP_vsadd
//	vDSP_vsdiv
//
//	In this case I must de-const using macros below
//
//	#if	TARGET_OS_IPHONE
//	#define	UNCONST( p )	p
//	#else
//	#define	UNCONST( p )	((float*)p)
//	#endif

namespace
JpMooParaiso
{	template	<size_t N>	struct
	VectorF
	{	float	u[ N ];
		VectorF()
		{	vDSP_vclr( u, 1, N );
		}
		VectorF( const float* const& p )
		{	vDSP_mmov( p, u, N, 1, N, N );
		}
		VectorF( VectorF<N> const& p )
		{	vDSP_mmov( p.u, u, N, 1, N, N );
		}
		VectorF	const& operator
		=( VectorF const& p )
		{	vDSP_mmov( p.u, u, N, 1, N, N );
			return p;
		}
		VectorF	operator
		-() const
		{	VectorF v;
			vDSP_vneg( u, 1, v.u, 1, N );
			return v;
		}
		void operator
		+=( float p )
		{	vDSP_vsadd( u, 1, &p, u, 1, N );
		}
		void operator
		*=( float p )
		{	vDSP_vsmul( u, 1, &p, u, 1, N );
		}
		void operator
		/=( float p )
		{	vDSP_vsdiv( u, 1, &p, u, 1, N );
		}
		void
		Dump() const
		{	for ( size_t n = 0; n < N; n++ ) printf( " %8.3f", (float)u[ n ] );
			printf( "\n" );
		}
	};

	template	<size_t N>	bool	operator
	==( VectorF<N> const& p1, VectorF<N> const& p2 )
	{	for ( size_t i = 0; i < N; i++ ) if ( p1.u[ i ] != p2.u[ i ] ) return false;
		return true;
	}

	template	<size_t N>	bool	operator
	!=( VectorF<N> const& p1, VectorF<N> const& p2 )
	{	for ( size_t i = 0; i < N; i++ ) if ( p1.u[ i ] != p2.u[ i ] ) return true;
		return false;
	}
//	Scalar
	template	<size_t N>	VectorF<N>	operator
	+( VectorF<N> const& pVec, float p )
	{	VectorF<N>	v;
		vDSP_vsadd( pVec.u, 1, &p, v.u, 1, N );
		return v;
	}
	template	<size_t N>	VectorF<N>	operator
	*( VectorF<N> const& pVec, float const& p )
	{	VectorF<N>	v;
		vDSP_vsmul( pVec.u, 1, &p, v.u, 1, N );
		return v;
	}
	template	<size_t N>	VectorF<N>	operator
	/( VectorF<N> const& pVec, float const& p )
	{	VectorF<N>	v;
		vDSP_vsdiv( pVec.u, 1, &p, v.u, 1, N );
		return v;
	}

//	Vector
	template	<size_t N>	VectorF<N>	operator
	+( VectorF<N> const& pL, VectorF<N> const& pR )
	{	VectorF<N>	v;
		vDSP_vadd( pR.u, 1, pL.u, 1, v.u, 1, N );
		return v;
	}

	template	<size_t N>	VectorF<N>	operator
	-( VectorF<N> const& pL, VectorF<N> const& pR )
	{	VectorF<N>	v;
		vDSP_vsub( pR.u, 1, pL.u, 1, v.u, 1, N );
		return v;
	}

	template	<size_t N>	VectorF<N>	operator
	*( VectorF<N> const& pL, VectorF<N> const& pR )
	{	VectorF<N>	v;
		vDSP_vmul( pR.u, 1, pL.u, 1, v.u, 1, N );
		return v;
	}

	template	<size_t N>	float		operator
	&( VectorF<N> const& pL, VectorF<N> const& pR )
	{	float	v;
		vDSP_dotpr( pR.u, 1, pL.u, 1, &v, N );
		return v;
	}

	inline	VectorF<3>	operator
	^( VectorF<3> const& p1, VectorF<3> const& p2 )
	{	VectorF<3>	v;
		v.u[ 0 ] = p1.u[ 1 ] * p2.u[ 2 ] - p1.u[ 2 ] * p2.u[ 1 ];
		v.u[ 1 ] = p1.u[ 2 ] * p2.u[ 0 ] - p1.u[ 0 ] * p2.u[ 2 ];
		v.u[ 2 ] = p1.u[ 0 ] * p2.u[ 1 ] - p1.u[ 1 ] * p2.u[ 0 ];
		return v;
	}

	template	<size_t R, size_t C>	struct
	MatrixF
	{	float	u[ R * C ];
		MatrixF()
		{	for ( size_t i = 0; i < R * C; i++ ) u[ i ] = 0;
		}
		MatrixF( const float* const p )
		{	for ( size_t i = 0; i < R * C; i++ ) u[ i ] = p[ i ];
		}
		MatrixF( MatrixF const& p )
		{	for ( size_t i = 0; i < R * C; i++ ) u[ i ] = p.u[ i ];
		}
		MatrixF const&
		operator =( MatrixF const& p )
		{	for ( size_t i = 0; i < R * C; i++ ) u[ i ] = p.u[ i ];
			return p;
		}
		void
		Dump() const
		{	for ( size_t r = 0; r < R; r++ )
			{	size_t	w = r * C;
				for ( size_t c = 0; c < C; c++ ) printf( " %8.3f", (float)u[ w + c ] );
				printf( "\n" );
			}
		}
		VectorF<C>
		Row( size_t p ) const
		{	VectorF<C>	v;
			for ( size_t c = 0, i = p * C; c < C; c++, i++ ) v.u[ c ] = u[ i ];
			return v;
		}
		VectorF<R>
		Col( size_t p ) const
		{	VectorF<R>	v;
			for ( size_t r = 0, i = p; r < R; r++, i += C ) v.u[ r ] = u[ i ];
			return v;
		}
		float&
		operator ()( size_t pR, size_t pC )
		{	return u[ pR * C + pC ];
		}
	};

	template	<size_t R, size_t C>	MatrixF<R, C>
	operator +( MatrixF<R, C> const& p1, MatrixF<R, C> const& p2 )
	{	MatrixF<R, C>	v;
		for ( size_t i = 0; i < R * C; i++ ) v.u[ i ] = p1.u[ i ] + p2.u[ i ];
		return v;
	}

	template	<size_t R, size_t C>	MatrixF<R, C>
	operator *( MatrixF<R, C> const& pMat, float const& p )
	{	MatrixF<R, C>	v;
		for ( size_t i = 0; i < R * C; i++ ) v.u[ i ] = pMat.u[ i ] * p;
		return v;
	}

	template	<size_t R, size_t C>	MatrixF<R, C>
	operator *( float const& p, MatrixF<R, C> const& pMat )
	{	MatrixF<R, C>	v;
		for ( size_t i = 0; i < R * C; i++ ) v.u[ i ] = p * pMat.u[ i ];
		return v;
	}

	template	<size_t R, size_t N, size_t C>	MatrixF<R, C>
	operator *( MatrixF<R, N> const& p1, MatrixF<N, C> const& p2 )
	{	MatrixF<R, C>	v;
		for ( size_t r = 0; r < R; r++ )
		{	size_t	w = r * C;
			for ( size_t c = 0; c < C; c++ ) v.u[ w + c ] = p1.Row( r ) * p2.Col( c );
		}
		return v;
	}

	template	<size_t R, size_t C>	VectorF<C>
	operator *( VectorF<R> const& pVec, MatrixF<R, C> const& pMat )
	{	VectorF<C>	v;
		for ( size_t c = 0; c < C; c++ ) v.u[ c ] = pVec * pMat.Col( c );
		return v;
	}

	template	<size_t R, size_t C>	VectorF<R>
	operator *( MatrixF<R, C> const& pMat, VectorF<C> const& pVec )
	{	VectorF<R>	v;
		for ( size_t r = 0; r < R; r++ ) v.u[ r ] = pMat.Row( r ) * pVec;
		return v;
	}

////////////////////////////////////////////////////////////////

	inline	VectorF<2>
	VectorF2( float const& pX = 0, float const& pY = 0 )
	{	VectorF<2> v;
		v.u[ 0 ] = pX;
		v.u[ 1 ] = pY;
		return v;
	}

	inline	VectorF<3>
	VectorF3( float const& pX = 0, float const& pY = 0, float const& pZ = 0 )
	{	VectorF<3> v;
		v.u[ 0 ] = pX;
		v.u[ 1 ] = pY;
		v.u[ 2 ] = pZ;
		return v;
	}

////////////////////////////////////////////////////////////////

	struct
	XY
	{	int	uX;
		int	uY;
		XY( int pX = 0, int pY = 0 )
		: uX( pX )
		, uY( pY )
		{
		}
	};
	inline	bool operator
	==( XY const& pL, XY const& pR )
	{	return pL.uX == pR.uX && pL.uY == pR.uY;
	}

	inline	bool operator
	!=( XY const& pL, XY const& pR )
	{	return pL.uX != pR.uX || pL.uY != pR.uY;
	}

	struct
	AARect
	{	bool	uUndef;
		int		uMinX;
		int		uMaxX;
		int		uMinY;
		int		uMaxY;
		AARect()
		: uUndef( true )
		{
		}
		AARect( XY const& p1, XY const& p2 )
		: uUndef( false )
		{	uMinX = p1.uX < p2.uX ? p1.uX : p2.uX;
			uMaxX = p1.uX > p2.uX ? p1.uX : p2.uX;
			uMinY = p1.uY < p2.uY ? p1.uY : p2.uY;
			uMinY = p1.uY > p2.uY ? p1.uY : p2.uY;
		}
	};

	inline	AARect	operator
	| ( AARect const& p1, AARect const& p2 )
	{	if ( p1.uUndef )	return p2;
		if ( p2.uUndef )	return p1;
		AARect	v;
		v.uMinX = p1.uMinX < p2.uMinX ? p1.uMinX : p2.uMinX;
		v.uMaxX = p1.uMaxX > p2.uMaxX ? p1.uMaxX : p2.uMaxX;
		v.uMinY = p1.uMinY < p2.uMinY ? p1.uMinY : p2.uMinY;
		v.uMaxY = p1.uMaxY > p2.uMaxY ? p1.uMaxY : p2.uMaxY;
		v.uUndef = false;
		return v;
	}

	inline	AARect	operator
	& ( AARect const& p1, AARect const& p2 )
	{	if ( p1.uUndef )	return p1;
		if ( p2.uUndef )	return p2;
		AARect	v;
		v.uMinX = p1.uMinX > p2.uMinX ? p1.uMinX : p2.uMinX;
		v.uMaxX = p1.uMaxX < p2.uMaxX ? p1.uMaxX : p2.uMaxX;
		if ( v.uMinX >= v.uMaxX ) return v;	//	undef
		v.uMinY = p1.uMinY > p2.uMinY ? p1.uMinY : p2.uMinY;
		v.uMaxY = p1.uMaxY < p2.uMaxY ? p1.uMaxY : p2.uMaxY;
		if ( v.uMinY >= v.uMaxY ) return v;	//	undef
		v.uUndef = false;
		return v;
	}

	enum
	Direction
	{	eR	= 1
	,	eU	= 2
	,	eL	= 4
	,	eD	= 8
	};
	struct
	PathElement : XY
	{	Direction		uDir;
		PathElement( XY const& p, Direction pDir )
		: XY( p )
		, uDir( pDir )
		{
		}
		PathElement()
		{
		}
	};

	typedef	Array<PathElement>	Contour;

	inline	bool
	Outer( Contour const& p )
	{	switch ( p.u[ 0 ].uDir )
		{
		case eR:	return true;
		case eU:	return false;
		default:	assert( false );
		}
	}
	inline	bool
	Inner( Contour const& p )
	{	switch ( p.u[ 0 ].uDir )
		{
		case eR:	return false;
		case eU:	return true;
		default:	assert( false );
		}
	}

	inline	AARect
	BoundingBox( Contour const& p )
	{	AARect v( p.u[ 0 ], p.u[ 0 ] );
		for ( size_t i = 1; i < p.uSize; i++ )
		{	if ( p.u[ i ].uX < v.uMinX ) v.uMinX = p.u[ i ].uX;
			if ( p.u[ i ].uX > v.uMaxX ) v.uMaxX = p.u[ i ].uX;
			if ( p.u[ i ].uY < v.uMinY ) v.uMinY = p.u[ i ].uY;
			if ( p.u[ i ].uY > v.uMaxY ) v.uMaxY = p.u[ i ].uY;
		}
		return v;
	}

	inline	int
	WindingLevel( Contour const& p, XY const& pTarget )
	{	int	v = 0;
		int	wX = pTarget.uX;
		int	wY = pTarget.uY;
		for ( size_t i = 0; i < p.uSize; i++ )
		{
			int wPrevY = p.u[ i ? i -1 : p.uSize - 1 ].uY;
			PathElement const& w = p.u[ i ];
			switch ( w.uDir )
			{
			case eU:
				if ( wPrevY <= wY && wY < w.uY && w.uX <= wX ) v--;
				break;
			case eD:
				if ( wPrevY > wY && wY >= w.uY && w.uX <= wX ) v++;
				break;
			default:	break;
			}
		}
		return v;
	}

	template	<typename F>	Contour
	DetectContour( F pCB, XY const& p, Direction pDir ) //	bool (^pCB)( XY const& )
	{
		Contour	v;

		XY	w( p );

		do
		{	Direction wOldDir = pDir;
			switch ( pDir )
			{
			case eL:
				w.uX--;
				if		( ! pCB( XY( w.uX - 1, w.uY - 1 ) ) )	pDir = eD;
				else if	(   pCB( XY( w.uX - 1, w.uY     ) ) )	pDir = eU;
				break;
			case eD:
				w.uY--;
				if ( ( w.uY < p.uY ) || ( w.uY == p.uY && w.uX < p.uX )	)	throw 0;
				if		( ! pCB( XY( w.uX    , w.uY - 1 ) ) )	pDir = eR;
				else if	(   pCB( XY( w.uX - 1, w.uY - 1 ) ) )	pDir = eL;
				break;
			case eR:
				w.uX++;
				if		( ! pCB( w ) )										pDir = eU;
				else if (   pCB( XY( w.uX    , w.uY - 1 ) ) )	pDir = eD;
				break;
			case eU:
				w.uY++;
				if		( ! pCB( XY( w.uX - 1, w.uY     ) ) )	pDir = eL;
				else if	(   pCB( w ) )										pDir = eR;
				break;
			}
			if ( pDir != wOldDir ) v.Append( PathElement( w, wOldDir ) );
		} while ( w != p );

		return v;
	}

	typedef	List<Contour>	Shape;

	inline	int
	WindingLevel( Shape const& p, XY const& pTarget )
	{	int	v = 0;
		p.RunInOrder( [ & ]( Contour const& p ) { v += WindingLevel( p, pTarget ); } );
		return v;
	}

	inline	void
	AdvZ( XY& p, int pMaxX )
	{	if ( ++p.uX < pMaxX ) return;
		p.uX = 0;
		p.uY++;
		return;
	}

	template	<typename F>	Shape
	DetectShape( int pMaxX, int pMaxY, F pCB )	//	bool (^pCB)( XY const& ) )
	{
		auto	wCB = [ pMaxX, pMaxY, pCB ]( XY const& p )->bool
		{	return
				p.uX < 0
			||	p.uX >= pMaxX
			||	p.uY < 0
			||	p.uY >= pMaxY
			?	false
			:	pCB( p )
			;
		};
#ifdef	SATURN_G_CONTOURS_ASYNC
		static	dispatch_semaphore_t	sSemaphore = ::dispatch_semaphore_create( 1 );

		dispatch_group_t wG = ::dispatch_group_create();
		dispatch_queue_t wQ = ::dispatch_get_global_queue( DISPAfloatCH_QUEUE_PRIORIfloatY_DEFAULfloat, 0 );
		__block	Shape v;
#else
		Shape v;
#endif
		for ( XY w; w.uY < pMaxY; AdvZ( w, pMaxX ) )
		{	if ( pCB( w ) )	//	No need to check range, so call pCB
			{	if
				(	! wCB( XY( w.uX - 1, w.uY     ) )
				&&	! wCB( XY( w.uX    , w.uY - 1 ) )
				)
#ifdef	SATURN_G_CONTOURS_ASYNC
				{	dispatch_group_async
					(	wG
					,	wQ
					,	^
#endif
						{	try
							{
#ifdef	SATURN_G_CONTOURS_ASYNC
								Contour const& wContour = DetectContour( wCB, w, eR );
								dispatch_semaphore_wait( sSemaphore, DISPAfloatCH_floatIME_FOREVER );
								v.Prepend( wContour );
								dispatch_semaphore_signal( sSemaphore );
#else
								v.Prepend( DetectContour( wCB, w, eR ) );
#endif
							}
							catch (...)	{}
						}
#ifdef	SATURN_G_CONTOURS_ASYNC
					);
				}
#endif
			}
			else
			{	if
				(	wCB( XY( w.uX - 1, w.uY     ) )
				&&	wCB( XY( w.uX    , w.uY - 1 ) )
				&&	wCB( XY( w.uX - 1, w.uY - 1 ) )
				)
#ifdef	SATURN_G_CONTOURS_ASYNC
				{	dispatch_group_async
					(	wG
					,	wQ
					,	^
#endif
						{	try
							{
#ifdef	SATURN_G_CONTOURS_ASYNC
								Contour const& wContour = DetectContour( wCB, w, eU );
								dispatch_semaphore_wait( sSemaphore, DISPAfloatCH_floatIME_FOREVER );
								v.Prepend( wContour );
								dispatch_semaphore_signal( sSemaphore );
#else
								v.Prepend( DetectContour( wCB, w, eU ) );
#endif
							}
							catch (...)	{}
						}
#ifdef	SATURN_G_CONTOURS_ASYNC
					);
				}
#endif
			}
		}

#ifdef	SATURN_G_CONTOURS_ASYNC
		dispatch_group_wait( wG, DISPAfloatCH_floatIME_FOREVER );
#if	!OS_OBJECT_USE_OBJC
		dispatch_release( wG );
#endif
#endif
		return v;
	}

	inline	AARect
	BoundingBox( Shape const& p )
	{
		AARect v;
		p.RunInOrder( [ & ]( Contour const& p ) { v = v | BoundingBox( p ); } );
		return v;
	}

	template	<typename F>	void
	ScanZ( Shape const& pShape, F pCB )	//	( XY const& )
	{	AARect wBBox = BoundingBox( pShape );
		Array<List<PathElement>> wEdgeList( wBBox.uMaxY - wBBox.uMinY );
		wEdgeList.uSize = wBBox.uMaxY - wBBox.uMinY;

		pShape.RunInOrder
		(	[ & ]( Contour const& pContour )
			{	for ( size_t i = 0; i < pContour.uSize; i++ )
				{	XY const& wPrev = pContour.u[ i == 0 ? pContour.uSize - 1 : i - 1 ];
					PathElement const& w = pContour.u[ i ];
					switch ( w.uDir )
					{
					case eU:
						for ( int y = wPrev.uY; y < w.uY; y++ )
						{	wEdgeList.u[ y - wBBox.uMinY ].Insert
							(	w
							,	[ & ]( PathElement const& p )
								{	return w.uX >= p.uX;
								}
							);
						}
						break;
					case eD:
						for ( int y = w.uY; y < wPrev.uY; y++ )
						{	wEdgeList.u[ y - wBBox.uMinY ].Insert
							(	w
							,	[ & ]( PathElement const& p )
								{	return w.uX >= p.uX;
								}
							);
						}
						break;
					default:
						break;
					}
				}
			}
		);

		XY v;
		for ( v.uY = 0; v.uY < wEdgeList.uSize; v.uY++ )
		{	int wWindingLevel = 0;
			int	wPrev = 0;
			wEdgeList.u[ v.uY ].RunInOrder
			(	[ & ]( PathElement const& p )
				{	if ( wWindingLevel ) for ( v.uX = wPrev; v.uX < p.uX; v.uX++ ) pCB( v );
					wPrev = p.uX;
					switch ( p.uDir )
					{
					case eD:	wWindingLevel++;	break;
					case eU:	wWindingLevel--;	break;
					default:	assert( false );
					}
				}
			);
		}
	}
/*
	inline	Contour
	Reshape( Contour const& p )
	{	assert( p.Size() >= 3 );

		Saturn::List<PathElement>	wL;
		Saturn::List<PathElement>	wR;
		Saturn::List<PathElement>	wD;
		Saturn::List<PathElement>	wU;

		PathElement wPrev = p( 0 );
		PathElement wCurr = p( 1 );
		size_t wNumElements = p.Size();
		for ( size_t i = 0; i < wNumElements; i++ )
		{	PathElement* wNext = ( i + 2 ) < wNumElements ? p( i + 2 ) : p( i + 2 - wNumElements );
			if ( wPrev->uDir == eD && wNext->uDir == eU ) wD.Add( wCurr );
			if ( wPrev->uDir == eU && wNext->uDir == eD ) wU.Add( wCurr );
			if ( wPrev->uDir == eL && wNext->uDir == eR ) wL.Add( wCurr );
			if ( wPrev->uDir == eR && wNext->uDir == eL ) wR.Add( wCurr );
			wPrev = wCurr;
			wCurr = wNext;
		}
		return p;
	}

	inline	Array<Contour>
	Reshape( Saturn::Array<Contour> const& p )
	{	Saturn::Array<Contour>	v;
		size_t wNumContours = p.Size();
		for ( size_t i = 0; i < wNumContours; i++ )
		{	try
			{	v.Add( Reshape( p( i ) ) );
			}
			catch (...)
			{
			}
		}
		return v;
	}
*/
	struct	BezierPoints
	{	double	uX0;
		double	uY0;
		double	uX1;
		double	uY1;
		double	uX2;
		double	uY2;
		double	uX3;
		double	uY3;
#if	TARGET_OS_IPHONE
		UIBezierPath*
		BezierPath()
		{	UIBezierPath* v = UIBezierPath.bezierPath;
			[	v
				moveToPoint:CGPointMake( uX0, uY0 )
			];
			[	v
				addCurveToPoint:CGPointMake( uX3, uY3 )
				controlPoint1:CGPointMake( uX1, uY1 )
				controlPoint2:CGPointMake( uX2, uY2 )
			];
			return v;
		}
#endif
	};

	inline	BezierPoints
	ParabolaBezierPoints( double pA, double pB, double pC, double pP, double pQ )
	{	//	Returns bezier points which simulates parabola by given parameter.
		//	y - pC = pA( x - pB )^2 where [ pP, pQ ] of x.

		double	(^wEval)( double p ) = ^( double p )
		{	double w = p - pB;
			return pA * w * w + pC;
		};
		BezierPoints	v;
		v.uX0 = pP;
		v.uY0 = wEval( pP );
		double	wPQ = pP * pQ;
		double	wPP = pP * pP;
		double	wQQ = pQ * pQ;
		v.uX1 = ( ( wPQ - 2 * wPP + wQQ ) / 3 ) / ( pQ - pP );
		v.uX2 = ( ( wPQ + wPP - 2 * wQQ ) / 3 ) / ( pP - pQ );
		double	wK = pA * ( pQ + pP - 2 * pB );
		double	wN = pC + pA * ( pB * pB - ( wPP + wPQ + wQQ ) / 3 );
		v.uY1 = v.uX1 * wK + wN;
		v.uY2 = v.uX2 * wK + wN;
		v.uX3 = pQ;
		v.uY3 = wEval( pQ );

		return v;
	}

	inline	BezierPoints
	ParabolaBezierPath_0_1( double pX, double pY )
	{	//	Returns bezier point which simulates parabola which have three point as below on it.
		//	( 0, 0 ), ( 1, 1 ), ( pX, pY )

		double	wA = ( pY - pX ) / ( pX * pX - pX );
		double	wB = ( wA - 1 ) / ( 2 * wA );
		double	wC = - ( wA * wB * wB );

		BezierPoints	v;
		v.uX0 = 0;
		v.uY0 = 0;
		v.uX1 = ( 1. / 3. );
		v.uX2 = ( 2. / 3. );
		double	wK = wA * ( 1 - 2 * wB );
		double	wN = wC + wA * ( wB * wB - 1. / 3. );
		v.uY1 = v.uX1 * wK + wN;
		v.uY2 = v.uX2 * wK + wN;
		v.uX3 = 1;
		v.uY3 = 1;
		return v;
	}
#if 0
	inline	void
	DrawParabola( CGContextRef pCR, double pK, double pX, double pY, double pP, double pQ )	//	頂点と K
	{	double	(^wEval)( double p ) = ^( double p )
		{	return pK * ( p - pX ) * ( p - pX ) + pY;
		};
		assert( pK != 0. );
	//	y - Y = K.( x - X )^2
	//	y = K.( x - X )^2 + Y
	//	y = K.x^2 - 2.K.X.x + K.X^2 + Y
		CGContextMoveToPoint( pCR, pP, wEval( pP ) );
		for ( int i = 1; i <= 160; i++ )
		{	double x = pP + i * ( pQ - pP ) / 160;
			double y = wEval( x );
			CGContextAddLineToPoint( pCR, x, y );
		}
		CGContextStrokePath( pCR );

	//	Tangent
	//	y - Y = K.( x - X )^2
	//	y = K.x^2 - 2.K.X.x + K.X.X + Y
	//	y' = 2.K.( x - X )
	//	at P
	//	Slant	: 2.K.( p - X )
	//	n		: K.( p - X ).( p - X ) + Y = 2.K.( p^2 - X.p ) + n
	//			: K.p^2 - 2.K.X.p + K.X^2 + Y = 2.K.p^2 - 2.K.X.p + n
	//			: K.X^2 + Y = K.p^2 + n
	//			: n = K.X^2 + Y - K.p^2
	//			: n = K.( X + p ).( X - p ) + Y
		{	double wSlant = 2 * pK * ( pP - pX );
			double wN = pK * ( pX + pP ) * ( pX - pP ) + pY;
#if	TARGET_OS_IPHONE
			CGContextSetStrokeColor( pCR, CGColorGetComponents( UIColor.blueColor.CGColor ) );
#else
			CGContextSetStrokeColor( pCR, CGColorGetComponents( NSColor.blueColor.CGColor ) );
#endif
			CGContextMoveToPoint( pCR, pP, wSlant * pP + wN );
			CGContextAddLineToPoint( pCR, pQ, wSlant * pQ + wN );
			CGContextStrokePath( pCR );
		}
		{	double wSlant = 2 * pK * ( pQ - pX );
			double wN = pK * ( pX + pQ ) * ( pX - pQ ) + pY;
#if	TARGET_OS_IPHONE
			CGContextSetStrokeColor( pCR, CGColorGetComponents( UIColor.blueColor.CGColor ) );
#else
			CGContextSetStrokeColor( pCR, CGColorGetComponents( NSColor.blueColor.CGColor ) );
#endif
			CGContextMoveToPoint( pCR, pP, wSlant * pP + wN );
			CGContextAddLineToPoint( pCR, pQ, wSlant * pQ + wN );
			CGContextStrokePath( pCR );
		}

	//	p->q
	//	Slant	: K.( q - X ).( q - X ) + Y - ( K.( p - X ).( p - X ) + Y ) / ( q - p )
	//			: K.( ( q - X ).( q - X ) - ( p - X ).( p - X ) ) / ( q - p )
	//			: K.( q^2 - p^2 - ( 2.q.X - 2.p.X ) ) / ( q - p )
	//			: K.( ( q + p ).( q - p ) - 2.X.( q - p ) ) / ( q - p )
	//			: K.( q + p - 2.X )
	//	K.( q - X ).( q - X ) + Y = K.( q + p - 2X ).q + n
	//	K.q^2 - 2K.X.q + K.X^2 + Y = K.q^2 + K.p.q - 2K.X.q + n
	//	K.X^2 + Y = K.p.q + n
	//	n = K.X^2 + Y - K.p.q

	//	The parallel to p->q line which touches the parabola
	//	Slant	: K.( q + p - 2.X )
	//	n		: K.x^2 - 2.K.X.x + K.X^2 + Y					= K.( q + p - 2.X ).x + n			of b^2 - 4ac == 0
	//			: x^2 - 2.X.x + X^2 - ( q + p - 2.X ).x			= ( n - Y ) / K
	//			: x^2 - ( q + p ).x + X^2		 				= ( n - Y ) / K
	//			: x^2 - ( q + p ).x + X^2 + ( Y - n ) / K		= 0
	//			: K.x^2 - K.( q + p ).x + K.X^2 + Y - n			= 0

	//	b^2-4ac	: K^2.( q + p )^2 - 4.K.( K.X^2 + Y - n )		= 0
	//			: K.( q + p )^2 / 4 - K.X^2 - Y + n )			= 0
	//			: K.( ( q + p )^2 / 4 - X^2 ) - Y + n )			= 0
	//			: Y - K.( ( q + p )^2 / 4 - X^2 )				= n

	//	nBase	= pK * pX * pX + pY - pK * pP * pQ
	//	nPQ		= pY - pK * ( ( pP + pQ ) * ( pP + pQ ) / 4 - pX * pX )
	//	nNew	= ( wNPQ * 4 - wNBase ) / 3

		{
	//		CGContextSetStrokeColor( pCR, CGColorGetComponents( UIColor.greenColor.CGColor ) );
	//		CGContextMoveToPoint( pCR, pP, wSlant * pP + wN );
	//		CGContextAddLineToPoint( pCR, pQ, wSlant * pQ + wN );
	//		CGContextStrokePath( pCR );
	//	CPs
			double wPQ = pP * pQ;
			double wPP = pP * pP;
			double wQQ = pQ * pQ;
			double x1 = ( ( wPQ - 2 * wPP + wQQ ) / 3 ) / ( pQ - pP );
			double x2 = ( ( wPQ + wPP - 2 * wQQ ) / 3 ) / ( pP - pQ );
			double wA = pK * ( pQ + pP - 2 * pX );
			double wB = pY + pK * ( pX * pX - ( wPP + wPQ + wQQ ) / 3 );
			double y1 = x1 * wA + wB;
			double y2 = x2 * wA + wB;
#if	TARGET_OS_IPHONE
			CGContextSetStrokeColor( pCR, CGColorGetComponents( UIColor.greenColor.CGColor ) );
#else
			CGContextSetStrokeColor( pCR, CGColorGetComponents( NSColor.greenColor.CGColor ) );
#endif
			CGContextMoveToPoint( pCR, x1, y1 );
			CGContextAddLineToPoint( pCR, x2, y2 );
			CGContextStrokePath( pCR );

			CGContextMoveToPoint( pCR, pP, wEval( pP ) );
			CGContextAddCurveToPoint( pCR, x1, y1, x2, y2, pQ, wEval( pQ ) );
			CGContextStrokePath( pCR );
		}


	//	CP1 -> CP2
	//	y = pK * ( pQ + pP - 2 * pX ) * x + pY + pK * pX * pX - pK * ( pP * pP + pP * pQ + pQ * pQ ) / 3
	//	Tangent 1
	//	y = 2 * pK * ( pP - pX ) * x + pK * ( pX + pP ) * ( pX - pP ) + pY
	//	y = 2 * pK * ( pQ - pX ) * x + pK * ( pX + pQ ) * ( pX - pQ ) + pY

	//	pK * ( pQ + pP - 2 * pX ) * x + pY + pK * pX * pX - pK * ( pP * pP + pP * pQ + pQ * pQ ) / 3
	//= 2 * pK * ( pP - pX ) * x + pK * ( pX + pP ) * ( pX - pP ) + pY

	//	( pK * ( pQ + pP - 2 * pX ) - 2 * pK * ( pP - pX ) ) * x
	//= pK * ( pX + pP ) * ( pX - pP ) + pY - pY - pK * pX * pX + pK * ( pP * pP + pP * pQ + pQ * pQ ) / 3

	//	3 * ( pQ - pP ) * x
	//= - 2 * pP * pP + pP * pQ + pQ * pQ

	//	Tangent 2
	//	y = 2 * pK * ( pQ - pX ) + K * ( pX + pQ ) * ( pX - pQ ) + pY
	}
#endif
}
