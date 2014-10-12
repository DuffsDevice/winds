#ifndef _WIN_T_KEY_
#define _WIN_T_KEY_

#include <_type/type.h>

//! Seccesary things from libnds
#include <nds/input.h>
extern "C" unsigned long int keysHeld();
extern "C" unsigned long int keysDown();

/*	
	Usage of character codes lower than 32 (space):
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	1-7, 14-20: Hardware Keys
	8,10, 21-23: Windows Specific Keys (Caps,Shift,Windows-Key,...)
	22-30: Initiation of font, font color or font size change (declared in type.guistring.h)
*/

enum class _key : _char
{
	none = 0,
	a = 1,
	b = 2,
	select = 3,
	start = 4,
	right = 5,
	left = 6,
	up = 7,
	backspace = 8,
	carriage_return = 10,
	down = 14,
	r = 15,
	l = 16,
	x = 17,
	y = 18,
	touch = 19,
	lid	= 20,
	windows = 21,
	caps = 22,
	shift = 23,
};

class _hardwareKeyPattern
{
	private:
		
		_u16 sum;
		
		//! Maps a libnds Key to a WinDS-ASCII-Key
		static const _u16 libndsBit2key_internal[12];
		
		//! Maps a WinDS-ASCII-Key to the bit that represents this key in libnds
		static const _u16 key2libndsBit_first[7];
		static const _u16 key2libndsBit_second[7];
		
	public:
		
		static _u16 key2libndsBit( _key key ){
			_char k = _char(key);
			if( k >= 1 && k <= 20 ){
				if( k <= 7 )
					return 1 << key2libndsBit_first[k-1];
				else if( k >= 14 )
					return 1 << key2libndsBit_second[k-14];
			}
			return 0;
		}
		
		static _key libndsBit2key( _u16 bit ){
			return bit < 12 ? (_key)libndsBit2key_internal[bit] : _key::none;
		}
		
		//! Check whether the supplied key stands for a hardware key
		static bool isHardwareKey( _key key ){
			return key2libndsBit( key ) != 0;
		}
		
		//! Check if the supplied key is marked as 'pressed' in the bit field
		bool isKeySet( _key key ) const {
			return sum & key2libndsBit( key );
		}
		
		//! Check if the supplied key-pattern is marked as 'pressed' in the bit field
		bool isPatternSet( _u16 pattern ) const {
			return sum & pattern;
		}
		
		//! Assign operator to apply a new hashsum
		_hardwareKeyPattern& operator=( _u16 newSum ){
			this->sum = newSum;
			return *this;
		}
		
		//! Add keys to the hash sum
		void add( const _hardwareKeyPattern& keys ){
			this->sum |= keys.sum;
		}
		
		//! Remove keys from the hash sum
		void remove( const _hardwareKeyPattern& keys ){
			this->sum &= ~keys.sum;
		}
		
		//! Defaut Assign Operators
		_hardwareKeyPattern& operator=( const _hardwareKeyPattern& ) = default;
		_hardwareKeyPattern& operator=( _hardwareKeyPattern&& ) = default;
		
		//! Ctor from Bit-field
		_hardwareKeyPattern( _u16 sum = 0 ) :
			sum( sum )
		{}
		//! Ctor from key
		_hardwareKeyPattern( _key key ) :
			sum( key2libndsBit( key ) )
		{}
		
		//! Copy and Move Ctors
		_hardwareKeyPattern( const _hardwareKeyPattern& ) = default;
		_hardwareKeyPattern( _hardwareKeyPattern&& ) = default;
		
		//! Get currently pressed hardware-keys
		static _u16	getCurrentKeys(){
			return keysHeld() & (~(KEY_TOUCH|KEY_LID));
		}
		
		//! Get newly pressed hardware-keys
		static _u16	getNewKeys(){
			return keysHeld() & (~(KEY_TOUCH|KEY_LID));
		}
		
		//! Get all currently pressed hardware-keys inclusive the touch pen and the flag, whether the lid is closed or not
		static _u16	getCurrentInputs(){
			return keysHeld();
		}
		
		//! Get all newly pressed hardware-keys inclusive the touch pen and the flag, whether the lid is closed or not
		static _u16	getNewInputs(){
			return keysDown();
		}
};

#endif