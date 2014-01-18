#include "_type/type.runtimeAttributes.h"
#include "_type/type.font.glyphs.h"
#include "_type/type.key.h"
#include "_type/type.system.h"

// Bitmaps
#include "_resource/resource.image.window.header.h"
#include "_resource/resource.image.window.buttons.h"
#include "_resource/resource.image.checkboxes.h"

_runtimeAttributes::_runtimeAttributes( flex_ptr<_user> user ) :
	defaultFont( "System7" )
	, defaultFontSize( 8 )
	, user( move(user) )
	, windowBar( BMP_WindowHeader() )
	, windowBarBlurred( BMP_WindowHeaderBlurred() )
	, highlightForeground( COLOR_WHITE )
	, highlightBackground( RGB255( 10 , 36 , 106 ) /* Dark blue */ )
	, itemForeground( COLOR_BLACK )
	, itemBackground( COLOR_WHITE )
	, controlBackground( RGB( 28 , 28 , 27 ) )
	, controlForeground( RGB( 22 , 22 , 20 ) )
	, magnifyKeyboardFocus( true )
{
	this->assocDirectories = {
		{ "%WINDIR%" , "/windows" }
		, { "%SYSTEM%" , "/windows/system" }
		, { "%APPDATA%" , "/windows/appdata" }
		, { "%USERS%" , "/windows/users" }
	};
	
	this->keyboardChar[0] = {
		'1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , '0' ,
		'q' , 'w' , 'e' , 'r' , 't' , 'z' , 'u' , 'i' , 'o' , 'p' ,
		'a' , 's' , 'd' , 'f' , 'g' , 'h' , 'j' , 'k' , 'l' , (_char)_key::backspace ,
		(_char)_key::caps , 'y' , 'x' , 'c' , 'v' , 'b' , 'n' , 'm' , '.' , (_char)_key::carriage_return ,
		(_char)_key::shift , (_char)_key::windows , ' ' , ',' , '-' , (_char)_key::shift
	};
	this->keyboardChar[1] = {
		'!' , '"' , '#' , '?' , '%' , '&' , '/' , '(' , ')' , '=' ,
		'Q' , 'W' , 'E' , 'R' , 'T' , 'Z' , 'U' , 'I' , 'O' , 'P' ,
		'A' , 'S' , 'D' , 'F' , 'G' , 'H' , 'J' , 'K' , 'L' , (_char)_key::backspace ,
		(_char)_key::caps , 'Y' , 'X' , 'C' , 'V' , 'B' , 'N' , 'M' , '+' , (_char)_key::carriage_return ,
		(_char)_key::shift , (_char)_key::windows , ' ' , ';' , '_' , (_char)_key::shift
	};
	this->keyboardText[0] = {
		"1" , "2" , "3" , "4" , "5" , "6" , "7" , "8" , "9" , "0" ,
		"q" , "w" , "e" , "r" , "t" , "z" , "u" , "i" , "o" , "p" ,
		"a" , "s" , "d" , "f" , "g" , "h" , "j" , "k" , "l" , string(1,_glyph::backspace) ,
		string(1,_glyph::caps) , "y" , "x" , "c" , "v" , "b" , "n" , "m" , "." , string(1,_glyph::carriageReturn) ,
		string(1,_glyph::shift) , string(1,_glyph::windows) , "Space" , "," , "-" , string(1,_glyph::shift)
	};
	this->keyboardText[1] = {
		"!" , "\"" , "#" , "?" , "%" , "&" , "/" , "(" , ")" , "=" ,
		"Q" , "W" , "E" , "R" , "T" , "Z" , "U" , "I" , "O" , "P" ,
		"A" , "S" , "D" , "F" , "G" , "H" , "J" , "K" , "L" , string( 1 , _glyph::backspace) ,
		string(1,_glyph::capsPressed) , "Y" , "X" , "C" , "V" , "B" , "N" , "M" , "+" , string(1,_glyph::carriageReturn) ,
		string(1,_glyph::shiftPressed) , string(1,_glyph::windows) , "Space" , ";" , "_" , string(1,_glyph::shiftPressed)
	};
}

_runtimeAttributes::~_runtimeAttributes()
{ }