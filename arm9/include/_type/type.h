// Check if already included
#ifndef _WIN_T_TYPES_
#define _WIN_T_TYPES_

#include "_type/type.predefines.h"

#include <string.h>
#include <list>
#define _GLIBCXX_DEBUG // Because vector::operator[] leads to undefined behaviour when undefined
#include <vector>
#include <deque>
#undef _GLIBCXX_DEBUG
#include <array>
#include <string>
#include <map>
#include <unordered_map>
#include <tuple>
#include <functional>

//! Code-sections
#include <nds/memory.h>

//! CV-Like Qualifiers
#define unused __attribute__(( unused ))
#define noinline __attribute__((noinline))
#define forceinline __attribute__((always_inline)) inline
#define DEPRECATED __attribute__((deprecated))

//! Define if you want speed profiling
//#define DEBUG_PROFILING

using std::make_pair;
using std::make_tuple;
using std::move;
using std::forward;
using std::function;
using std::ignore;

template<typename T>
	using _list = std::list<T>;
template<typename T>
	using _vector = std::vector<T>;
template<typename T>
	using _deque = std::deque<T>;
template<typename T,typename T2>
	using _map = std::map<T,T2>;
template<typename T,typename T2>
	using _unorderedMap = std::unordered_map<T,T2>;
template<typename T,typename T2>
	using _pair = std::pair<T,T2>;
template<typename...T>
	using _tuple = std::tuple<T...>;
template<typename T,unsigned int T2>
	using _array = std::array<T,T2>;
template<typename T>
	using _initializerList = std::initializer_list<T>;
	

typedef std::basic_string<_char>	string;

//! Minimum and Maximum of two
using std::min;
using std::max;

//! Calculate the Median value of three
_s32 mid( _s32 a , _s32 b , _s32 c );

//! Templates that specify the conversion between strings and 'T'
namespace std{
	template<class E> class hash{
		using sfinae = typename std::enable_if<std::is_enum<E>::value, E>::type;
		public:
		size_t operator()(const E&e) const {
			return std::hash<
				typename std::underlying_type<E>::type
			>()(
				static_cast<typename std::underlying_type<E>::type>(e)
			);
		}
	};
};
template<typename T>
	using _toStr = _unorderedMap<T,_literal>;
template<typename T>
	using _fromStr = _unorderedMap<string,T>;
template<typename From, typename To>
	using _converter = _unorderedMap<From,To>;

enum class _dimension : _u8{
	horizontal = 0 ,
	vertical = 1
};

enum class _direction : _u8{
	center = 0,
	middle = center,
	left = 4,
	leftup = 5,
	leftdown = 6,
	up = 1,
	upleft = leftup,
	upright = 9,
	down = 2,
	downright = 10,
	downleft = leftdown,
	right = 8,
	rightup = upright,
	rightdown = downright,
	
	// Masks to filter
	horizontalMask = 12,
	verticalMask = 3
};

//! Filter a horizontal/vertical part out of a given direction
static unused constexpr _direction getHorizontalPart( _direction dir ){
	return _direction( _u8(dir) & _u8(_direction::horizontalMask) );
}
static unused constexpr _direction getVerticalPart( _direction dir ){
	return _direction( _u8(dir) & _u8(_direction::verticalMask) );
}

//! Convert _dimension to string and back
extern _toStr<_dimension>	dimension2string;
extern _fromStr<_dimension>	string2dimension;

/**
 * Pack two integers into one
 * The union will hopefully be treated
 * as one single _u64/_s64 (faster)
 */
union						_2s32
{
	_s64	val;
	struct{ _s32 first; _s32 second; };
	
	//Ctors
	_2s32() {}
	_2s32( _s64 v ) : val( v ) {}
	_2s32( _s32 f , _s32 s ) : first( f ) , second( s ) {}
	
	operator bool(){ return this->val != 0; }
	_2s32 operator+( _s32 val ){ return _2s32( first + val , second + val ); }
	_2s32 operator-( _s32 val ){ return _2s32( first - val , second - val ); }
	_2s32 operator*( _s32 val ){ return _2s32( first * val , second * val ); }
	_2s32 operator/( _s32 val ){ return _2s32( first / val , second / val ); }
	_2s32& operator+=( _s32 val ){ first += val; second += val; return *this; }
	_2s32& operator-=( _s32 val ){ first -= val; second -= val; return *this; }
	_2s32& operator*=( _s32 val ){ first *= val; second *= val; return *this; }
	_2s32& operator/=( _s32 val ){ first /= val; second /= val; return *this; }
	bool operator==( _2s32 val ){ return val.val == this->val; }
	bool operator!=( _2s32 val ){ return val.val != this->val; }
};

static unused _2s32 operator-( const _2s32& val ){ return _2s32( -val.first , -val.second ); }
static unused bool operator!( const _2s32& val ){ return !val.val; }

union 						_2u32
{
	_u64	val;
	struct{ _u32 first; _u32 second; };
	
	// Ctors
	_2u32() {}
	_2u32( _u64 v ) : val( v ) {}
	_2u32( _u32 f , _u32 s ) : first( f ) , second( s ) {}
	
	operator bool(){ return this->val != 0; }
	_2u32 operator+( _s32 val ){ return _2u32( first + val , second + val ); }
	_2u32 operator-( _s32 val ){ return _2u32( first - val , second - val ); }
	_2u32 operator*( _u32 val ){ return _2u32( first * val , second * val ); }
	_2u32 operator/( _u32 val ){ return _2u32( first / val , second / val ); }
	_2u32& operator+=( _s32 val ){ first += val; second += val; return *this; }
	_2u32& operator-=( _s32 val ){ first -= val; second -= val; return *this; }
	_2u32& operator*=( _u32 val ){ first *= val; second *= val; return *this; }
	_2u32& operator/=( _u32 val ){ first /= val; second /= val; return *this; }
	bool operator==( _2u32 val ){ return val.val == this->val; }
	bool operator!=( _2u32 val ){ return val.val != this->val; }
};

static const _length SCREEN_WIDTH = 256;
static const _length SCREEN_HEIGHT = 192;

//! Some constraints
template<class T, class B> struct subclass_of {
	static void constraints(T* p) { B* pb = p; }
	subclass_of() { void(*)(T*) = constraints; }
};

template<class T1, class T2> struct copyable {
	static void constraints(T1 a, T2 b) { T2 c = a; b = a; }
	copyable() { void(*)(T1,T2) = constraints; }
};

template<class T1, class T2 = T1> struct comparable {
	static void constraints(T1 a, T2 b) { a==b; a!=b; a<b; a>b; }
	comparable() { void(*)(T1,T2) = constraints; }
};

//! Counts, how many digits a number has
extern int countDecimals( _s32 value , _u8 numbersystem = 10 );

//! Reads an int from a string
extern int string2int( const _char* str ) __attribute__(( nonnull (1) ));
static unused int string2int( string str ){ return string2int( str.c_str() ); }

//! Converts an int to string
extern string int2string( _int val , _u8 zeroFill = 0 , _u8 numbersystem = 10 );

//! Alignment enumeration
enum class _align : _u8 {
	left = 0,
	center = 1,
	right = 2
};

//! Vertical Alignment enumeration
enum class _valign : _u8 {
	top = 4,
	middle = 8,
	bottom = 12
};

class _padding;
class _margin;

//! Class that describes the width of the four borders of a rectangle
struct _border
{
	_length left;
	_length top;
	_length right;
	_length bottom;
	
	_border( _length l , _length t , _length r , _length b ) : left(l) , top(t) , right(r) , bottom(b) {}
	_border( _length width ) : left(width) , top(width) , right(width) , bottom(width) {}
	_border() : left(0) , top(0) , right(0) , bottom(0) {}
	
	//! Comparisons
	inline bool operator==( const _border& other ) const { return !( *this != other ); }
	inline bool operator!=( const _border& other ) const {
		return other.left != this->left || other.top != this->top || other.right != this->right || other.bottom != this->bottom;
	}
	
	//! Casts for them
	operator _padding();
	operator _margin();
};

//! Typedefs for class _padding and _margin
struct _padding : _border{
	_padding( _length l , _length t , _length r , _length b ) : _border( l , t , r , b ) {}
	_padding( _length width ) : _border( width ) {}
	_padding(){}
};
struct _margin : _border{
	_margin( _length l , _length t , _length r , _length b ) : _border( l , t , r , b ) {}
	_margin( _length width ) : _border( width ) {}
	_margin(){}
};

/**
 * This class will, used as parameter type, allow the omission
 * of one parameter using 'std::ignore' or just 'ignore'
 *
 * @note This is not like a default parameter,
 * 		 but a way of indicating the lack of a value at all
 */
template<typename T>
class _optValue
{
	private:
		T		val;
		bool	isGiven;
		template<class> friend class _optValue; // Grants all _optValue instantiations to each other
		
		
	public:
		
		//! Constructors
		template<class T2>
		_optValue( _optValue<T2>&& opt ) : val( (T&&)opt.val ) , isGiven( opt.isGiven ){}
		template<class T2>
		_optValue( const _optValue<T2>& opt ) : val( (T)opt.val ) , isGiven( opt.isGiven ){}
		_optValue( T&& val ) : val( move( val ) ) , isGiven( true ){}
		_optValue( const T& val ) : val( val ) , isGiven( true ){}
		_optValue( decltype(std::ignore) = std::ignore ) : val() , isGiven( false ){}
		_optValue() : val() , isGiven( false ){}
		
		//! Check for validity
		bool isValid() const { return isGiven; }
		
		//! Assign operators
		_optValue<T>& operator=( T&& val ){ this->val = move(val); isGiven = true; return *this; }
		_optValue<T>& operator=( const T& val ){ this->val = val; isGiven = true; return *this; }
		_optValue<T>& operator=( decltype(std::ignore) ){ this->val = T(); isGiven = false; return *this; }
		template<typename T2>
		_optValue<T>& operator=( _optValue<T2>&& opt ){ val = move(opt.val); isGiven = opt.isGiven; return *this; }
		template<typename T2>
		_optValue<T>& operator=( const _optValue<T2>& opt ){ val = opt.val; isGiven = opt.isGiven; return *this; }
		
		//! Cast operators
		operator T&&(){ return (T&&)val; }
		operator const T&() const { return (T)val; }
};

/**
 * Class that will used as parameter type quickly allocate
 * a copy of the passed object or moves it into a new allocation
 * 
 * @note This is especially useful with subclasses that have to get stored as a pointer
 */
template<class T>
class _paramAlloc
{
	private:
		
		T*	ptr;
		_u8	passed;
	
	public:
		
		//! Ctor that will allocate the object
		template<typename T2>
		_paramAlloc( T2&& obj ) :
			passed( false )
		{
			typedef typename std::remove_reference<T2>::type T3;
			unused typedef decltype( std::is_convertible<T3,T>::value ) check; // Check if T2 can be casted to T
			ptr = (T*) new T3( (T2&&)obj );
		}
		
		template<typename T2>
		_paramAlloc( T2* obj ) = delete;
		
		//! Move and copy ctors
		_paramAlloc( const _paramAlloc& ) = delete;
		_paramAlloc( _paramAlloc&& from ) :
			ptr( from.ptr ) , passed( from.passed )
		{
			from.passed = true;
			from.ptr = nullptr;
		}
		
		//! Get the now allocated pointer
		operator T*(){ this->passed = true; return this->ptr; }
		operator bool(){ return this->ptr != nullptr; }
		
		//! Named accessors
		bool isValid(){ return this->ptr != nullptr; }
		T* get(){ this->passed = true; return this->ptr; }
		
		//! Dtor
		~_paramAlloc(){
			if( !this->passed && this->ptr )
				delete this->ptr;
		}
		
		//! There is no assign!
		_paramAlloc& operator=( const _paramAlloc& ) = delete;
		_paramAlloc& operator=( _paramAlloc&& ) = delete;
};

//! Holds useful information about the touch of the stylus
struct _touch
{
	_coord x;
	_coord y;
	
	_touch& operator=( touchPosition t )
	{
		x = t.px;
		y = t.py;
		return *this;
	}
	
	_touch( touchPosition t ){ *this = t; }
	
	_touch() :
		x( 0 )
		, y ( 0 )
	{ }
};

//! Convert _align and _valign to string
extern _toStr<_align>	align2string;
extern _toStr<_valign>	valign2string;

//! Convert a given string to either _align or _valign
extern _fromStr<_align>		string2align;
extern _fromStr<_valign>	string2valign;

//! Trim not-printable characters at both ends of a given string
void trim( string& , const char* delims = " \n\r\t" , bool front = true , bool back = true );

//! Tools for analyzing the efficiency of code
#include "_type/type.analyzer.h"

#endif