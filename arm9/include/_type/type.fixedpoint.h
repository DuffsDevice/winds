#ifndef _WIN_T_FIXEDPOINT_
#define _WIN_T_FIXEDPOINT_

// Fixed-point Math implementation
namespace _detail_{inline _int CountLeadingZeros(_int x);}

template<int p> // Abszissa
class _fixed{
	
	private:
	
		static inline _int fixmulf( _int a , _int b )
		{
			return ( a * b ) >> p;
		}
		
		static inline _int fixmul(_int a, _int b)
		{
			return (_int)(((_s64)a * b) >> p);
		}
		
		static inline int fixdiv(_int a, _int b)
		{
		#if 0
			return (_int)((((_s64)a) << p) / b);
		#else	
			// The following produces the same results as the above but gcc 4.0.3 
			// generates fewer instructions (at least on the ARM processor).
			union {
				_s64 a;
				struct {
					_int l;
					_int h;
				};
			} x;
			
			x.l = a << p;
			x.h = a >> (sizeof(_int) * 8 - p);
			return (_int)(x.a / b);
		#endif
		}
		
		_int intValue;
	
	public:
	
		_fixed()
		{}
		
		_fixed( _int i ) 
			: intValue( i << p )
		{}
		
		explicit _fixed( float f ) : 
			intValue( _int( f * float( 1 << p ) ) )
		{}
		
		float toFloat(){
			return *this;
		}
		
		int toInt(){
			return intValue >> p;
		}
		
		operator float(){
			return float( intValue ) / float( 1 << p );
		}
		
		_fixed& operator+= ( _fixed r ) { intValue += r.intValue; return *this; }
		_fixed& operator-= ( _fixed r ) { intValue -= r.intValue; return *this; }
		_fixed& operator*= ( _fixed r ) { intValue = fixmul( intValue , r.intValue ); return *this; }
		_fixed& operator/= ( _fixed r ) { intValue = fixdiv( intValue , r.intValue ); return *this; }
		
		_fixed& operator+= ( _int r ) { intValue += r << p; return *this; }
		_fixed& operator-= ( _int r ) { intValue -= r << p; return *this; }
		_fixed& operator*= ( _int r ) { intValue *= r; return *this; }
		_fixed& operator/= ( _int r ) { intValue /= r; return *this; }
		
		_fixed operator- () const { _fixed x; x.intValue = -intValue; return x; }
		_fixed operator+ ( _fixed r ) const { _fixed x = *this; x += r; return x; }
		_fixed operator- ( _fixed r ) const { _fixed x = *this; x -= r; return x; }
		_fixed operator* ( _fixed r ) const { _fixed x = *this; x *= r; return x; }
		_fixed operator/ ( _fixed r ) const { _fixed x = *this; x /= r; return x; }
		
		bool operator== ( _fixed r ) const { return intValue == r.intValue; }
		bool operator!= ( _fixed r ) const { return !(*this == r); }
		bool operator<  ( _fixed r ) const { return intValue < r.intValue; }
		bool operator>  ( _fixed r ) const { return intValue > r.intValue; }
		bool operator<= ( _fixed r ) const { return intValue <= r.intValue; }
		bool operator>= ( _fixed r ) const { return intValue >= r.intValue; }
		
		bool operator== ( _int r ) const { return intValue == r << p; }
		bool operator!= ( _int r ) const { return intValue != r << p; }
		bool operator<  ( _int r ) const { return intValue < r << p; }
		bool operator>  ( _int r ) const { return intValue > r << p; }
		bool operator<= ( _int r ) const { return intValue <= r << p; }
		bool operator>= ( _int r ) const { return intValue >= r << p; }

		_fixed operator+ ( _int r ) const { _fixed x = *this; x += r; return x; }
		_fixed operator- ( _int r ) const { _fixed x = *this; x -= r; return x; }
		_fixed operator* ( _int r ) const { _fixed x = *this; x *= r; return x; }
		_fixed operator/ ( _int r ) const { _fixed x = *this; x /= r; return x; }
		
		// Post-increment
		_fixed& operator++ ( _int r ){ this->intValue += r << p; return *this; }
		_fixed& operator-- ( _int r ){ this->intValue -= r << p; return *this; }
		
		// Preincrement
		_fixed& operator++ (){ this->intValue += 1 << p; return *this; }
		_fixed& operator-- (){ this->intValue -= 1 << p; return *this; }
};

namespace _detail_
{
	// Internals

	inline _int CountLeadingZeros(_int x)
	{
		_int exp = 31;
		
		if (x & 0xffff0000) { 
			exp -= 16; 
			x >>= 16; 
		}
		
		if (x & 0xff00) { 
			exp -= 8; 
			x >>= 8; 
		}
		
		if (x & 0xf0) { 
			exp -= 4; 
			x >>= 4; 
		}
		
		if (x & 0xc) { 
			exp -= 2; 
			x >>= 2; 
		}
		
		if (x & 0x2) { 
			exp -= 1; 
		}
		
		return exp;
	}
}

typedef _fixed<8> _float;
#endif