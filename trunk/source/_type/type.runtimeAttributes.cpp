#include "_type/type.runtimeAttributes.h"
#include "_resource/FONT_Tahoma7.h"
#include "_resource/BMP_WindowHeader.h"
#include "_resource/BMP_Checkboxes.h"
#include "_resource/BMP_WindowButtons.h"
#include "_resource/BMP_WindowsWallpaper.h"

_runtimeAttributes::_runtimeAttributes() :
	wallpaper( BMP_WindowsWallpaper() ) 
	, wallpaperView( WALLPAPER_ORIG ) 
	//, wallpaperBgColor( RGB( 7 , 13 , 20 ) )
	, defaultFont( "Tahoma7" )
	, defaultFontSize( 8 ) 
	, windowBar( BMP_WindowHeader() )
{
	this->assocDirectories = { { "%WINDIR%" , "/windows" } , { "%APPDATA%" , "/windows/appdata" } , { "%USERS%" , "/windows/users" } };
	
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
		"a" , "s" , "d" , "f" , "g" , "h" , "j" , "k" , "l" , "Ð" , 
		"ª" , "y" , "x" , "c" , "v" , "b" , "n" , "m" , "." , "þ" , 
		"¾" , "¼" , "Space" , "," , "-" , "¾"
	};
	this->keyboardText[1] = { 
		"!" , "\"" , "#" , "?" , "%" , "&" , "/" , "(" , ")" , "=" ,
		"Q" , "W" , "E" , "R" , "T" , "Z" , "U" , "I" , "O" , "P" , 
		"A" , "S" , "D" , "F" , "G" , "H" , "J" , "K" , "L" , "Ð" , 
		"ª" , "Y" , "X" , "C" , "V" , "B" , "N" , "M" , "." , "þ" , 
		"½" , "¼" , "Space" , ";" , "+" , "½"
	};
}

_runtimeAttributes::~_runtimeAttributes()
{ }