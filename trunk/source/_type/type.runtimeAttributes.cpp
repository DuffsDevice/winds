#include "_type/type.runtimeAttributes.h"
#include "_graphic/FONT_Tahoma7.h"
#include "_graphic/BMP_WindowHeader.h"
#include "_graphic/BMP_WindowButtons.h"

_runtimeAttributes::_runtimeAttributes() :
	wallpaper( nullptr ) 
	, wallpaperView( WALLPAPER_ORIG ) 
	, wallpaperBgColor( RGB( 7 , 13 , 20 ) )  
	, maxClickCycles( 30 ) 
	, maxDoubleClickCycles( 60 )
	, maxDoubleClickArea( 6 )
	, minDragDistance( 10 ) 
	, defaultFont( new FONT_Tahoma7() ) 
	, startButtonText( "start" ) 
	, windowBar( new BMP_WindowHeader() ) 
	, fileObjectHeight( 10 )
	, showFileExtension( true )
{
	this->keyboardChar[0] = { 
		'1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , '0' ,
		'q' , 'w' , 'e' , 'r' , 't' , 'z' , 'u' , 'i' , 'o' , 'p' , 
		'a' , 's' , 'd' , 'f' , 'g' , 'h' , 'j' , 'k' , 'l' , DSWindows::KEY_BACKSPACE , 
		DSWindows::KEY_CAPS , 'y' , 'x' , 'c' , 'v' , 'b' , 'n' , 'm' , '.' , DSWindows::KEY_CARRIAGE_RETURN , 
		DSWindows::KEY_SHIFT , DSWindows::KEY_WINDOWS , ' ' , ',' , '-' , DSWindows::KEY_SHIFT
	};
	this->keyboardChar[1] = { 
		'1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , '0' ,
		'Q' , 'W' , 'E' , 'R' , 'T' , 'Z' , 'U' , 'I' , 'O' , 'P' , 
		'A' , 'S' , 'D' , 'F' , 'G' , 'H' , 'J' , 'K' , 'L' , DSWindows::KEY_BACKSPACE , 
		DSWindows::KEY_CAPS , 'Y' , 'X' , 'C' , 'V' , 'B' , 'N' , 'M' , '.' , DSWindows::KEY_CARRIAGE_RETURN , 
		DSWindows::KEY_SHIFT , DSWindows::KEY_WINDOWS , ' ' , ';' , '+' , DSWindows::KEY_SHIFT
	};
	this->keyboardText[0] = { 
		"1" , "2" , "3" , "4" , "5" , "6" , "7" , "8" , "9" , "0" ,
		"q" , "w" , "e" , "r" , "t" , "z" , "u" , "i" , "o" , "p" , 
		"a" , "s" , "d" , "f" , "g" , "h" , "j" , "k" , "l" , "<-" , 
		"Caps" , "y" , "x" , "c" , "v" , "b" , "n" , "m" , "." , "CR" , 
		"Shift" , "#" , "Space" , "," , "-" , "Shift"
	};
	this->keyboardText[1] = { 
		"1" , "2" , "3" , "4" , "5" , "6" , "7" , "8" , "9" , "0" ,
		"Q" , "W" , "E" , "R" , "T" , "Z" , "U" , "I" , "O" , "P" , 
		"A" , "S" , "D" , "F" , "G" , "H" , "J" , "K" , "L" , "<-" , 
		"Caps" , "Y" , "X" , "C" , "V" , "B" , "N" , "M" , "." , "CR" , 
		"Shift" , "#" , "Space" , ";" , "+" , "Shift"
	};
}