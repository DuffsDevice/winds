#include <_gadget/gadget.passcodebox.h>
#include <_type/type.font.glyphs.h>
#include <_type/type.guistring.h>
#include <_controller/controller.font.h>

void _passcodeBox::refresher()
{
	
}

void _passcodeBox::removeStr( _int position , _length numChars ){
	this->realText.erase( position , numChars );
	this->text = string( this->realText.length() , this->replaceChar );
}

void _passcodeBox::insertStr( _int position , string s ){
	this->realText.insert( position , move(s) );
	this->text = string( this->realText.length() , this->replaceChar );
}

void _passcodeBox::setStrValue( string val ){
	this->realText = move(val);
	_textBox::setStrValue( string( this->realText.length() , (char)this->replaceChar ) );
}

_passcodeBox::_passcodeBox( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string value , _fontHandle replaceFont , _optValue<_char> repChar , _style&& style ) :
	_textBox( x , y , width , height , string( value.length() , repChar ) , (_style&&)style )
	, realText( value )
	, replaceChar( repChar.isValid() ? (_char)repChar : _glyph::smallCircleFilled )
{
	_textBox::setFont( replaceFont ? replaceFont.rawData() : _fontController::getFont("SystemSymbols8") );
}