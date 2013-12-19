#include "_type/type.color.h"

_color& _color::setRGB( _u8 red , _u8 green , _u8 blue ){
	this->color = RGB( red , green , blue );
	this->minRGB = min( min( red , green ), blue );
	this->maxRGB = max( max( red , green ), blue );
	return *this;
}

_color& _color::setHSL( _u16 hue , _u16 sat , _u16 lum )
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
	this->setRGB( r * 31.0f , g * 31.0f , b * 31.0f );
	return *this;
}

_u16 _color::getH()
{
	_u8 delta  = this->maxRGB - this->minRGB;
	_u8 deltaHalf  = delta >> 1;
	
	_int red = this->getR();
	_int green = this->getG();
	_int blue = this->getB();
	
	if( this->getS() == 0 )
		return 0;		
	else if( red == this->maxRGB )
		return ( ( green - blue ) * 60 + deltaHalf ) / delta;
		
	else if( green == this->maxRGB )
		return 120 + ( ( blue - red ) * 60 + deltaHalf ) / delta;
		
	else if( blue == this->maxRGB )
		return 240 + ( ( red - green ) * 60 + deltaHalf ) / delta;
	
	return 0;
}


_color& _color::adjustHSL( int hue , int sat , int lum ){
	this->setHSL( mid( 0 , this->getH() + hue , 360 ) , mid( 0 , this->getS() + sat , 100 ) , mid( 0 , this->getL() + lum , 100 ) );
	return *this;
}

_color& _color::adjustRGB( _s8 red , _s8 gre , _s8 blu ){
	this->setRGB( mid( 0 , this->getR() + red , 31 ) , mid( 0 , this->getG() + gre , 31 ) , mid( 0 , this->getB() + blu , 31 ) );
	return *this;
}

_map<string,_pixel> string2color = {
	{ "transparent" , COLOR_TRANSPARENT },
	{ "yellow" 	, COLOR_YELLOW },
	{ "green" 	, COLOR_GREEN },
	{ "cyan" 	, COLOR_CYAN },
	{ "blue" 	, COLOR_BLUE },
	{ "magenta" , COLOR_MAGENTA },
	{ "red" 	, COLOR_RED },
	{ "gray" 	, COLOR_GRAY },
	{ "black" 	, COLOR_BLACK },
	{ "white" 	, COLOR_WHITE }
};