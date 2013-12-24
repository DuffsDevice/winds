// Check if already included
#ifndef _WIN_T_TYPES_
#define _WIN_T_TYPES_

#include "_type/type.predefines.h"

#include <string.h>
#include <list>
#define _GLIBCXX_DEBUG // Because vector::operator[] leads to undefined behaviour when undefined
#include <vector>
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
typedef _vector<string>				_cmdArgs;

//! Minimum and Maximum of two
using std::min;
using std::max;

//! Calculate the Median value of three
_s32 mid( _s32 a , _s32 b , _s32 c );

//! Templates that specify the conversion between strings and 'T'
template<typename T>
	using _toStr = _map<T,_literal>;
template<typename T>
	using _fromStr = _map<string,T>;

enum class _dimension : _u8{
	horizontal = 0 ,
	vertical = 1
};

enum class _direction : _u8{
	left = 0,
	leftup = 4,
	leftdown = 12,
	up = 5,
	upleft = leftup,
	upright = 6,
	down = 13,
	downright = 14,
	downleft = leftdown,
	right = 10,
	rightup = upright,
	rightdown = downright
};

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

//! Convert red, green, blue to 15bit Triplette
static constexpr inline _pixel RGB( _u8 r , _u8 g , _u8 b ){
	return r | ( g << 5 ) | ( b << 10 ) | ( 1 << 15 ); // a=1 means opaque, a=0 means transparent
}
static constexpr inline _pixel RGB255( _u8 r , _u8 g , _u8 b ){
	return ( r >> 3 ) | ( ( g >> 3 ) << 5 ) | ( ( b >> 3 ) << 10 ) | ( 1 << 15 ); // a=1 means opaque, a=0 means transparent
}
static constexpr inline _pixel RGBHEX( _u32 hex ){
	return ( ( hex & 0xff0000 ) >> 19 ) | ( ( ( hex & 0x00ff00 ) >> 11 ) << 5 ) | ( ( ( hex & 0x0000ff ) >> 3 ) << 10 ) | ( 1 << 15 ); // a=1 means opaque, a=0 means transparent
}

//! Convert " " " to 16bit Triplette including an alpha-channel
static constexpr inline _pixel RGBA( _u8 r , _u8 g , _u8 b , bool a ){
	return r | ( g << 5 ) | ( b << 10 ) | ( a << 15 ); // a=1 means opaque, a=0 means transparent
}
static constexpr inline _pixel RGBA255( _u8 r , _u8 g , _u8 b , bool a ){
	return ( r >> 3 ) | ( ( g >> 3 ) << 5 ) | ( ( b >> 2 ) << 10 ) | ( ( a >> 3 ) << 15 ); // a=1 means opaque, a=0 means transparent
}
static constexpr inline _pixel RGBAHEX( _u32 hex , bool alpha ){
	return ( ( hex & 0xff0000 ) >> 19 ) | ( ( ( hex & 0x00ff00 ) >> 11 ) << 5 ) | ( ( ( hex & 0x0000ff ) >> 3 ) << 10 ) | ( alpha << 15 ); // a=1 means opaque, a=0 means transparent
}

//! Gets the specific component value from a color
static constexpr inline _u8 RGB_GETR( _pixel c ){ return c & 0x1F; }
static constexpr inline _u8 RGB_GETG( _pixel c ){ return ( c >> 5 ) & 0x1F; }
static constexpr inline _u8 RGB_GETB( _pixel c ){ return ( c >> 10 ) & 0x1F; }
static constexpr inline _u8 RGB_GETA( _pixel c ){ return c >> 15; }

//! Sets a specific value
static inline void RGB_SETR( _pixel& c , _u8 red ){ c = ( c & ~0x1F ) | red; }
static inline void RGB_SETG( _pixel& c , _u8 green ){ c = ( c & ~( 0x1F << 5 ) ) | ( green << 5 ); }
static inline void RGB_SETB( _pixel& c , _u8 blue ){ c = ( c & ~( 0x1F << 10 ) ) | ( blue << 10 ); }
static inline void RGB_SETA( _pixel& c , bool alpha ){ c = ( c & ~( 1 << 15 ) ) | ( alpha << 15 ); }

//! Counts, how many digits a number has
extern int countDecimals( _s32 value , _u8 numbersystem = 10 );

//! Reads an int from a string
extern int string2int( const _char* str ) __attribute__(( nonnull (1) ));
static unused int string2int( string str ){ return string2int( str.c_str() ); }

//! Converts an int to string
extern string int2string( _int val , _u8 zeroFill = 0 , _u8 numbersystem = 10 );

//! Predefined colors
#define COLOR_TRANSPARENT	(_pixel(0))
#define COLOR_YELLOW 		(RGB(31,31,0))
#define COLOR_GREEN 		(RGB(0,31,0))
#define COLOR_CYAN 			(RGB(0,31,31))
#define COLOR_BLUE 			(RGB(0,0,31))
#define COLOR_MAGENTA 		(RGB(31,0,31))
#define COLOR_RED 			(RGB(31,0,0))
#define COLOR_GRAY 			(RGB(15,15,15))
#define COLOR_BLACK 		(_pixel(1<<15))
#define COLOR_WHITE 		(_pixel((1<<16)-1))
#define NO_COLOR 			(_pixel(0))

// Useful things
namespace DSWindows
{
	// The Numbers used here are 
	// ASCII-Values that are not being used
	const _char KEY_A = 1,
	KEY_B = 2,
	KEY_SELECT = 3,
	KEY_START = 4,
	KEY_RIGHT = 5,
	KEY_LEFT	= 6,
	KEY_UP = 7,

	KEY_BACKSPACE = 8,
	KEY_CARRIAGE_RETURN = 10,

	KEY_DOWN	= 14,
	KEY_R = 15,
	KEY_L = 16,
	KEY_X = 17,
	KEY_Y = 18,

	KEY_CAPS	= 19,
	KEY_SHIFT = 20,
	KEY_WINDOWS = 21,
	
	// Excape characters used in fonts to prepare special actions
	STR_CHANGEFONT = 22,
	STR_CHANGEFONTCOLOR = 23,
	STR_CHANGEFONTSIZE = 24;
	
	extern bool isHardwareKey( _char );
	
	//! Displayed as replacemanet if language-specific term
	//! is not available in the currently selected language
	static const char* const emptyStringSignature = "[]";
	
	//! Maps a Libnds Key to a DSWindows-Key
	extern _char libnds2key[12];
};

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
		_u8		isGiven;
		template<class> friend class _optValue; // Grants all _optValue instantiations to each other
	public:
		
		template<class T2>
		_optValue( _optValue<T2>&& opt ) : val( (T&&)opt.val ) , isGiven( opt.isGiven ) {}
		template<class T2>
		_optValue( const _optValue<T2>& opt ) : val( (T)opt.val ) , isGiven( opt.isGiven ) {}
		_optValue( T&& val ) : val( move( val ) ) , isGiven( true ) { }
		_optValue( const T& val ) : val( val ) , isGiven( true ) { }
		_optValue( decltype(std::ignore) = std::ignore ) : val() , isGiven( false ) { }
		
		bool isValid() const { return isGiven; }
		_optValue<T>& operator =( T&& val ){ this->val = move(val); isGiven = true; return *this; }
		_optValue<T>& operator =( const T& val ){ this->val = val; isGiven = true; return *this; }
		_optValue<T>& operator =( decltype(std::ignore) ){ this->val = T(); isGiven = false; return *this; }
		
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
			typedef decltype( std::is_convertible<T3,T>::value ) check; // Check if T2 can be casted to T
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
			if( !this->passed )
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

//! Enumerates available _languages for WinDS
enum class _language : _u8
{
	japanese = 0,
	english,
	french,
	german,
	italian,
	spanish,
	chinese
};

//! Trim not-printable characters at both ends of a given string
void trim( string& , const char* delims = " \n\r\t" , bool front = true , bool back = true );

//! Tools for analyzing the efficiency of code
#include "_type/type.analyzer.h"

#endif