#include <_type/type.color.h>
#include <math.h>

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
	//! Written by David Flanagan.
	//! Copyright (c) 1992-2001 by David Flanagan
	//! @see http://arc.liv.ac.uk/repos/darcs/sge/source/3rdparty/qmon/Xmt310/Xmt/HSLtoRGB.c
	int red = 0, green = 0, blue = 0;
    int v;
    int m, sv, fract, vsf, mid1, mid2, sextant;
	
	while( hue >= 360 )
		hue -= 360;
	
    v = (lum < 50) ? (lum * (sat + 100) / 100) : (lum + sat - (lum * sat / 100));
    if (v <= 0){
		this->setRGBInternal( 0 , 0 , 0 , alpha );
		return;
	}

    m = lum + lum - v;
    sv = 100 * (v - m) / v;

    sextant = hue/60;
    fract = 100 * (hue - (sextant * 60)) / 60;
    vsf = v * sv * fract / 10000;
    mid1 = m + vsf;
    mid2 = v - vsf;

    switch(sextant)
	{
		case 0: red = v; green = mid1; blue = m; break;
		case 1: red = mid2; green = v; blue = m; break;
		case 2: red = m; green = v; blue = mid1; break;
		case 3: red = m; green = mid2; blue = v; break;
		case 4: red = mid1; green = m; blue = v; break;
		case 5: red = v; green = m; blue = mid2; break;
    }
	
	this->setRGBInternal( red * 31 / 100 , green * 31 / 100 , blue * 31 / 100 , alpha );
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
	_int red = this->getR();
	_int green = this->getG();
	_int blue = this->getB();
	_u8 minRGB = min( min( red , green ) , blue );
	_u8 maxRGB = max( max( red , green ) , blue );
	_u8 delta  = maxRGB - minRGB;
	
	if( !delta ) // Saturation = 0
		return 0;
	
	_u8 deltaHalf  = delta >> 1;	
	
	if( red == maxRGB )
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

_color::_colorLab _color::toLab() const 
{
	//rgb to xyz
	float red = this->getR() / 31.0f;
	float green = this->getG() / 31.0f;
	float blue = this->getB() / 31.0f;
	if( red > 0.04045 )
		red = powf( (red + 0.055 ) / 1.055 , 2.4 );
	else
		red /= 12.92;

	if( green > 0.04045 )
		green = powf( ( green + 0.055 ) / 1.055 , 2.4 );
	else
		green /= 12.92;

	if( blue > 0.04045 )
		blue = powf( ( blue + 0.055 ) / 1.055 , 2.4 );
	else 
		blue /= 12.92;

	red *= 100.f;
	green *= 100.f;
	blue *= 100.f;
	float x = red * 0.4124f + green * 0.3576f + blue * 0.1805f;
	float y = red * 0.2126f + green * 0.7152f + blue * 0.0722f;
	float z = red * 0.0193f + green * 0.1192f + blue * 0.9505f;

	//xyz to lab
	x /= 95.047f;
	y /= 100.000f;
	z /= 108.883f;
	if( x > 0.008856f ) 
		x = powf( x , 0.3333f );
	else
		x = ( 7.787f * x ) + 0.1379f;

	if( y > 0.008856 )
		y = powf( y , 0.3333f );
	else
		y = ( 7.787f * y ) + 0.1379f;

	if( z > 0.008856 )
		z = powf( z , 0.3333f );
	else
		z = ( 7.787 * z ) + 0.1379f;
	
	return _color::_colorLab{
		.l = ( 116.0f * y ) - 16.0f
		, .a = 500.0f * ( x - y )
		, .b = 200.0f * ( y - z )
	};
}

float fastSqrt( float number ){
	#pragma GCC diagnostic ignored "-Wstrict-aliasing"
    long i;
    float x, y;
    const float f = 1.5F;
    x = number * 0.5F;
    y  = number;
    i  = * ( long * ) &y;
    i  = 0x5f3759df - ( i >> 1 );
    y  = * ( float * ) &i;
    y  = y * ( f - ( x * y * y ) );
    y  = y * ( f - ( x * y * y ) );
    return number * y;
	#pragma GCC diagnostic pop
}

#define DELTA_E_K_L             2.f
#define DELTA_E_K_1             0.048f
#define DELTA_E_K_2             0.014f

_u16 _color::distanceInternalRGB( _color col1 , _color col2 )
{
	_u8 deltaR = col1.getR() - col2.getR();
	_u8 deltaG = col1.getG() - col2.getG();
	_u8 deltaB = col1.getB() - col2.getB();
	return deltaR * deltaR + deltaG * deltaG + deltaB * deltaB;
}

_u16 _color::distanceInternalCIE94( _color::_colorLab col1 , _color::_colorLab col2 )
{
	float deltaL = col1.l - col2.l;
	float c1 = fastSqrt( col1.a * col1.a + col1.b * col1.b );
	float c2 = fastSqrt( col2.a * col2.a + col2.b * col2.b );
	float deltaC = c1 - c2;
	float deltaA = col1.a - col2.a;
	float deltaB = col1.b - col2.b;
	float deltaHSquared = deltaA * deltaA + deltaB * deltaB - deltaC * deltaC;
	float deltaH;
	if( deltaHSquared > 0 )
		deltaH = fastSqrt( deltaHSquared );
	else
		deltaH = 0;

	float q1 = deltaL / DELTA_E_K_L;
	float q2 = deltaC / ( 1 + DELTA_E_K_1 * c1 );
	float q3 = deltaH / ( 1 + DELTA_E_K_2 * c2 );
	return ( fastSqrt( q1 * q1 + q2 * q2 + q3 * q3 ) + 0.5f );
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