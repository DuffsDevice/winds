#include "_type/type.runtimeAttributes.h"
#include "_graphic/FONT_Tahoma7.h"
#include "_graphic/BMP_WindowHeader.h"
#include "_graphic/BMP_Checkboxes.h"
#include "_graphic/BMP_WindowButtons.h"
#include "_graphic/BMP_WindowsWallpaper.h"
#include "_file/direntry.png.h"
#include "_type/type.bitmapAnimation.h"

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
	, fileObjectHeight( 10 ) //10
	, showFileExtension( true )
	, userLogo( nullptr )
	, userImage( nullptr )
	, userName( "Jakob" )
{
	this->assocDirectories = { { "%WINDIR%" , "windows" } , { "%APPDATA%" , "windows/appdata" } , { "%USERS%" , "windows/users" } };
	
	this->userLogo = new _bitmap( 14 , 14 );
	this->userLogo->reset( COLOR_WHITE );
	
	_png* uImage = new _png( "windows/users/" + this->userName + "/userimage.png" );
	
	if( uImage && uImage->getBitmap() )
	{
		_bitmapTransform* bmp = new _bitmapTransform( 13 , 13 , uImage );
		bmp->compute();
		
		// Create a Logo
		this->userImage = bmp;
		
		// ... and the Raw Image
		this->userLogo->copy( 0 , 0 , this->userImage );
	}
	
	delete uImage;
	
	this->userLogo->drawRect( 0 , 0 , 14 , 14 , RGB( 31 , 31 , 22 ) );
	this->userLogo->drawPixel( 0 , 0 , RGB( 15 , 15 , 24 ) );
	this->userLogo->drawPixel( 13 , 0 , RGB( 15 , 15 , 24 ) );
	this->userLogo->drawPixel( 13 , 13 , RGB( 15 , 15 , 24 ) );
	this->userLogo->drawPixel( 0 , 13 , RGB( 15 , 15 , 24 ) );
	
	this->keyboardChar[0] = { 
		'1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , '0' ,
		'q' , 'w' , 'e' , 'r' , 't' , 'z' , 'u' , 'i' , 'o' , 'p' , 
		'a' , 's' , 'd' , 'f' , 'g' , 'h' , 'j' , 'k' , 'l' , DSWindows::KEY_BACKSPACE , 
		DSWindows::KEY_CAPS , 'y' , 'x' , 'c' , 'v' , 'b' , 'n' , 'm' , '.' , DSWindows::KEY_CARRIAGE_RETURN , 
		DSWindows::KEY_SHIFT , DSWindows::KEY_WINDOWS , ' ' , ',' , '-' , DSWindows::KEY_SHIFT
	};
	this->keyboardChar[1] = { 
		'!' , '"' , '#' , '$' , '%' , '&' , '/' , '(' , ')' , '=' ,
		'Q' , 'W' , 'E' , 'R' , 'T' , 'Z' , 'U' , 'I' , 'O' , 'P' , 
		'A' , 'S' , 'D' , 'F' , 'G' , 'H' , 'J' , 'K' , 'L' , DSWindows::KEY_BACKSPACE , 
		DSWindows::KEY_CAPS , 'Y' , 'X' , 'C' , 'V' , 'B' , 'N' , 'M' , '+' , DSWindows::KEY_CARRIAGE_RETURN , 
		DSWindows::KEY_SHIFT , DSWindows::KEY_WINDOWS , ' ' , ';' , '_' , DSWindows::KEY_SHIFT
	};
	this->keyboardText[0] = { 
		"1" , "2" , "3" , "4" , "5" , "6" , "7" , "8" , "9" , "0" ,
		"q" , "w" , "e" , "r" , "t" , "z" , "u" , "i" , "o" , "p" , 
		"a" , "s" , "d" , "f" , "g" , "h" , "j" , "k" , "l" , "<-" , 
		"Caps" , "y" , "x" , "c" , "v" , "b" , "n" , "m" , "." , "CR" , 
		"Shift" , "#" , "Space" , "," , "-" , "Shift"
	};
	this->keyboardText[1] = { 
		"!" , "\"" , "#" , "$" , "%" , "&" , "/" , "(" , ")" , "=" ,
		"Q" , "W" , "E" , "R" , "T" , "Z" , "U" , "I" , "O" , "P" , 
		"A" , "S" , "D" , "F" , "G" , "H" , "J" , "K" , "L" , "<-" , 
		"Caps" , "Y" , "X" , "C" , "V" , "B" , "N" , "M" , "." , "CR" , 
		"Shift" , "#" , "Space" , ";" , "+" , "Shift"
	};
}

_runtimeAttributes::~_runtimeAttributes()
{
	delete this->userLogo;
}