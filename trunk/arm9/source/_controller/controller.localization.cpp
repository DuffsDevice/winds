#include <_controller/controller.localization.h>
#include <_controller/controller.registry.h>
#include <_type/type.key.h>
#include <_type/type.binfile.h>
#include <_type/type.font.glyphs.h>

bool _localizationController::init()
{
	// Read Language from registry
	currentLanguage = _registryController::getSystemRegistry().readIndex( "localization" , "language" );
	
	// Read Localization Data
	localizationTextTable = new _ini( (string) _binFile( "%SYSTEM%/localizationText.ini" ) );
	localizationMonthTable = new _ini( (string) _binFile( "%SYSTEM%/localizationMonth.ini" ) );
	
	invalidStringSignature = "[]";
	
	keyboardKey[0] = {
		'1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , '0' ,
		'q' , 'w' , 'e' , 'r' , 't' , 'z' , 'u' , 'i' , 'o' , 'p' ,
		'a' , 's' , 'd' , 'f' , 'g' , 'h' , 'j' , 'k' , 'l' , (_char)_key::backspace ,
		(_char)_key::caps , 'y' , 'x' , 'c' , 'v' , 'b' , 'n' , 'm' , '.' , (_char)_key::carriage_return ,
		(_char)_key::shift , (_char)_key::windows , ' ' , ',' , '-' , (_char)_key::shift
	};
	keyboardKey[1] = {
		'!' , '"' , '#' , '?' , '%' , '&' , '/' , '(' , ')' , '=' ,
		'Q' , 'W' , 'E' , 'R' , 'T' , 'Z' , 'U' , 'I' , 'O' , 'P' ,
		'A' , 'S' , 'D' , 'F' , 'G' , 'H' , 'J' , 'K' , 'L' , (_char)_key::backspace ,
		(_char)_key::caps , 'Y' , 'X' , 'C' , 'V' , 'B' , 'N' , 'M' , '+' , (_char)_key::carriage_return ,
		(_char)_key::shift , (_char)_key::windows , ' ' , ';' , '_' , (_char)_key::shift
	};
	keyboardText[0] = {
		"1" , "2" , "3" , "4" , "5" , "6" , "7" , "8" , "9" , "0" ,
		"q" , "w" , "e" , "r" , "t" , "z" , "u" , "i" , "o" , "p" ,
		"a" , "s" , "d" , "f" , "g" , "h" , "j" , "k" , "l" , string(1,_glyph::backspace) ,
		string(1,_glyph::caps) , "y" , "x" , "c" , "v" , "b" , "n" , "m" , "." , string(1,_glyph::carriageReturn) ,
		string(1,_glyph::shift) , string(1,_glyph::windows) , getBuiltInString("lbl_space") , "," , "-" , string(1,_glyph::shift)
	};
	keyboardText[1] = {
		"!" , "\"" , "#" , "?" , "%" , "&" , "/" , "(" , ")" , "=" ,
		"Q" , "W" , "E" , "R" , "T" , "Z" , "U" , "I" , "O" , "P" ,
		"A" , "S" , "D" , "F" , "G" , "H" , "J" , "K" , "L" , string( 1 , _glyph::backspace) ,
		string(1,_glyph::capsPressed) , "Y" , "X" , "C" , "V" , "B" , "N" , "M" , "+" , string(1,_glyph::carriageReturn) ,
		string(1,_glyph::shiftPressed) , string(1,_glyph::windows) , getBuiltInString("lbl_space") , ";" , "_" , string(1,_glyph::shiftPressed)
	};
	
	return true;
}

const string& _localizationController::getMonth( _u8 month ){
	const string& value = localizationMonthTable->readIndex( int2string( month ) , currentLanguage );
	if( value.empty() )
		return invalidStringSignature;
	return value;
}

const string& _localizationController::getBuiltInString( const string& name ){
	const string& value = localizationTextTable->readIndex( name , currentLanguage );
	if( value.empty() )
		return invalidStringSignature;
	return value;
}


void _localizationController::end()
{
	// Write language
	_registryController::getSystemRegistry().writeIndex( "localization" , "language" , currentLanguage );
	
	// Delete Localization Data
	localizationMonthTable = nullptr;
	localizationTextTable = nullptr;
}

_language			_localizationController::currentLanguage;
_uniquePtr<_ini>	_localizationController::localizationTextTable;
_uniquePtr<_ini>	_localizationController::localizationMonthTable;
string				_localizationController::invalidStringSignature;
_vector<_char>		_localizationController::keyboardKey[2];
_vector<string>		_localizationController::keyboardText[2];