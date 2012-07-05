#include "_type/type.color.h"

_color Color( void ){
	_color col;
	return col;
}

_color::_color(){
	this->color = 0;
	this->hue = 0;
	this->sat = 0;
	this->lum = 0;
	this->red = 0;
	this->gre = 0;
	this->blu = 0;
}

_color::_color( _color &color ){
	this->color = color.getColor();
	this->red = color.getR();
	this->gre = color.getG();
	this->blu = color.getB();
	this->minRGB = min( min( this->red , this->gre ), this->blu );
	this->maxRGB = max( max( this->red , this->gre ), this->blu );
	this->hue = color.getH();
	this->sat = color.getS();
	this->lum = color.getL();
}

_pixel _color::getColor(){
	return this->color;
}

_color* _color::setColor( _pixel color ){
	this->color  = color;
	this->red = this->getR();
	this->gre = this->getG();
	this->blu = this->getB();
	this->minRGB = min( min( this->red , this->gre ), this->blu );
	this->maxRGB = max( max( this->red , this->gre ), this->blu );
	this->sat = this->getS();
	this->hue = this->getH();
	this->lum = this->getL();
	return this;
}

_color* _color::setRGB( _u8 red , _u8 green , _u8 blue ){
	this->color = RGB( red , green , blue );
	return this;
}

_color* _color::setHSL( _u16 hue , _u8 sat , _u8 lum ){
	/*_u8 delta  = this->maxRGB - this->minRGB;
	double h = double( hue ) / 60;
	double s = double( sat ) / 100 * 31;
	double l = this->maxRGB = double( lum ) / 100 * 31;
	if( s == 0 )
		this->color = RGB( s , s , s );
	else{
		delta = double( s * maxRGB ) / 31;
		if( this->hue > 3 ){
			this->setB( maxRGB );
			if( this->hue > 4 ){
				this->setG( maxRGB - delta );
				this->setR( ( h - 4 ) * delta + this->gre );
			}else{
				this->setR( maxRGB - delta );
				this->setG( this->red - ( h - 4 ) * delta );
			}
		}else if ( this->hue > 1 ){
			this->setG( maxRGB );
			if( this->hue > 2 ){
				this->setR( maxRGB - delta );
				this->setB( ( h - 2 ) * delta + this->red );
			}else{
				this->setB( maxRGB - delta );
				this->setR( this->blu - ( h - 2 ) * delta );
			}
		}else if( this->hue > -1 ){
			this->setR( maxRGB );
			if( this->hue > 0 ){
				this->setB( maxRGB - delta );
				this->setG( h * delta + this->blu );
			}else{
				this->setG( maxRGB - delta );
				this->setB( this->gre - h * delta );
			}
		}
	}*/
	double v;
	double r,g,b;
	double h = double(hue)/360, sl = double(sat)/100, l = double(lum)/100;
	r = l;   // default to gray
	g = l;
	b = l;
	v = (l <= 0.5) ? (l * (1.0 + sl)) : (l + sl - l * sl);
	if (v > 0)
	{
		double m;
		double sv;
		int sextant;
		double fract, vsf, mid1, mid2;

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
	return this;
}

_u8 _color::getR(){
	return RGB_GETR( this->color );
}

_u8 _color::getG(){
	return RGB_GETG( this->color );
}

_u8 _color::getB(){
	return RGB_GETB( this->color );
}

_u8 _color::getS(){
	double delta  = this->maxRGB - this->minRGB;
	if( this->maxRGB != 0 )
		return this->sat = 31 * delta / this->maxRGB / 31 * 100 ;
	return this->sat = 0;
}

_u8 _color::getL(){
	return this->lum = double( this->maxRGB + this->minRGB ) / 62 * 100 ;
}

_u16 _color::getH(){
	double delta  = this->maxRGB - this->minRGB;
	if( this->sat == 0 )
		return this->hue = 0;
		
	else if( this->red == this->maxRGB)
		return this->hue = ( 0  + double( this->gre - this->blu ) / delta ) * 60;
		
	else if( this->gre == this->maxRGB)
		return this->hue = ( 2 + double( this->blu - this->red ) / delta ) * 60;
		
	else if( this->blu == this->maxRGB)
		return this->hue = ( 4 + double( this->red - this->gre ) / delta ) * 60;
	return NO_COLOR;
}

_color* _color::setH( _u8 hue ){
	this->setHSL( hue , this->sat , this->lum );
	this->hue = hue;
	return this;
}

_color* _color::setS( _u8 sat ){
	this->setHSL( this->hue , sat , this->lum );
	this->sat = sat;
	return this;
}

_color* _color::setL( _u8 lum ){
	this->setHSL( this->hue , this->sat , lum );
	this->lum = lum;
	return this;
}

_color* _color::setR( _u8 red ){
	this->setRGB( red , this->gre , this->blu );
	this->red = red;
	return this;
}

_color* _color::setG( _u8 green ){
	this->setRGB( this->red , green , this->blu );
	this->gre = green;
	return this;
}

_color* _color::setB( _u8 blue ){
	this->setRGB( this->red , this->gre , blue );
	this->blu = blue;
	return this;
}

_color* _color::adjustHSL( int hue , int sat , int lum ){
	this->setHSL( mid( 0 , this->hue + hue , 360 ) , mid( 0 , this->sat + sat , 100 ) , mid( 0 , this->lum + lum , 100 ) );
	this->hue = hue;
	this->sat = sat;
	this->lum = lum;
	return this;
}

_color* _color::adjustRGB( _s8 red , _s8 gre , _s8 blu ){
	this->setRGB( mid( 0 , this->red + red , 31 ) , mid( 0 , this->gre + gre , 31 ) , mid( 0 , this->blu + blu , 31 ) );
	this->red = red;
	this->gre = gre;
	this->blu = blu;
	return this;
}