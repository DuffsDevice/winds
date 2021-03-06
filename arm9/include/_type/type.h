// Check if already included
#ifndef _WIN_T_TYPES_
#define _WIN_T_TYPES_

#include <_type/type.predefines.h>

#include <string.h>
#include <list>
#define _GLIBCXX_DEBUG // Because vector::operator[] leads to undefined behaviour when undefined key
#include <vector>
#include <deque>
#undef _GLIBCXX_DEBUG
#include <array>
#include <string>
#include <map>
#include <unordered_map>
#include <tuple>
#include <functional>
#include <stdarg.h>

//! Code section
#define ITCM_CODE	__attribute__((section(".itcm"), long_call))

//! Additional CV-Like Qualifiers
#define deprecated	__attribute__(( deprecated ))
#define unused		__attribute__(( unused ))

using std::make_pair;
using std::make_tuple;
using std::move;
using std::forward;
using std::function;
using std::ignore;
using std::get;

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

//! Two values packed in one variable
#include <_type/type.2t.h>

using _2s32		= _2T<_s32,_u64>;
using _2u32		= _2T<_u32,_u32>;
using _2s16		= _2T<_s16,_u32>;
using _2u16		= _2T<_u16,_u32>;
using _2s8		= _2T<_s8,_u16>;
using _2u8		= _2T<_u8,_u16>;
using _pos		= _2T<_coord,_u32>;
using _size		= _2T<_length,_u32>;
using _2char	= _2T<_char,_u16>;
using _2wchar	= _2T<_wchar,_u32>;

//! Type of strings
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

//! Type of wide utf8-capable strings
#include <_library/library.utf8.h>

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

//! Convert _dimension to string and back
extern _toStr<_dimension>	dimension2string;
extern _fromStr<_dimension>	string2dimension;

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
extern int			countDecimals( _s32 value , _u8 numbersystem = 10 );

//! Reads an int from a string
extern int			string2int( _literal str ) __attribute__(( nonnull(1) )) ;
static unused int	string2int( string str ){ return string2int( str.c_str() ); }

//! Converts an int to string
extern string		int2string( _int val , _u8 zeroFill = 0 , _u8 numbersystem = 10 );
extern string		fmt2string( _literal format , ... ) __attribute__(( format( printf , 1 , 2 ) )) ;
extern string		vfmt2string( _literal format , va_list );

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


//! Convert _align and _valign to string
extern _toStr<_align>	align2string;
extern _toStr<_valign>	valign2string;

//! Convert a given string to either _align or _valign
extern _fromStr<_align>		string2align;
extern _fromStr<_valign>	string2valign;

//! Trim not-printable characters at both ends of a given string
void trim( string& , _literal delims = " \n\r\t" , bool front = true , bool back = true );
void trim( wstring& , _wliteral delims = L" \n\r\t" , bool front = true , bool back = true );

//! Tools for analyzing the efficiency of code
#include <_type/type.analyzer.h>

#endif