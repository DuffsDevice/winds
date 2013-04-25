#include "_type/type.runtimeAttributes.h"
#include "_resource/BMP_WindowHeader.h"
#include "_resource/BMP_Checkboxes.h"
#include "_resource/BMP_WindowButtons.h"
#include "_resource/BMP_WindowsWallpaper.h"

_runtimeAttributes::_runtimeAttributes() :
	defaultFont( "System7" )
	, defaultFontSize( 8 )
	, windowBar( BMP_WindowHeader() )
	, windowBarBlurred( BMP_WindowHeaderBlurred() )
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
		'a' , 's' , 'd' , 'f' , 'g' , 'h' , 'j' , 'k' , 'l' , DSWindows::KEY_BACKSPACE ,
		DSWindows::KEY_CAPS , 'y' , 'x' , 'c' , 'v' , 'b' , 'n' , 'm' , '.' , DSWindows::KEY_CARRIAGE_RETURN ,
		DSWindows::KEY_SHIFT , DSWindows::KEY_WINDOWS , ' ' , ',' , '-' , DSWindows::KEY_SHIFT
	};
	this->keyboardChar[1] = {
		'!' , '"' , '#' , '?' , '%' , '&' , '/' , '(' , ')' , '=' ,
		'Q' , 'W' , 'E' , 'R' , 'T' , 'Z' , 'U' , 'I' , 'O' , 'P' ,
		'A' , 'S' , 'D' , 'F' , 'G' , 'H' , 'J' , 'K' , 'L' , DSWindows::KEY_BACKSPACE ,
		DSWindows::KEY_CAPS , 'Y' , 'X' , 'C' , 'V' , 'B' , 'N' , 'M' , '+' , DSWindows::KEY_CARRIAGE_RETURN ,
		DSWindows::KEY_SHIFT , DSWindows::KEY_WINDOWS , ' ' , ';' , '_' , DSWindows::KEY_SHIFT
	};
	this->keyboardText[0] = {
		"1" , "2" , "3" , "4" , "5" , "6" , "7" , "8" , "9" , "0" ,
		"q" , "w" , "e" , "r" , "t" , "z" , "u" , "i" , "o" , "p" ,
		"a" , "s" , "d" , "f" , "g" , "h" , "j" , "k" , "l" , string(1,glyph::backspace) ,
		string(1,glyph::caps) , "y" , "x" , "c" , "v" , "b" , "n" , "m" , "." , string(1,glyph::carriageReturn) ,
		string(1,glyph::shift) , string(1,glyph::windows) , "Space" , "," , "-" , string(1,glyph::shift)
	};
	this->keyboardText[1] = {
		"!" , "\"" , "#" , "?" , "%" , "&" , "/" , "(" , ")" , "=" ,
		"Q" , "W" , "E" , "R" , "T" , "Z" , "U" , "I" , "O" , "P" ,
		"A" , "S" , "D" , "F" , "G" , "H" , "J" , "K" , "L" , string( 1 , glyph::backspace) ,
		string(1,glyph::capsPressed) , "Y" , "X" , "C" , "V" , "B" , "N" , "M" , "+" , string(1,glyph::carriageReturn) ,
		string(1,glyph::shiftPressed) , string(1,glyph::windows) , "Space" , ";" , "_" , string(1,glyph::shiftPressed)
	};
}

_runtimeAttributes::~_runtimeAttributes()
{ }