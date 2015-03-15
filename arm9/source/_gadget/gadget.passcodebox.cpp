#include <_gadget/gadget.passcodebox.h>
#include <_type/type.guistring.h>
#include <_controller/controller.font.h>

void _passcodeBox::refresher()
{
	
}

void _passcodeBox::removeStr( _int position , _length numChars ){
	this->realText.erase( position , numChars );
	this->text = string( this->realText.length() , this->replaceChar );
}

void _passcodeBox::insertStr( _int position , wstring s ){
	this->realText.insert( position , move(s) );
	this->text = string( this->realText.length() , this->replaceChar );
}

void _passcodeBox::setStrValue( wstring val ){
	this->realText = move(val);
	_textBox::setStrValue( wstring( this->realText.length() , (char)this->replaceChar ) );
}

_passcodeBox::_passcodeBox( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , wstring value , _optValue<_char> repChar , _style&& style ) :
	_textBox( x , y , width , height , string( value.length() , repChar ) , (_style&&)style )
	, realText( value )
	, replaceChar( repChar.isValid() ? (_wchar)repChar : L'•' )
{}