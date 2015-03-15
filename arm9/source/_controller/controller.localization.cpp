#include <_controller/controller.localization.h>
#include <_controller/controller.registry.h>
#include <_type/type.key.h>
#include <_type/type.binfile.h>

bool _localizationController::init()
{
	// Read Language from registry
	currentLanguage = _registryController::getSystemRegistry().readIndex( "localization" , "language" ).c_str();
	
	// Read Localization Data
	localizationTextTable = new _ini( (string) _binFile( "%SYSTEM%/localizationText.ini" ) );
	localizationMonthTable = new _ini( (string) _binFile( "%SYSTEM%/localizationMonth.ini" ) );
	
	invalidStringSignature = "[]";
	
	keyboardKey[0] = {
		'1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , '0' ,
		'q' , 'w' , 'e' , 'r' , 't' , 'z' , 'u' , 'i' , 'o' , 'p' ,
		'a' , 's' , 'd' , 'f' , 'g' , 'h' , 'j' , 'k' , 'l' , (_wchar)_key::backspace ,
		(_wchar)_key::caps , 'y' , 'x' , 'c' , 'v' , 'b' , 'n' , 'm' , '.' , (_wchar)_key::carriage_return ,
		(_wchar)_key::shift , (_wchar)_key::windows , ' ' , ',' , '-' , (_wchar)_key::shift
	};
	keyboardKey[1] = {
		'!' , '"' , '#' , '?' , '%' , '&' , '/' , '(' , ')' , '=' ,
		'Q' , 'W' , 'E' , 'R' , 'T' , 'Z' , 'U' , 'I' , 'O' , 'P' ,
		'A' , 'S' , 'D' , 'F' , 'G' , 'H' , 'J' , 'K' , 'L' , (_wchar)_key::backspace ,
		(_wchar)_key::caps , 'Y' , 'X' , 'C' , 'V' , 'B' , 'N' , 'M' , '+' , (_char)_key::carriage_return ,
		(_wchar)_key::shift , (_wchar)_key::windows , ' ' , ';' , '_' , (_wchar)_key::shift
	};
	keyboardText[0] = {
		"1" , "2" , "3" , "4" , "5" , "6" , "7" , "8" , "9" , "0" ,
		"q" , "w" , "e" , "r" , "t" , "z" , "u" , "i" , "o" , "p" ,
		"a" , "s" , "d" , "f" , "g" , "h" , "j" , "k" , "l" , "\u21FD" ,
		"\u21E9" , "y" , "x" , "c" , "v" , "b" , "n" , "m" , "." , "\u21B5" ,
		"\u21E7" , "\uF8FF" , getBuiltInString("lbl_space") , "," , "-" , "\u21E7"
	};
	keyboardText[1] = {
		"!" , "\"" , "#" , "?" , "%" , "&" , "/" , "(" , ")" , "=" ,
		"Q" , "W" , "E" , "R" , "T" , "Z" , "U" , "I" , "O" , "P" ,
		"A" , "S" , "D" , "F" , "G" , "H" , "J" , "K" , "L" , "\u21FD" ,
		"\u2B07" , "Y" , "X" , "C" , "V" , "B" , "N" , "M" , "+" , "\u21B2" ,
		"\u2B06" , "\uF8FF" , getBuiltInString("lbl_space") , ";" , "_" , "\u2B06"
	};
	
	return true;
}

const wstring& _localizationController::getMonth( _u8 month ){
	bool exists = true;
	const wstring& value = localizationMonthTable->readIndex( int2string( month ) , (_literal)currentLanguage , &exists );
	if( !exists )
		return invalidStringSignature;
	return value;
}

const wstring& _localizationController::getBuiltInString( const string& name ){
	bool exists;
	const wstring& value = localizationTextTable->readIndex( name , (_literal)currentLanguage , &exists );
	if( !exists )
		return invalidStringSignature;
	return value;
}


void _localizationController::end()
{
	// Write language
	_registryController::getSystemRegistry().writeIndex( "localization" , "language" , (_literal)currentLanguage );
	
	// Delete Localization Data
	localizationMonthTable = nullptr;
	localizationTextTable = nullptr;
}

_language			_localizationController::currentLanguage;
_uniquePtr<_ini>	_localizationController::localizationTextTable;
_uniquePtr<_ini>	_localizationController::localizationMonthTable;
wstring				_localizationController::invalidStringSignature;
_vector<_wchar>		_localizationController::keyboardKey[2];
_vector<wstring>	_localizationController::keyboardText[2];
TinyHyphenator		_localizationController::hyphenatorInstance = TinyHyphenator( TinyHyphenatorLanguages::German() );