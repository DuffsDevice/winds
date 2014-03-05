#include "_gadget/gadget.passcodebox.h"
#include "_type/type.system.h"
#include "_type/type.font.glyphs.h"
#include "_type/type.text.phrases.h"

void _passcodeBox::refresher()
{
	
}

void _passcodeBox::removeStr( _int position , _length numChars )
{
	this->realText.erase( position , numChars );
	
	if( this->realText.length() )
		this->strValue = stringIntegrator::fontChangePhrase( this->replaceFont ) + string( this->realText.length() , this->replaceChar );
	else
		this->strValue = "";
}

void _passcodeBox::insertStr( _int position , string s )
{
	this->realText.insert( position , s );
	
	if( this->realText.length() )
		this->strValue = stringIntegrator::fontChangePhrase( this->replaceFont ) + string( this->realText.length() , this->replaceChar );
	else
		this->strValue = "";
}

void _passcodeBox::setStrValue( string val )
{
	this->realText = val;
	
	if( this->realText.length() )
		_textBox::setStrValue( stringIntegrator::fontChangePhrase( this->replaceFont ) + string( this->realText.length() , (char)this->replaceChar ) );
	else
		_textBox::setStrValue( "" );
}

_passcodeBox::_passcodeBox( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string value , const _font* repFont , _char repChar , _style&& style ) :
	_textBox( x , y , width , height , string( value.length() , repChar ) , (_style&&)style )
	, realText( value )
	, replaceChar( repChar )
	, replaceFont( repFont )
{
	if( !replaceFont )
		replaceFont = _system::getFont("SystemSymbols8");
	if( !replaceChar )
		replaceChar = _glyph::smallCircleFilled;
}