//	Written by Satoru Ogura, Tokyo.
//
import Foundation

typealias	E		= UInt32
typealias	E2		= UInt64

//typealias	E		= UInt8
//typealias	E2		= UInt16

let	NUM_BITS	= sizeof( E ) * 8 - 1
let	BORDER		= E( 1 << NUM_BITS )

enum
BigIntegerError	:	ErrorType {
	case			DivideByZero
}

class
EsUtil {
	var	m		: [ E ]
	init( _ a	: [ E ] ) { m = a }
	
	func
	AddDigit( a: Int, _ radix: Int ) {
		var	wCarry = E2( a )
		for i in 0 ..< m.count {
			let w = E2( m[ i ] ) * E2( radix ) + wCarry
			m[ i ] = E( w % E2( BORDER ) )
			wCarry = w >> E2( NUM_BITS )
		}
		if wCarry > 0 { m.append( E( wCarry ) ) }
	}
	func
	CarryAll() {
		if m.count > 0 {
			for i in 1 ..< m.count {
				if m[ i - 1 ] >= BORDER {
					m[ i - 1 ] %= BORDER
					m[ i ] += 1
				}
			}
			if m[ m.count - 1 ] >= BORDER {
				m[ m.count - 1 ] %= BORDER
				m.append( 1 )
			}
		}
	}
	func
	Normalize() {
		while m.last == 0 { m.removeLast() }
	}
	func
	BorrowAll() {
		for i in 1 ..< m.count {
			if m[ i - 1 ] >= BORDER {
				m[ i - 1 ] %= BORDER
				m[ i ] = m[ i ] > 0 ? m[ i ] - 1 : E( BORDER ) - 1 + E( BORDER )
			}
		}
		Normalize()
	}
	func
	Add( a: E, at: Int = 0 ) {
		var	wCarry = a
		for i in at ..< m.count {
			m[ i ] += wCarry
			wCarry = m[ i ] >> E( NUM_BITS )
			m[ i ] %= BORDER
		}
		if wCarry > 0 { m.append( E( wCarry ) ) }
	}
	func
	Add2N( a: Int ) {
		let	wAt = a / NUM_BITS
		while m.count <= wAt { m.append( 0 ) }
		Add( 1 << E( a - wAt * NUM_BITS ), at: wAt )
	}

	func
	Minus( a: E, at: Int = 0 ) {
		if m[ at ] >= a { m[ at ] -= a } else { m[ at ] = m[ at ] + BORDER - a + BORDER }
		var	i = at + 1
		while i < m.count {
			if m[ i - 1 ] >= BORDER {
				m[ i - 1 ] %= BORDER
				m[ i ] += 1
				i += 1
			} else {
				break
			}
		}
		if m[ m.count - 1 ] >= BORDER {
			m[ m.count - 1 ] %= BORDER
			m.append( 1 )
		}
	}
	func
	Div2N( a: Int ) {
		for _ in 0 ..< a / NUM_BITS { m.removeFirst() }
		let wNumBits = E( a % NUM_BITS )
		if wNumBits > 0 {
			m[ 0 ] >>= wNumBits
			for i in 1 ..< m.count {
				m[ i - 1 ] |= ( m[ i ] << ( E( NUM_BITS ) - wNumBits ) ) % BORDER
				m[ i ] >>= wNumBits
			}
			Normalize()
		}
	}
	func
	Mul2N( a: Int ) {
		let wNumBits = E( a % NUM_BITS )
		if wNumBits > 0 {
			m.append( 0 )
			for i in ( 1 ..< m.count ).reverse() {
				m[ i ] |= ( m[ i - 1 ] >> ( E( NUM_BITS ) - wNumBits ) ) % BORDER
				m[ i - 1 ] = ( m[ i - 1 ] << wNumBits ) % BORDER
			}
			Normalize()
		}
		for _ in 0 ..< Int( a ) / NUM_BITS { m.insert( 0, atIndex:0 ) }
	}
}

func
NumBits( a: [ E ] ) -> Int {
	if a.count == 0 { return 0 }
	var v = a.count * NUM_BITS
	var	w = a[ a.count - 1 ]
	for _ in 0 ..< NUM_BITS {
		w <<= E( 1 )
		if w & BORDER > 0 { break }
		v -= 1
	}
	return v
}

func
FloatValue( a: [ E ] ) -> Float64 {
	var	v = 0 as Float64
	for w in a.reverse() { v = v * Float64( BORDER ) + Float64( w ) }
	return v
}

func
Digits( a: [ E ], _ radix: Int ) -> String {
	if a.count == 0 { return "0" }
	var wEs = a
	var	v = ""
	while wEs.count > 0 {
		var i = wEs.count - 1
		var	wBorrow = E2( wEs[ i ] ) % E2( radix )
		wEs[ i ] /= E( radix )
		if wEs[ i ] == 0 { wEs.removeLast() }
		while i > 0 {
			i -= 1;
			let w = ( wBorrow << E2( NUM_BITS ) ) + E2( wEs[ i ] )
			wEs[ i ] = E( w / E2( radix ) )
			wBorrow = w % E2( radix )
		}
		if wBorrow < 10 {
			v = "\(UnicodeScalar( Int( wBorrow ) + Int( ( "0" as UnicodeScalar ).value ) ))" + v
		} else {
			v = "\(UnicodeScalar( Int( wBorrow ) - 10 + Int( ( "A" as UnicodeScalar ).value ) ))" + v
		}
	}
	return v
}

public	class
BigInteger : CustomStringConvertible {

	var	m		:	[ E ]
	let	minus	:	Bool

	init( _ a: [ E ] = [ E ](), _ minus: Bool = false ) {
		m = a;
		self.minus = minus
	}
	
	public	func
	StringRepresentation( radix: Int ) -> String {
		let	v = Digits( m, radix )
		return minus ? "-" + v : v
	}
	public	var
	description: String {
		return StringRepresentation( 10 )
	}
	func
	Float() -> Float64 {
		let	v = FloatValue( m )
		return minus ? -v : v
	}
	func
	RemainderE() -> Int {
		let	v = m.count == 0 ? 0 : Int( m[ 0 ] )
		return minus ? -v : v
	}
}

func
MakeBigInteger( a: String, radix: Int = 10, minus: Bool = false ) -> BigInteger {
	let	w = EsUtil( [ E ]() );
	for u in a.unicodeScalars {
		switch u {
		case "0" ... "9":
			w.AddDigit( Int( u.value ) - Int( ( "0" as UnicodeScalar ).value ), radix )
		case "A" ... "Z":
			w.AddDigit( Int( u.value ) - Int( ( "A" as UnicodeScalar ).value ) + 10, radix )
		case "a" ... "z":
			w.AddDigit( Int( u.value ) - Int( ( "a" as UnicodeScalar ).value ) + 10, radix )
		default:
			break
		}
	}
	return BigInteger( w.m, minus )
}

func
MakeBigInteger( a: Int ) -> BigInteger {
	var	w		= a
	var	wMinus	= false
	if w < 0 {
		w = -w
		wMinus = true
	}

	let	v = EsUtil( [ E ]() );
	while w > 0 {
		v.m.append( E( w % Int( BORDER ) ) )
		w /= Int( BORDER )
	}
	return BigInteger( v.m, wMinus )
}

func
_Compare( l: [ E ], _ r: [ E ] ) -> Int {
	if l.count == r.count {
		for i in ( 0 ..< l.count ).reverse() {
			if l[ i ] > r[ i ] { return  1 }
			if l[ i ] < r[ i ] { return -1 }
		}
		return 0
	} else {
		return l.count > r.count ? 1 : -1
	}
}

func
Compare( l: BigInteger, _ r: BigInteger ) -> Int {
	if l.m.count == 0 && r.m.count == 0 { return 0 }
	return l.minus
	?	r.minus ? -_Compare( l.m, r.m ) : -1
	:	r.minus ?  1 :  _Compare( l.m, r.m )
}

public	func
IsZero( l: BigInteger ) -> Bool {
	return l.m.count == 0
}

public	func
==( l: BigInteger, r: BigInteger ) -> Bool { return Compare( l, r ) == 0 }

public	func
!=( l: BigInteger, r: BigInteger ) -> Bool { return Compare( l, r ) != 0 }

public	func
>( l: BigInteger, r: BigInteger ) -> Bool { return Compare( l, r ) == 1 }

public	func
>=( l: BigInteger, r: BigInteger ) -> Bool { return Compare( l, r ) != -1 }

public	func
<( l: BigInteger, r: BigInteger ) -> Bool { return Compare( l, r ) == -1 }

public	func
<=( l: BigInteger, r: BigInteger ) -> Bool { return Compare( l, r ) != 1 }

func
_Plus( b: [ E ], _ s: [ E ] ) -> [ E ] {
	let	v = EsUtil( b )
	for ( i, w ) in s.enumerate() { v.m[ i ] += w }
	v.CarryAll()
	return v.m
}

func
Plus( l: [ E ], _ r: [ E ] ) -> [ E ] {
	return l.count >= r.count ? _Plus( l, r ) : _Plus( r, l )
}

func
_Minus( b: [ E ], _ s: [ E ] ) -> [ E ] {
	let	v = EsUtil( b )
	for ( i, w ) in s.enumerate() {
		if v.m[ i ] >= w {
			v.m[ i ] -= w
		} else {
			v.m[ i ] = v.m[ i ] + E( BORDER ) - w + E( BORDER )
		}
	}
	v.BorrowAll()
	return v.m
}
func
Minus( l: [ E ], _ r: [ E ] ) -> ( reverse: Bool, [ E ] ) {
	return l.count >= r.count
	?	( false, _Minus( l, r ) )
	:	(  true, _Minus( r, l ) )
}

func
Mul( l: [ E ], _ r: [ E ] ) -> [ E ] {
	let v = EsUtil( [ E ]( count: l.count + r.count, repeatedValue: 0 ) )
	for il in 0 ..< l.count {
		for ir in 0 ..< r.count {
			let w = E2( l[ il ] ) * E2( r[ ir ] )
			v.Add( E( w % E2( BORDER ) ), at: il + ir )
			v.Add( E( w >> E2( NUM_BITS ) ), at: il + ir + 1 )
		}
	}
	v.Normalize()
	return v.m
}

func
QR( l: [ E ], _ r: [ E ] ) -> ( [ E ], [ E ] ) {

	let vQuotient = EsUtil( [ E ]() )
	let	vRemainder = EsUtil( l )
	let wRNB = NumBits( r )
	
	while true {
		let	wLNB = NumBits( vRemainder.m )
		if wLNB > wRNB {
			let	wR = EsUtil( r )
			let	wNB = wLNB - wRNB
			wR.Mul2N( wNB )
			if _Compare( vRemainder.m, wR.m ) == -1 {
				vQuotient.Add2N( wNB - 1 )
				wR.Div2N( 1 )
			} else {
				vQuotient.Add2N( wNB )
			}
			vRemainder.m = _Minus( vRemainder.m, wR.m )
		} else { break }
	}
	if _Compare( vRemainder.m, r ) != -1 {
		vQuotient.Add( 1 )
		vRemainder.m = _Minus( vRemainder.m, r )
	}
	return ( vQuotient.m, vRemainder.m )
}

public	func
+( l: BigInteger, r: BigInteger ) -> BigInteger {
	if r.m.count == 0 { return l }
	if l.m.count == 0 { return r }
	switch ( l.minus, r.minus ) {
	case ( false, false ):	//	+	+
		return BigInteger( Plus( l.m, r.m ), false )
	case ( false, true ):	//	+	-
		let ( wRev, wEs ) = Minus( l.m, r.m )
		return BigInteger( wEs, wRev )
	case ( true, false ):	//	-	+
		let ( wRev, wEs ) = Minus( l.m, r.m )
		return BigInteger( wEs, !wRev )
	case ( true, true ):	//	-	-
		return BigInteger( Plus( l.m, r.m ), true )
	}
}

public	func
-( l: BigInteger, r: BigInteger ) -> BigInteger {
	if r.m.count == 0 { return l }
	if l.m.count == 0 { return BigInteger( r.m, !r.minus ) }
	switch ( l.minus, r.minus ) {
	case ( false, false ):	//	+	+
		let ( wRev, wEs ) = Minus( l.m, r.m )
		return BigInteger( wEs, wRev )
	case ( false, true ):	//	+	-
		return BigInteger( Plus( l.m, r.m ), false )
	case ( true, false ):	//	-	+
		return BigInteger( Plus( l.m, r.m ), true )
	case ( true, true ):	//	-	-
		let ( wRev, wEs ) = Minus( l.m, r.m )
		return BigInteger( wEs, !wRev )
	}
}

public	func
*( l: BigInteger, r: BigInteger ) -> BigInteger {
	if r.m.count == 0 { return r }
	if l.m.count == 0 { return l }
	return BigInteger( Mul( l.m, r.m ), l.minus != r.minus )
}

public	func
/( l: BigInteger, r: BigInteger ) throws -> BigInteger {
	if r.m.count == 0 { throw BigIntegerError.DivideByZero }
	if l.m.count == 0 { return l }
	return BigInteger( QR( l.m, r.m ).0, l.minus != r.minus )
}

public	func
%( l: BigInteger, r: BigInteger ) throws -> BigInteger {
	if r.m.count == 0 { throw BigIntegerError.DivideByZero }
	if l.m.count == 0 { return l }
	return BigInteger( QR( l.m, r.m ).1, l.minus != r.minus )
}

func
And( b: [ E ], _ s: [ E ] ) -> [ E ] {
	let v = EsUtil( [ E ]( count: s.count, repeatedValue: 0 ) )
	var	i = 0
	while i < s.count {
		v.m[ i ] = ( b[ i ] & s[ i ] ) % BORDER
		i += 1
	}
	v.Normalize()
	return v.m
}
public	func
&( l: BigInteger, r: BigInteger ) -> BigInteger {
	let	v = l.m.count >= r.m.count ? And( l.m, r.m ) : And( r.m, l.m )
	return BigInteger( v, l.minus && r.minus )
}

func
Or( b: [ E ], _ s: [ E ] ) -> [ E ] {
	var	v = b
	for i in 0 ..< s.count { v[ i ] = b[ i ] | s[ i ] }
	return v
}
public	func
|( l: BigInteger, r: BigInteger ) -> BigInteger {
	let	v = l.m.count >= r.m.count ? Or( l.m, r.m ) : Or( r.m, l.m )
	return BigInteger( v, l.minus || r.minus )
}

func
XOr( b: [ E ], _ s: [ E ] ) -> [ E ] {
	var	v = b
	for i in 0 ..< s.count { v[ i ] = b[ i ] ^ s[ i ] }
	return v
}
public	func
^( l: BigInteger, r: BigInteger ) -> BigInteger {
	let	v = l.m.count >= r.m.count ? XOr( l.m, r.m ) : XOr( r.m, l.m )
	return BigInteger( v, l.minus != r.minus )
}
