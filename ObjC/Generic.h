#pragma once
//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

#include	<cassert>

#include	<dispatch/dispatch.h>

#ifdef	__MACH__
#include	<mach/mach_time.h>
#endif

namespace
JpMooParaiso
{
#ifdef	__MACH__
	struct
	Timer
	{	uint64_t u;
		mach_timebase_info_data_t uTB;
		Timer()
		{	mach_timebase_info( &uTB );
			u = mach_absolute_time();
		}
		uint64_t
		Nano() const
		{	return ( mach_absolute_time() - u ) * uTB.numer / uTB.denom;
		}
		uint64_t
		Micro() const
		{	return ( mach_absolute_time() - u ) * uTB.numer / ( uTB.denom * 1000 );
		}
		uint64_t
		Milli() const
		{	return ( mach_absolute_time() - u ) * uTB.numer / ( uTB.denom * 1000000 );
		}
		uint64_t
		Second() const
		{	return ( mach_absolute_time() - u ) * uTB.numer / ( uTB.denom * 1000000000 );
		}
	};
#endif
	template	<typename T>	int
	Compare( T const& pL, T const& pR )
	{	return pL == pR ? 0 : pL < pR ? -1 : 1;
	}
	template	<typename T>	int
	Sign( T const& p )
	{	return p == 0 ? 0 : p < 0 ? -1 : 1;
	}

	struct
	BinarySearchResult
	{	int		uMatchMode;
		size_t	u;
	};
	//	Data	1 3 5 7 9
	//			Search	mMatchMode	Data[u](u)
	//			 0	->	<			1(0)
	//			 1	->	=			1(0)
	//			 2	->	>	or		1(0)
	//				->	<			3(1)
	//			 3	->	=			3(1)
	//			 :
	//			 7	->	=			7(3)
	//			 8	->	>	or		7(3)
	//				->	<			9(4)
	//			 9	->	=			9(4)
	//			10	->	>			9(4)
	template	<typename T, typename F>	BinarySearchResult
	BinarySearch( T* p, size_t pSize, F pCB )	//	pCB must return TriState
	{	assert( pSize );	//	Can't search empty array.
		BinarySearchResult	v;
		size_t	wL = 0;
		size_t	wH = pSize;
		do
		{	v.u = ( wL + wH ) / 2;
			v.uMatchMode = pCB( p[ v.u ] );
//printf( "L:%zu H:%zu u:%zu Mode:%d\n", wL, wH, v.u, v.uMatchMode );
			switch ( v.uMatchMode )
			{	
			case  0:
				return v;
			case -1:
				if ( wH == v.u ) return v;
				wH = v.u;
				break;
			case  1:
				if ( wL == v.u ) return v;
				wL = v.u;
				break;
			}
		}
		while ( true );
		return v;
	}

	template	<typename T>	void
	Copy( const T* pSrc, T* pDst, size_t pSize )
	{	memcpy( pDst, pSrc, pSize * sizeof( T ) );
	}

	template	<typename T>	void
	Zero( T* p, size_t pSize )
	{	bzero( p, pSize * sizeof( T ) );
	}

	template	<typename T>	void
	Move( T* pSrc, T* pDst, size_t p )
	{	memmove( pDst, pSrc, p * sizeof( T ) );
	}

	template	<typename T>	void
	Fill( T* pDst, T const& p, size_t pSize )
	{	while ( pSize-- ) pDst[ pSize ] = p;
	}

	template	<typename F>	void
	Loop( size_t p, F pCB )
	{	while ( p-- ) pCB();
	}

	template	<typename F>	void
	LoopAsync( size_t p, F pCB )
	{	dispatch_queue_t wQ = ::dispatch_get_global_queue( DISPATCH_QUEUE_PRIORITY_DEFAULT, 0 );
		while ( p-- ) ::dispatch_async( wQ, ^{ pCB(); } );
	}

	template	<typename F>	void
	LoopAsyncWait( size_t p, F pCB )
	{	dispatch_queue_t wQ = ::dispatch_get_global_queue( DISPATCH_QUEUE_PRIORITY_DEFAULT, 0 );
		dispatch_apply( p, wQ, ^(size_t) { pCB(); } );
	}

	template	<typename F>	void
	For( size_t p, F pCB )
	{	while ( p-- ) pCB( p );
	}

	template	<typename F>	void
	ForAsync( size_t p, F pCB )
	{	dispatch_queue_t wQ = ::dispatch_get_global_queue( DISPATCH_QUEUE_PRIORITY_DEFAULT, 0 );
		while ( p-- ) ::dispatch_async( wQ, ^{ pCB( p ); } );
	}

	template	<typename F>	void
	ForAsyncWait( size_t p, F pCB )
	{	dispatch_queue_t wQ = ::dispatch_get_global_queue( DISPATCH_QUEUE_PRIORITY_DEFAULT, 0 );
		dispatch_apply( p, wQ, ^(size_t p) { pCB( p ); } );
	}

	template	<typename T>	struct
	iStream
	{	virtual	bool		Avail()	= 0;
		virtual	operator	T()		const = 0;
	};

	template	<typename T, typename F>	void
	For( iStream<T>& pStream, F pCB )
	{	while ( pStream.Avail() ) pCB( (T)pStream );
	}

	template	<typename T, typename F>	void
	ForAsync( iStream<T>& pStream, F pCB )
	{	dispatch_queue_t wQ = ::dispatch_get_global_queue( DISPATCH_QUEUE_PRIORITY_DEFAULT, 0 );
		while ( pStream.Avail() )
		{	T w = pStream;
			dispatch_async( wQ, ^{ pCB( w ); } );
		}
	}

	template	<typename T, typename F>	void
	ForAsyncWait( iStream<T>& pStream, F pCB )
	{	dispatch_group_t wG = ::dispatch_group_create();
		dispatch_queue_t wQ = ::dispatch_get_global_queue( DISPATCH_QUEUE_PRIORITY_DEFAULT, 0 );
		while ( pStream.Avail() )
		{	T w = pStream;
			dispatch_group_async( wG, wQ, ^{ pCB( w ); } );
		}
		dispatch_group_wait( wG, DISPATCH_TIME_FOREVER );
#if	!OS_OBJECT_USE_OBJC
		dispatch_release( wG );
#endif
	}

	template	<typename T>	struct
	Array
	{
		size_t	uCapacity;
		T*		u;
		size_t	uSize;

		virtual
	~	Array()
		{	delete[] u;	//	delete[] is null-safe
		}
		Array( size_t pCapacity = 0, bool pInfrate = false )
		:	uCapacity( pCapacity )
		,	u( new T[ pCapacity ] )
		,	uSize( pInfrate ? pCapacity : 0 )
		{
		}
		Array( T* p, size_t pCapacity )
		:	uCapacity( pCapacity )
		,	u( new T[ pCapacity ] )
		,	uSize( pCapacity )
		{	Copy( p, u, uSize );
		}
		Array( Array<T> const& p )
		:	uCapacity( p.uCapacity )
		,	u( new T[ uCapacity ] )
		,	uSize( p.uSize )
		{	Copy( p.u, u, uSize );
		}
		Array( Array<T>&& p )
		:	uCapacity( p.uCapacity )
		,	u( p.u )
		,	uSize( p.uSize )
		{	p.u = nullptr;
		}
		Array<T> const& operator
		=( Array<T> const& p )
		{	if ( this != &p )
			{	delete[] u;
				uCapacity = p.uCapacity;
				u = new T[ uCapacity ];
				uSize = p.uSize;
				Copy( p.u, u, uSize );
			}
			return p;
		}
		Array<T> const& operator
		=( Array<T>&& p )
		{	if ( this != &p )
			{	delete[] u;
				uCapacity = p.uCapacity;
				u = p.u;
				uSize = p.uSize;
				p.u = nullptr;
			}
			return *this;
		}

		void
		Inflate( size_t p )
		{	assert( p > uCapacity );
			uCapacity = p;
			T* w = new T[ uCapacity ];
			Copy( u, w, uSize );
			delete[] u;
			u = w;
		}
		
		void
		Size( size_t p )
		{	if ( p > uCapacity ) Inflate( p );
			uSize = p;
		}

		void
		Insert( size_t pIndex, T const& p )
		{	assert( pIndex <= uSize );
			if ( uSize == uCapacity ) Inflate( uCapacity ? uCapacity * 2 : 1 );
			Move( u + pIndex, u + pIndex + 1, uSize - pIndex );
			u[ pIndex ] = p;
			uSize++;
		}

		void
		Append( T const& p )
		{	if ( uSize == uCapacity ) Inflate( uCapacity ? uCapacity * 2 : 1 );
			u[ uSize++ ] = p;
		}

		void
		RemoveAtIndex( size_t pIndex )
		{	assert( pIndex < uSize );
			Move( u + pIndex + 1, u + pIndex, --uSize - pIndex);
		}

		void
		Remove( T const& p )
		{	size_t	i = uSize;
			while ( i-- ) if ( u[ i ] == p ) RemoveAtIndex( i );
		}

		template	<typename F>	void
		RunInOrder( F pCB ) const
		{	for ( size_t i = 0; i < uSize; i++ ) pCB( u[ i ] );
		}

		template	<typename F>	Array<size_t>
		Indices( F pCB ) const
		{	Array<size_t>	v;
			for ( size_t i = 0; i < uSize; i++ ) if ( pCB( u[ i ] ) ) v.Append( i );;
			return v;
		}

		bool
		Contains( T const& p )	const
		{	for ( size_t i = 0; i < uSize; i++ ) if ( u[ i ] == p ) return true;
			return false;
		}
	};

	template	<typename T>	Array<T>	operator
	+( T const& p, Array<T> const& pArray )
	{	Array<T>	v( 1 + pArray.uSize );
		v.u[ 0 ] = 0;
		for ( int i = 0; i < pArray.uSize; i++ ) v.u[ i + 1 ] = pArray.u[ i ];
		v.uSize = pArray.uSize + 1;
		return v;
	}
	template	<typename T>	Array<T>	operator
	+( Array<T> const& pArray, T const& p )
	{	Array<T>	v( pArray.uSize + 1 );
		for ( int i = 0; i < pArray.uSize; i++ ) v.u[ i ] = pArray.u[ i ];
		v.u[ pArray.uSize ] = p;
		v.uSize = pArray.uSize + 1;
		return v;
	}

	template	<typename T>	Array<T>	operator
	+( Array<T> const& p1, Array<T> const& p2 )
	{	Array<T>	v( p1.uSize + p2.uSize );
		for ( int i = 0; i < p1.uSize; i++ ) v.u[ i ] = p1.u[ i ];
		for ( int i = 0; i < p2.uSize; i++ ) v.u[ i + p1.uSize ] = p2.u[ i ];
		v.uSize = p1.uSize + p2.uSize;
		return v;
	}

	template	<typename T>	struct
	List
	{	struct
		E
		{	T	u;
			E*	uNext;
			E( T const& p, E* pNext )
			:	u( p )
			,	uNext( pNext )
			{
			}
		};
		E*		u;

		void
		Clear( E* p )
		{	if ( p )
			{	Clear( p->uNext );
				delete p;
			}
		}

		void
		Copy( E* p )
		{	if ( p )
			{	Copy( p->uNext );
				u = new E( p->u, u );
			}
		}

		virtual
	~	List()
		{	Clear( u );
		}
		List()
		:	u( nullptr )
		{
		}
		List( List<T> const& p )
		:	u( nullptr )
		{	Copy( p.u );
		}
		List( List<T>&& p )
		:	u( p.m )
		{	p.u = nullptr;
		}
		List<T> const& operator
		=( List<T> const& p )
		{	Clear( u );
			u = nullptr;
			Copy( p.u );
			return p;
		}
		List<T> const& operator
		=( List<T>&& p )
		{	Clear( u );
			u = p.u;
			p.u = nullptr;
			return *this;
		}
		size_t
		Count()	const
		{	size_t v = 0;
			for ( E* w = u; w; w = w->mNext ) v++;
			return v;
		}

		template	<typename F>	void
		RunInOrder( F pCB ) const
		{	for ( E* w = u; w; w = w->uNext ) pCB( w->u );
		}

		template	<typename F>	void
		Insert( T const& p, F pCB )
		{	E** w = &u;
			while ( *w && pCB( (*w)->u ) ) w = &(*w)->uNext;
			*w = new E( p, *w );
		}

		void
		Prepend( T const& p )
		{	u = new E( p, u );
		}

		void
		Append( T const& p )
		{	E**	w = &u;
			while ( *w ) w = &(*w)->uNext;
			*w = new E( p, *w );
		}

		bool
		Contains( T const& p )	const
		{	for ( E* w = u; w; w = w->uNext ) if ( w->u == p ) return true;
			return false;
		}
	};
	template	<typename F>	struct
	Managed
	{	void							(^uDisposer)( F p );
		Managed const*					uParent;
		JpMooParaiso::Array<Managed*>*	uChildren;
		Managed( void (^pDisposer)( F p ) )
		:	uDisposer( pDisposer )
		,	uParent( nullptr )
		,	uChildren( new JpMooParaiso::Array<Managed*>() )
		{
		}
		static	void
		Adopt( Managed<F> const* pParent, Managed<F>* pChild )
		{	pParent->uChildren->Append( pChild );
			pChild->uParent = pParent;
		}
		virtual
	~	Managed()
		{	if ( uParent )
			{	uParent->uChildren->Remove( this );
				uChildren->RunInOrder( ^( Managed* p ) { Adopt( uParent, p ); } );
			}
			else
			{	if ( uChildren->uSize )
				{	Managed* w1st = uChildren->u[ 0 ];
					w1st->uParent = nullptr;
					for ( size_t i = 1; i < uChildren->uSize; i++ ) Adopt( w1st, uChildren->u[ i ] );
				}
				else
				{	uDisposer( (F)this );
				}
			}
			delete uChildren;
		}
		Managed( Managed<F> const& p )
		:	uDisposer( p.uDisposer )
		,	uParent( &p )
		,	uChildren( new JpMooParaiso::Array<Managed*>() )
		{	p.uChildren->Append( this );
		}
		Managed<F>	const&	operator
		=( Managed<F> const& p )
		{	assert( false );
		}
	};
}

