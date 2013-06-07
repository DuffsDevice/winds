#include "_gadget/gadget.passcodebox.h"
#include "_type/type.system.h"
#include "_type/type.font.glyphs.h"
#include "_type/type.textphrases.h"

void _passcodebox::refresher()
{
	
}

void _passcodebox::removeStr( _int position , _length numChars )
{
	this->realText.erase( position , numChars );
	
	if( this->realText.length() )
		this->strValue = stringIntegrator::fontChangePhrase( this->replaceFont ) + string( this->realText.length() , this->replaceChar );
	else
		this->strValue = "";
}

void _passcodebox::insertStr( _int position , string s )
{
	this->realText.insert( position , s );
	
	if( this->realText.length() )
		this->strValue = stringIntegrator::fontChangePhrase( this->replaceFont ) + string( this->realText.length() , this->replaceChar );
	else
		this->strValue = "";
}

void _passcodebox::setStrValue( string val )
{
	this->realText = val;
	
	if( this->realText.length() )
		_textbox::setStrValue( stringIntegrator::fontChangePhrase( this->replaceFont ) + string( this->realText.length() , (char)this->replaceChar ) );
	else
		_textbox::setStrValue( "" );
}

_passcodebox::_passcodebox( _coord x , _coord y , _length width , string value , const _font* repFont , _char repChar , _style style ) :
	_textbox( x , y , width , string( value.length() , repChar ) , style )
	, realText( value )
	, replaceChar( repChar )
	, replaceFont( repFont )
{
	if( !replaceFont )
		replaceFont = _system::getFont("SystemSymbols8");
	if( !replaceChar )
		replaceChar = glyph::smallCircleFilled;
}

_passcodebox::_passcodebox( _coord x , _coord y , _length width , _length height , string value , const _font* repFont , _char repChar , _style style ) :
	_textbox( x , y , width , height , string( value.length() , repChar ) , style )
	, realText( value )
	, replaceChar( repChar )
	, replaceFont( repFont )
{
	if( !replaceFont )
		replaceFont = _system::getFont("SystemSymbols8");
	if( !replaceChar )
		replaceChar = glyph::smallCircleFilled;
}