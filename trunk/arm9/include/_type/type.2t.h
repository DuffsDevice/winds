#ifndef _WIN_T_2T_
#define _WIN_T_2T_

/**
 * Pack two integers into one
 * The union will hopefully be treated
 * as one single value (faster)
 */
template<typename SingleType,typename DoubleType>
union _2T
{
	DoubleType		val;
	struct{
		SingleType	first;
		SingleType	second;
	};
	
	//Ctors
	_2T() {}
	_2T( DoubleType v ) : val( v ) {}
	_2T( SingleType first , SingleType second ) : first( first ) , second( second ) {}
	
	operator bool(){ return this->val != 0; }
	_2T operator+( SingleType val ){ return _2T( first + val , second + val ); }
	_2T operator+( _2T val ){ return _2T( first + val.first , second + val.second ); }
	_2T operator-( SingleType val ){ return _2T( first - val , second - val ); }
	_2T operator-( _2T val ){ return _2T( first - val.first , second - val.second ); }
	_2T operator*( SingleType val ){ return _2T( first * val , second * val ); }
	_2T operator*( _2T val ){ return _2T( first * val.first , second * val.second ); }
	_2T operator/( SingleType val ){ return _2T( first / val , second / val ); }
	_2T operator/( _2T val ){ return _2T( first / val.first , second / val.second ); }
	_2T& operator+=( SingleType val ){ first += val; second += val; return *this; }
	_2T& operator+=( _2T val ){ first += val.first; second += val.second; return *this; }
	_2T& operator-=( SingleType val ){ first -= val; second -= val; return *this; }
	_2T& operator-=( _2T val ){ first -= val.first; second -= val.second; return *this; }
	_2T& operator*=( SingleType val ){ first *= val; second *= val; return *this; }
	_2T& operator*=( _2T val ){ first *= val.first; second *= val.second; return *this; }
	_2T& operator/=( SingleType val ){ first /= val; second /= val; return *this; }
	_2T& operator/=( _2T val ){ first /= val.first; second /= val.second; return *this; }
	bool operator==( _2T val ){ return val.val == this->val; }
	bool operator!=( _2T val ){ return val.val != this->val; }
};

template<typename SingleType,typename DoubleType>
static unused _2T<SingleType,DoubleType> operator-( const _2T<SingleType,DoubleType>& val ){ return _2T<SingleType,DoubleType>( -val.first , -val.second ); }
template<typename SingleType,typename DoubleType>
static unused bool operator!( const _2T<SingleType,DoubleType>& val ){ return !val.val; }

#endif