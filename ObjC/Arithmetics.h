#pragma once
//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

#include	"Generic.h"

#include	<iostream>
#include	<cmath>
#include	<complex>

namespace
JpMooParaiso
{
#ifdef	__LP64__
	inline	std::ostream&	operator
	<<( std::ostream& pStream, __int128_t const& p )
	{	for ( int i = 0; i < 16; i++ )
		{	pStream << "( ( " << ((int64_t*)&p)[ 0 ] << " << 64 ) + " << ((int64_t*)&p)[ 1 ] << ')';
		}
		return pStream;
	}
#endif

	template	<typename T>	T
	GCD( T p1, T p2 )
	{	T	w;
		while ( ( w = p1 % p2  ) )
		{	p1 = p2;
			p2 = w;
		}
		return p2;
	}

	template	<typename T>	T
	Log2N( T const& p )
	{	T	v = 1;
		while ( ( 1 << v ) < p ) v++;
		return v;
	}

#pragma	mark	Matrix

	template	<typename T, size_t RN, size_t CN>	struct
	Matrix
	{	T		u[ RN ][ CN ];
		
		Matrix<T, 1, CN>
		Row( size_t p ) const
		{	Matrix<T, 1, CN> v;
			for ( size_t i = 0; i < CN; i++ ) v.u[ 0 ][ i ] = u[ p ][ i ];
			return v;
		}
		
		Matrix<T, RN, 1>
		Col( size_t p ) const
		{	Matrix<T, RN, 1> v;
			for ( size_t i = 0; i < RN; i++ ) v.u[ i ][ 0 ] = u[ i ][ p ];
			return v;
		}
	};

	template	<typename T, size_t RN, size_t CN>	void	operator
	>> ( T pSrc[ RN ][ CN ], Matrix<T, RN, CN >& p )
	{	for ( size_t r = 0; r < RN; r++ )
		{	for ( size_t c = 0; c < CN; c++ )
			{	p.u[ r ][ c ] = pSrc[ r ][ c ];
			}
		}
	}
	template	<typename T, size_t RN, size_t CN>	std::ostream&	operator
	<< ( std::ostream& pStream, Matrix<T, RN, CN> const& p )
	{	{	for ( size_t r = 0; r < RN; r++ )
			{	for ( size_t c = 0; c < CN; c++ )
				{	std::cout << p.u[ r ][ c ] << ' ';
				}
				std::cout << std::endl;
			}
		}
		return pStream;
	}

	template	<typename T, size_t RN, size_t CN>	bool	operator
	== ( Matrix<T, RN, CN> const& pL, Matrix<T, RN, CN> const& pR )
	{	for ( size_t r = 0; r < RN; r++ )
		{	for ( size_t c = 0; c < CN; c++ )
			{	if ( pL.u[ r ][ c ] != pR.u[ r ][ c ] ) return false;
			}
		}
		return true;
	}

	template	<typename T, size_t RN, size_t CN>	Matrix<T, RN, CN>	operator
	+ ( Matrix<T, RN, CN> const& pL, Matrix<T, RN, CN> const& pR )
	{	Matrix<T, RN, CN> v;
		{	for ( size_t r = 0; r < RN; r++ )
			{	for ( size_t c = 0; c < CN; c++ )
				{	v.u[ r ][ c ] = pL.u[ r ][ c ] + pR.u[ r ][ c ];
				}
			}
		}
		return v;
	}

	template	<typename T, size_t RN, size_t CN>	Matrix<T, RN, CN>	operator
	- ( Matrix<T, RN, CN> const& pL, Matrix<T, RN, CN> const& pR )
	{	Matrix<T, RN, CN> v;
		{	for ( size_t r = 0; r < RN; r++ )
			{	for ( size_t c = 0; c < CN; c++ )
				{	v.u[ r ][ c ] = pL.u[ r ][ c ] - pR.u[ r ][ c ];
				}
			}
		}
		return v;
	}

	template	<typename T, size_t RN, size_t CN>	Matrix<T, RN, CN>	operator
	* ( Matrix<T, RN, CN> const& p, T const& pScalar )
	{	Matrix<T, RN, CN> v;
		for ( size_t r = 0; r < RN; r++ )
		{	for ( size_t c = 0; c < CN; c++ ) v.u[ r ][ c ] = p.u[ r ][ c ] * pScalar;
		}
		return v;
	}

	template	<typename T, size_t RN, size_t CN>	Matrix<T, RN, CN>	operator
	* ( T const& pScalar, Matrix<T, RN, CN> const& p )
	{	Matrix<T, RN, CN> v;
		for ( size_t r = 0; r < RN; r++ )
		{	for ( size_t c = 0; c < CN; c++ ) v.u[ r ][ c ] = p.u[ r ][ c ] * pScalar;
		}
		return v;
	}


	template	<typename T, size_t RN, size_t N, size_t CN>	Matrix<T, RN, CN>	operator
	* ( Matrix<T, RN, N> const& pL, Matrix<T, N, CN> const& pR )
	{	Matrix<T, RN, CN> v;
		for ( size_t r = 0; r < RN; r++ )
		{	for ( size_t c = 0; c < CN; c++ )
			{	v.u[ r ][ c ] = 0;
				for ( size_t i = 0; i < N; i++ )
				{	v.u[ r ][ c ] += pL.u[ r ][ i ] * pR.u[ i ][ c ];
				}
			}
		}
		return v;
	}

	template	<typename T, size_t N>	Matrix<T, N, N>	operator
	! ( Matrix<T, N, N> p )
	{	Matrix<T, N, N> v;

		for( size_t i = 0; i < N; i++ )
		{	for( size_t j = 0; j < N; j++ ) v.u[ i ][ j ] = i == j ? 1 : 0;
		}
		for ( size_t i = 0; i < N; i++ )
		{	T wII = p.u[ i ][ i ];
			for( size_t c = 0; c < N; c++ )
			{	p.u[ i ][ c ] /= wII;
				v.u[ i ][ c ] /= wII;
			}
			for ( size_t r = 0; r < N; r++ )
			{	if ( r != i )
				{	T wRI = p.u[ r ][ i ];
					for ( size_t c = 0; c < N; c++ )
					{	p.u[ r ][ c ] -= wRI * p.u[ i ][ c ];
						v.u[ r ][ c ] -= wRI * v.u[ i ][ c ];
					}
				}
			}
		}
		return v;
	}

#pragma	mark	Rational

	template	<typename T>	struct
	Rational
	{
		T	uD;
		T	uN;

		Rational( T pN = 0, T pD = 1 )
		:	uN( pN )
		,	uD( pD )
		{	assert( pD );
			T	w = GCD( pN, pD );
			uN /= w;
			uD /= w;
		}

		operator double() const
		{	return 	(double)uN / (double)uD;
		}

		void	operator
		+= ( Rational const& p );
		void	operator
		-= ( Rational const& p );
		void	operator
		*= ( Rational const& p );
		void	operator
		/= ( Rational const& p );
	};
	template	<typename T>	std::ostream&	operator
	<< ( std::ostream& pStream, Rational<T> const& p )
	{	switch ( p.uD )
		{
		case  1:	pStream <<   p.uN;	break;
		case -1:	pStream << - p.uN;	break;
		default:	pStream << p.uN << '/' << p.uD;	break;
		}
		return pStream;
	}

	template	<typename T>	int
	Compare( Rational<T> const& pL, Rational<T> const& pR )
	{	return Compare( pL.uN * pR.uD, pR.uN * pL.uD );
	}

	template	<typename T>	Rational<T>	operator
	== ( Rational<T> const& pL, Rational<T> const& pR )
	{	return pL.uN * pR.uD == pR.uN * pL.uD;
	}
	
	template	<typename T>	Rational<T>	operator
	+ ( Rational<T> const& pL, Rational<T> const& pR )
	{	return pL.uD == pR.uD
		?	Rational<T>( pL.uN + pR.uN, pL.uD )
		:	Rational<T>( pL.uN * pR.uD + pR.uN * pL.uD, pL.uD * pR.uD )
		;
	}
	template	<typename T>	Rational<T>	operator
	- ( Rational<T> const& pL, Rational<T> const& pR )
	{	return pL.uD == pR.uD
		?	Rational<T>( pL.uN - pR.uN, pL.uD )
		:	Rational<T>( pL.uN * pR.uD - pR.uN * pL.uD, pL.uD * pR.uD )
		;
	}
	template	<typename T>	Rational<T>	operator
	* ( Rational<T> const& pL, Rational<T> const& pR )
	{	return Rational<T>( pL.uN * pR.uN, pL.uD * pR.uD );
	}
	template	<typename T>	Rational<T>	operator
	/ ( Rational<T> const& pL, Rational<T> const& pR )
	{	return Rational<T>( pL.uN * pR.uD, pL.uD * pR.uN );
	}
	template	<typename T>	void	Rational<T>::operator
	+= ( Rational<T> const& p )
	{	*this = *this + p;
	}

	template	<typename T>	void	Rational<T>::operator
	-= ( Rational const& p )
	{	*this = *this - p;
	}

	template	<typename T>	void	Rational<T>::operator
	*= ( Rational const& p )
	{	*this = *this * p;
	}

	template	<typename T>	void	Rational<T>::operator
	/= ( Rational const& p )
	{	*this = *this / p;
	}

#pragma	mark	P2

	template	<typename T>	struct
	P2
	{	Rational<T>	u;
		T			uP2;
		P2( Rational<T> const& p, T const& pP2 = 0 )
		:	u( p )
		,	uP2( pP2 )
		{
			if ( u.uN )
			{
				while ( u.uN % 2 == 0 )
				{	u.uN /= 2;
					uP2++;
				}
				if ( u.uD )
				{	while ( u.uD % 2 == 0 )
					{	u.uD /= 2;
						uP2--;
					}
				}
			}
			else
			{	uP2 = 0;
			}
		}
		P2( T const& pN = 0, T const& pD = 1, T const& pP2 = 0 )
		:	P2( Rational<T>( pN, pD ), pP2 )
		{
		}
		operator double() const
		{	return (double)u * pow( 2, uP2 );
//			return ( uP2 < 0 )
//			?	(double)u / ( 1 << - uP2 )
//			:	(double)u * ( 1 <<   uP2 )
//			;
		}

		void	operator
		+= ( P2 const& pL );
		void	operator
		-= ( P2 const& pL );
		void	operator
		*= ( P2 const& pL );
		void	operator
		/= ( P2 const& pL );
	};

	template	<typename T>	std::ostream&	operator
	<< ( std::ostream& pStream, P2<T> const& p )
	{	pStream << p.u << '(' << p.uP2 << ')';
		return pStream;
	}

	template	<typename T>	int
	Compare( P2<T> const& pL, P2<T> const& pR )
	{	switch ( Compare( pL.uP2, pR.uP2 ) )
		{
		case  0:	return Compare( pL.u, pR.u );
		case -1:	return Compare( pL.u.uN * pR.u.uD, pR.u.uN * pL.u.uD << pR.uP2 - pL.uP2 );
		case  1:	return Compare( pL.u.uN * pR.u.uD << pL.uP2 - pR.uP2, pR.u.uN * pL.u.uD );
		}
		assert( false );
	}

	template	<typename T>	P2<T>	operator
	== ( P2<T> const& pL, P2<T> const& pR )
	{	switch ( Compare( pL.uP2, pR.uP2 ) )
		{
		case  0:	return pL.u == pR.u;
		case -1:	return pL.u.uN * pR.u.uD == pR.u.uN * pL.u.uD << pR.uP2 - pL.uP2;
		case  1:	return pL.u.uN * pR.u.uD << pL.uP2 - pR.uP2 == pR.u.uN * pL.u.uD;
		}
		assert( false );
	}

//	2^a * n1 / d1 + 2^b * n2 / d2
//	( 2^a * n1 * d2 + 2^b * n2 * d1 ) / d1 * d2

	template	<typename T>	P2<T>	operator
	+ ( P2<T> const& pL, P2<T> const& pR )
	{	switch ( Compare( pL.uP2, pR.uP2 ) )
		{
		case  0:
			return P2<T>( pL.u + pR.u, pL.uP2 );
		case -1:
			return P2<T>
			(	pL.u.uN * pR.u.uD + ( pR.u.uN * pL.u.uD << pR.uP2 - pL.uP2 )
			,	pL.u.uD * pR.u.uD
			,	pL.uP2
			);
		case  1:
			return P2<T>
			(	( pL.u.uN * pR.u.uD << pL.uP2 - pR.uP2 ) + pR.u.uN * pL.u.uD
			,	pL.u.uD * pR.u.uD
			,	pR.uP2
			);
		}
		assert( false );
	}
	template	<typename T>	P2<T>	operator
	- ( P2<T> const& pL, P2<T> const& pR )
	{	switch ( Compare( pL.uP2, pR.uP2 ) )
		{
		case  0:
			return P2<T>( pL.u - pR.u, pL.uP2 );
		case -1:
			return P2<T>
			(	pL.u.uN * pR.u.uD - ( pR.u.uN * pL.u.uD << pR.uP2 - pL.uP2 )
			,	pL.u.uD * pR.u.uD
			,	pL.uP2
			);
		case  1:
			return P2<T>
			(	( pL.u.uN * pR.u.uD << pL.uP2 - pR.uP2 ) - pR.u.uN * pL.u.uD
			,	pL.u.uD * pR.u.uD
			,	pR.uP2
			);
		}
		assert( false );
	}
	template	<typename T>	P2<T>	operator
	* ( P2<T> const& pL, P2<T> const& pR )
	{	return P2<T>( pL.u * pR.u, pL.uP2 + pR.uP2 );
	}
	template	<typename T>	P2<T>	operator
	/ ( P2<T> const& pL, P2<T> const& pR )
	{	return P2<T>( pL.u / pR.u, pL.uP2 - pR.uP2 );
	}
	template	<typename T>	void	P2<T>::operator
	+= ( P2<T> const& pL )
	{	*this = *this + pL;
	}

	template	<typename T>	void	P2<T>::operator
	-= ( P2 const& pL )
	{	*this = *this - pL;
	}

	template	<typename T>	void	P2<T>::operator
	*= ( P2 const& pL )
	{	*this = *this * pL;
	}

	template	<typename T>	void	P2<T>::operator
	/= ( P2 const& pL )
	{	*this = *this / pL;
	}

#pragma	mark	Complex

	template	<typename T>	struct
	Complex
	{	T	uR;
		T	uI;
		Complex( T pR = T( 0 ), T pI = T( 0 ) )
		:	uR( pR )
		,	uI( pI )
		{
		}
		
		void	operator
		+= ( Complex const& p );
		void	operator
		-= ( Complex const& p );
		void	operator
		*= ( Complex const& p );
		void	operator
		/= ( Complex const& p );
	};

	template	<typename T>	std::ostream&	operator
	<< ( std::ostream& pStream, Complex<T> const& p )
	{	pStream << '(' << p.uR << "+i*" << p.uI << ')';
		return pStream;
	}

	template	<typename T>	Complex<T>	operator
	+ ( Complex<T> const& pL, Complex<T> const& pR )
	{	return Complex<T>
		(	pL.uR + pR.uR
		,	pL.uI + pR.uI
		);
	}

	template	<typename T>	Complex<T>	operator
	- ( Complex<T> const& pL, Complex<T> const& pR )
	{	return Complex<T>
		(	pL.uR - pR.uR
		,	pL.uI - pR.uI
		);
	}

	template	<typename T>	Complex<T>	operator
	* ( Complex<T> const& pL, Complex<T> const& pR )
	{	return Complex<T>
		(	pL.uR * pR.uR - pL.uI * pR.uI
		,	pL.uR * pR.uI + pL.uI * pR.uR
		);
	}

	template	<typename T>	Complex<T>	operator
	/ ( Complex<T> const& pL, Complex<T> const& pR )
	{	T	w = pR.uR * pR.uR + pR.uI * pR.uI;
		return Complex<T>
		(	( pL.uR * pR.uR - pL.uI * pR.uI ) / w
		,	( pR.uR * pL.uI - pL.uR * pR.uI ) / w
		);
	}

	template	<typename T>	void	Complex<T>::operator
	+= ( Complex<T> const& p )
	{	*this = *this + p;
	}
	template	<typename T>	void	Complex<T>::operator
	-= ( Complex const& p )
	{	*this = *this - p;
	}
	template	<typename T>	void	Complex<T>::operator
	*= ( Complex const& p )
	{	*this = *this * p;
	}
	template	<typename T>	void	Complex<T>::operator
	/= ( Complex const& p )
	{	*this = *this / p;
	}
	
	template<typename T>	Complex<T>
	Exp( Complex<T> const& p )
	{	T e = exp( p.uR );
		return Complex<T>( e * cos( p.uI ), e * sin( p.uI ) );
	}

	template<typename T>	Complex<T>
	Conj( Complex<T> const& p )
	{	return Complex<T>( p.uR, - p.uI );
	}

/*
template<class _Tp>
complex<_Tp>
exp(const complex<_Tp>& __x)
{
    _Tp __i = __x.imag();
    if (isinf(__x.real()))
    {
        if (__x.real() < _Tp(0))
        {
            if (!isfinite(__i))
                __i = _Tp(1);
        }
        else if (__i == 0 || !isfinite(__i))
        {
            if (isinf(__i))
                __i = _Tp(NAN);
            return complex<_Tp>(__x.real(), __i);
        }
    }
    else if (isnan(__x.real()) && __x.imag() == 0)
        return __x;
    _Tp __e = exp(__x.real());
    return complex<_Tp>(__e * cos(__i), __e * sin(__i));
}
*/
#pragma	mark	DFT

	template	<typename T>	Array<Complex<T>>
	DFT( Array<Complex<T>> const& p )
	{	Array<Complex<T>> v( p.uSize, true );
		for ( size_t n = 0; n < p.uSize; n++ )
		{	T ω = 2 * M_PI * n / p.uSize;
			for ( size_t k = 0; k < p.uSize; k++ )
			{	T Θ = ω * k;
				v.u[ n ].uR += p.u[ k ].uR * cos( Θ ) + p.u[ k ].uI * sin( Θ );
				v.u[ n ].uI += p.u[ k ].uI * cos( Θ ) - p.u[ k ].uR * sin( Θ );
			}
		}
		return v;
	}

	template	<typename T>	Array<Complex<T>>
	IDFT( Array<Complex<T>> const& p )
	{	Array<Complex<T>> v( p.uSize, true );
		for ( size_t n = 0; n < 8; n++ )
		{	T ω = 2 * M_PI * n / p.uSize;
			for ( size_t k = 0; k < p.uSize; k++ )
			{	T Θ = ω * k;
				v.u[ n ].uR += p.u[ k ].uR * cos( Θ ) - p.u[ k ].uI * sin( Θ );
				v.u[ n ].uI += p.u[ k ].uI * cos( Θ ) + p.u[ k ].uR * sin( Θ );
			}
			v.u[ n ].uR /= p.uSize;
			v.u[ n ].uI /= p.uSize;
		}
		return v;
	}

#pragma	mark	FFT

	template	<typename T, size_t N>	struct	//	The number of samples is 2^N
	FFTOutplace
	{	//	UC	need to implement inplace version.
		size_t	u[ 1 << N ];
		
		FFTOutplace()
		{	u[ 0 ] = 0;
			u[ 1 ] = 1;
			size_t wSize = 2;
			for ( size_t i = 0; i < N - 1; ++i )
			{	for ( size_t j = 0; j < wSize; j++ ) u[ j ] *= 2;
				for ( size_t j = 0; j < wSize; j++ ) u[ j + wSize ] = u[ j ] + 1;
				wSize *= 2;
			}
		}

		Array<Complex<T>>
		Forward( Array<Complex<T>> const& p )
		{	Array<Complex<T>> v( p.uSize, true );
			for ( size_t i = 0; i < ( 1 << N ); i++ ) v.u[ i ] = p.u[ u[ i ] ];
			size_t wPow2 = 1;
			for ( size_t i = 0; i < N; i++ )
			{	size_t wPow2m = wPow2;
				wPow2 <<= 1;
				Complex<T> w = Exp( Complex<T>( .0, 2 * M_PI / wPow2 ) );
				Complex<T> wS = Complex<T>( 1, 0 );

				for ( size_t k = 0; k < wPow2m; ++k )
				{	for ( size_t j = 0; j < ( 1 << N ); j += wPow2 )
					{	Complex<T>& wA = v.u[ j + k ];
						Complex<T>& wB = v.u[ j + k + wPow2m ];
						Complex<T> wFB = wS * wB;
						wB = wA - wFB;
						wA += wFB;
					}
					wS *= w;
				}
			}
			return v;
		}
		Array<Complex<T>>
		Inverse( Array<Complex<T>> const& p )
		{	Array<Complex<T>> v( p.uSize, true );
			for ( size_t i = 0; i < ( 1 << N ); i++ ) v.u[ i ] = p.u[ u[ i ] ];
			size_t wPow2 = 1;
			for ( size_t i = 0; i < N; i++ )
			{	size_t wPow2m = wPow2;
				wPow2 <<= 1;
				Complex<T> w = Conj( Exp( Complex<T>( .0, 2 * M_PI / wPow2 ) ) );
				Complex<T> wS = Complex<T>( 1, 0 );

				for ( size_t k = 0; k < wPow2m; ++k )
				{	for ( size_t j = 0; j < ( 1 << N ); j += wPow2 )
					{	Complex<T>& wA = v.u[ j + k ];
						Complex<T>& wB = v.u[ j + k + wPow2m ];
						Complex<T> wFB = wS * wB;
						wB = wA - wFB;
						wA += wFB;
					}
					wS *= w;
				}
			}
			for ( size_t i = 0; i < ( 1 << N ); i++ ) v.u[ i ] /= Complex<T>( 1 << N );

			return v;
		}
	};

#pragma	mark	Draft

	template	<typename T, size_t N>	Matrix<T, N, 1>
	SweepOut( Matrix<T, N, N> p, Matrix<T, N, 1> pC )
	{	for ( size_t i = 0; i < N; i++ )
		{	pC.u[ i ][ 0 ] /= p.u[ i ][ i ];
			for ( size_t c = i + 1; c < N; c++ ) p.u[ i ][ c ] /= p.u[ i ][ i ];	//	Set Diagonal coefficient to 1
			p.u[ i ][ i ] = 1;
			for ( size_t r = 0; r < N; r++ )				//	For all row
			{	if ( r != i )								//	but [ i ]
				{	pC.u[ r ][ 0 ] -= p.u[ r ][ i ] * pC.u[ i ][ 0 ];
					for ( size_t c = i + 1; c < N; c++ ) p.u[ r ][ c ] -= p.u[ r ][ i ] * p.u[ i ][ c ];
					p.u[ r ][ i ] = 0;
				}
			} 
		}
		return pC;
	}

#pragma	mark	Test

	inline	void
	Arithmetics_Test()
	{
		static	Rational<int64_t>	sMat[ 8 ][ 8 ] =
		{	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1
		,	 1,	-1,	 1,	-1,	 1,	-1,	 1,	-1
		,	 1,	 1,	-1,	-1,	 1,	 1,	-1,	-1
		,	 1,	-1,	-1,	 1,	 1,	-1,	-1,	 1
		,	 1,	 1,	 1,	 1,	-1,	-1,	-1,	-1
		,	 1,	-1,	 1,	-1,	-1,	 1,	-1,	 1
		,	 1,	 1,	-1,	-1,	-1,	-1,	 1,	 1
		,	 1,	-1,	-1,	 1,	-1,	 1,	 1,	-1
		};
		Matrix<Rational<int64_t>, 8, 8> wMat;
		sMat >> wMat;
		P2<int64_t>	w1( 8 );
		P2<int64_t>	w2( 1, 8 );
		assert( w1 + w1 == P2<int64_t>(  1, 1,  4 ) );
		assert( w1 - w1 == P2<int64_t>(  0, 1,  0 ) );
		assert( w2 + w2 == P2<int64_t>(  1, 1, -2 ) );
		assert( w2 - w2 == P2<int64_t>(  0, 1,  0 ) );
		assert( w1 + w2 == P2<int64_t>( 65, 1, -3 ) );
		assert( w1 - w2 == P2<int64_t>( 63, 1, -3 ) );
		assert( w1 * w2 == P2<int64_t>(  1, 1,  0 ) );
		assert( w1 / w2 == P2<int64_t>(  1, 1,  6 ) );
	}

	
	inline	void
	SweepOutTest()
	{
		Matrix<double, 4, 4>	w;
		w.u[ 0 ][ 0 ] =  1;	w.u[ 0 ][ 1 ] = -2;	w.u[ 0 ][ 2 ] = -2;	w.u[ 0 ][ 3 ] =  2;
		w.u[ 1 ][ 0 ] =  2;	w.u[ 1 ][ 1 ] = -2;	w.u[ 1 ][ 2 ] = -3;	w.u[ 1 ][ 3 ] =  3;
		w.u[ 2 ][ 0 ] = -1;	w.u[ 2 ][ 1 ] =  6;	w.u[ 2 ][ 2 ] =  3;	w.u[ 2 ][ 3 ] = -2;
		w.u[ 3 ][ 0 ] =  1;	w.u[ 3 ][ 1 ] =  4;	w.u[ 3 ][ 2 ] =  0;	w.u[ 3 ][ 3 ] = -1;

		Matrix<double, 4, 1>	wC;
		wC.u[ 0 ][ 0 ] =   5;
		wC.u[ 1 ][ 0 ] =  10;
		wC.u[ 2 ][ 0 ] =   2;
		wC.u[ 3 ][ 0 ] = -10;

		Matrix<double, 4, 1>	wA;
		wA.u[ 0 ][ 0 ] =  9;
		wA.u[ 1 ][ 0 ] = -2;
		wA.u[ 2 ][ 0 ] = 15;
		wA.u[ 3 ][ 0 ] = 11;
		
		assert( wA == SweepOut( w, wC ) );
		//	9
		//	-2
		//	15
		//	11
	}

}

