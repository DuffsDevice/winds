#include "_type/type.color.h"

_color::_color( const string& name )
{
	static _map<string,_pixel> string2color = {
		{ "white" , _color::white } ,
		{ "silver" , _color::silver } ,
		{ "gray" , _color::gray } ,
		{ "black" , _color::black } ,
		{ "red" , _color::red } ,
		{ "maroon" , _color::maroon } ,
		{ "orange" , _color::orange } ,
		{ "yellow" , _color::yellow } ,
		{ "olive" , _color::olive } ,
		{ "lime" , _color::lime } ,
		{ "green" , _color::green } ,
		{ "aqua" , _color::aqua } ,
		{ "teal" , _color::teal } ,
		{ "blue" , _color::blue } ,
		{ "navy" , _color::navy } ,
		{ "fuchsia" , _color::fuchsia } ,
		{ "purple" , _color::purple } ,
		{ "transparent" , _color::transparent }
	};
	this->color = string2color[name];
}

void _color::setRGBInternal( _u8 red , _u8 green , _u8 blue , bool alpha ){
	this->color = red | ( green << 5 ) | ( blue << 10 ) | ( alpha << 15 );
}

void _color::setHSLInternal( _u16 hue , _u16 sat , _u16 lum , bool alpha )
{
	float v;
	float r,g,b;
	float h = float(hue)/360, sl = float(sat)/100, l = float(lum)/100;
	r = l;   // default to gray
	g = l;
	b = l;
	v = (l <= 0.5) ? (l * (1.0 + sl)) : (l + sl - l * sl);
	if (v > 0)
	{
		float m;
		float sv;
		int sextant;
		float fract, vsf, mid1, mid2;

		m = l + l - v;
		sv = (v - m ) / v;
		h *= 6.0;
		sextant = (int)h;
		fract = h - sextant;
		vsf = v * sv * fract;
		mid1 = m + vsf;
		mid2 = v - vsf;
		switch (sextant)
		{
			case 6:
			case 0:
				  r = v;
				  g = mid1;
				  b = m;
				  break;
			case 1:
				  r = mid2;
				  g = v;
				  b = m;
				  break;
			case 2:
				  r = m;
				  g = v;
				  b = mid1;
				  break;
			case 3:
				  r = m;
				  g = mid2;
				  b = v;
				  break;
			case 4:
				  r = mid1;
				  g = m;
				  b = v;
				  break;
			case 5:
				  r = v;
				  g = m;
				  b = mid2;
				  break;
		}
	}
	this->setRGBInternal( r * 31.0f , g * 31.0f , b * 31.0f , alpha );
}

_u16 _color::getS() const {
	_u8 minRGB = min( min( this->getR() , this->getG() ), this->getB() );
	_u8 maxRGB = max( max( this->getR() , this->getG() ), this->getB() );
	if( maxRGB != 0 )
		return ( ( maxRGB - minRGB ) * 100 + ( maxRGB >> 1 ) ) / maxRGB;
	return 0;
}

_color& _color::setH( _u16 hue ){
	_u8 minRGB = min( min( this->getR() , this->getG() ), this->getB() );
	_u8 maxRGB = max( max( this->getR() , this->getG() ), this->getB() );
	_u16 sat = maxRGB ? ( ( maxRGB - minRGB ) * 100 + ( maxRGB >> 1 ) ) / maxRGB : 0;
	_u16 lum = ( ( maxRGB + minRGB ) * 100 + 31 ) / 62;
	this->setHSL( hue , sat , lum );
	return *this;
}

_u16 _color::getL() const {
	_u8 minRGB = min( min( this->getR() , this->getG() ), this->getB() );
	_u8 maxRGB = max( max( this->getR() , this->getG() ), this->getB() );
	return ( ( maxRGB + minRGB ) * 100 + 31 ) / 62;
}

_u16 _color::getH()
{
	_u8 minRGB = min( min( this->getR() , this->getG() ), this->getB() );
	_u8 maxRGB = max( max( this->getR() , this->getG() ), this->getB() );
	_u8 delta  = maxRGB - minRGB;
	_u8 deltaHalf  = delta >> 1;
	
	_int red = this->getR();
	_int green = this->getG();
	_int blue = this->getB();
	
	if( this->getS() == 0 )
		return 0;		
	else if( red == maxRGB )
		return ( ( green - blue ) * 60 + deltaHalf ) / delta;
		
	else if( green == maxRGB )
		return 120 + ( ( blue - red ) * 60 + deltaHalf ) / delta;
		
	else if( blue == maxRGB )
		return 240 + ( ( red - green ) * 60 + deltaHalf ) / delta;
	
	return 0;
}

_color& _color::adjustHSL( int hue , int sat , int lum , _optValue<bool> alpha ){
	this->setHSL( mid( 0 , this->getH() + hue , 360 ) , mid( 0 , this->getS() + sat , 100 ) , mid( 0 , this->getL() + lum , 100 ) , move(alpha) );
	return *this;
}

_color& _color::adjustRGB( _s8 red , _s8 green , _s8 blu , _optValue<bool> alpha ){
	this->setRGB( mid( 0 , this->getR() + red , 31 ) , mid( 0 , this->getG() + green , 31 ) , mid( 0 , this->getB() + blu , 31 ) , move(alpha) );
	return *this;
}

const _color _color::white = _color::fromRGB( 31 , 31 , 31 );
const _color _color::silver = _color::fromRGB( 24 , 24 , 24 );
const _color _color::gray = _color::fromRGB( 16 , 16 , 16 );
const _color _color::black = _color::fromRGB( 0 , 0 , 0 );
const _color _color::red = _color::fromRGB( 31 , 0 , 0 );
const _color _color::maroon = _color::fromRGB( 16 , 0 , 0 );
const _color _color::orange = _color::fromRGB( 31 , 21 , 0 );
const _color _color::yellow = _color::fromRGB( 31 , 31 , 0 );
const _color _color::olive = _color::fromRGB( 16 , 16 , 0 );
const _color _color::lime = _color::fromRGB( 0 , 31 , 0 );
const _color _color::green = _color::fromRGB( 0 , 16 , 0 );
const _color _color::aqua = _color::fromRGB( 0 , 31 , 31 );
const _color _color::teal = _color::fromRGB( 0 , 16 , 16 );
const _color _color::blue = _color::fromRGB( 0 , 0 , 31 );
const _color _color::navy = _color::fromRGB( 0 , 0 , 16 );
const _color _color::fuchsia = _color::fromRGB( 31 , 0 , 31 );
const _color _color::purple = _color::fromRGB( 16 , 0 , 16 );
const _color _color::transparent = _color::fromRGB( 0 , 0 , 0 , false );