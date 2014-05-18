#include "_gadget/gadget.slider.h"
#include "_controller/controller.font.h"

#define OFFSETX 3
#define OFFSETX2 3
#define OFFSETY 3
#define OFFSETY2 3

constexpr _length V_SLIDER_BASE_WIDTH( bool drawSnaps ){ return 12 + (!!drawSnaps) * 6; }
constexpr _length H_SLIDER_BASE_HEIGHT( bool drawSnaps ){ return 10 + (!!drawSnaps) * 5; }

_slider::_slider( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> sliderLength , _s32 value , _dimension dim , _s32 upperBound , _s32 lowerBound , _style&& style ) :
	_gadget( _gadgetType::slider , x , y , dim == _dimension::horizontal ? sliderLength : _optValue<_length>(20) , dim == _dimension::vertical ? sliderLength : _optValue<_length>(16) , move(style) )
	, intValue( mid( lowerBound , value , upperBound ) )
	, lowerBound( lowerBound )
	, upperBound( upperBound )
	, dimension( dim )
	, snap( 1 )
{
	// Add Handle
	this->sliderHandle = new _sliderHandle( 4 , 0 , dim );
	this->sliderHandle->setInternalEventHandler( onDragStart , make_callback( this , &_slider::dragHandler ) );
	this->sliderHandle->setInternalEventHandler( onDragging , make_callback( this , &_slider::dragHandler ) );
	
	// Add it
	this->addChild( this->sliderHandle );
	
	// Register update Handler and call it immediately
	this->setInternalEventHandler( onUpdate , make_callback( &_slider::updateHandler ) );
	this->updateNow();
	
	// Register Handler
	this->setInternalEventHandler( onResize , make_callback( &_slider::updateHandler ) );
	this->setInternalEventHandler( onDraw , make_callback( &_slider::refreshHandler ) );
	this->setInternalEventHandler( onKeyDown , make_callback( &_slider::keyHandler ) );
	this->setInternalEventHandler( onKeyRepeat , make_callback( &_slider::keyHandler ) );
	this->setInternalEventHandler( onBlur , make_callback( &_slider::focusHandler ) );
	this->setInternalEventHandler( onFocus , make_callback( &_slider::focusHandler ) );
	
	// Draw
	this->redraw();
}

_slider::~_slider(){
	delete this->sliderHandle;
}

_callbackReturn _slider::dragHandler( _event event )
{
	_slider* that = event.getGadget<_slider>();
	
	static _s8 deltaX;
	static _s8 deltaY;
	
	if( event == onDragStart )
	{
		deltaX = event.getPosX();
		deltaY = event.getPosY();
	}
	else /* onDragging */
	{
		_s32 realX = event.getPosX() - deltaX + that->getWidth() / 2;
		_s32 realY = event.getPosY() - deltaY + that->getHeight() / 2;
		
		if( this->dimension == _dimension::horizontal )
		{
			realX -= OFFSETX;
			_length realLength = this->getWidth() - OFFSETX2 - OFFSETX;
			this->percentage = float( realX ) / realLength;
		}
		else
		{
			realY -= OFFSETY;
			_length realLength = this->getHeight() - OFFSETY2 - OFFSETY;
			this->percentage = 1 - float( realY ) / realLength;
		}
		
		// Refresh Slider
		this->update();
	}
	
	return handled;
}

_callbackReturn _slider::keyHandler( _event event )
{
	_slider* that = event.getGadget<_slider>();
	
	if( event.getKeyCode() == _key::down || event.getKeyCode() == _key::left )
		that->decreaseInternal();
	else if( event.getKeyCode() == _key::up || event.getKeyCode() == _key::right )
		that->increaseInternal();
	
	return handled;
}

_callbackReturn _slider::focusHandler( _event event )
{
	_slider* that = event.getGadget<_slider>();
	
	// Update handle
	that->sliderHandle->redraw();
	
	return handled;
}

_callbackReturn _slider::refreshHandler( _event event )
{
	_slider* that = event.getGadget<_slider>();
	
	_bitmapPort bP = that->getBitmapPort( event );
	_length myW = bP.getWidth();
	_length myH = bP.getHeight();
	
	bP.fill( _color::fromRGB( 30 , 30 , 29 ) );
	
	// Fetch Font for labels
	_fontHandle font = _fontController::getStandardFont();
	_u8 fontSize = _fontController::getStandardFontSize();
	
	if( that->dimension == _dimension::horizontal )
	{
		bP.drawHorizontalLine( OFFSETX - 2 , 5 , myW - OFFSETX - OFFSETX2 + 4 , _color::fromRGB8( 170 , 170 , 165 ) );
		bP.drawHorizontalLine( OFFSETX - 2 , 6 , myW - OFFSETX - OFFSETX2 + 4 , _color::fromRGB8( 235 , 235 , 235 ) );
		bP.drawHorizontalLine( OFFSETX - 2 , 7 , myW - OFFSETX - OFFSETX2 + 4 , _color::white );
		bP.drawPixel( OFFSETX - 3 , 6 , _color::fromRGB8( 157 , 156 , 153 ) );
		bP.drawPixel( myW - OFFSETX2 + 2 , 6 , _color::white );
		
		bP.drawString( 1 , H_SLIDER_BASE_HEIGHT(that->snap) + 1 , font , that->lowerBoundText , _color::black , fontSize );
		bP.drawString(
			myW - font->getStringWidth( that->upperBoundText , fontSize )
			, H_SLIDER_BASE_HEIGHT( that->snap ) + 1
			, font
			, that->upperBoundText
			, _color::black
			, fontSize );
	}
	else
	{
		_coord linePos = that->snap ? 10 : 6;
		bP.drawVerticalLine( linePos -1 , OFFSETY - 2 , myH - OFFSETY - OFFSETY2 + 4 , _color::fromRGB8( 170 , 170 , 165 ) );
		bP.drawVerticalLine( linePos , OFFSETY - 2 , myH - OFFSETY - OFFSETY2 + 4 , _color::fromRGB8( 235 , 235 , 235 ) );
		bP.drawVerticalLine( linePos + 1 , OFFSETY - 2 , myH - OFFSETY - OFFSETY2 + 4 , _color::white );
		bP.drawPixel( linePos , OFFSETY - 3 , _color::fromRGB8( 157 , 156 , 153 ) );
		bP.drawPixel( linePos , myH - OFFSETY2 + 2 , _color::white );
		
		bP.drawString( V_SLIDER_BASE_WIDTH( that->snap ) + 2 , 0 , font , that->upperBoundText , _color::black , fontSize );
		bP.drawString( V_SLIDER_BASE_WIDTH( that->snap ) + 2 , myH - font->getHeight( fontSize ) , font , that->lowerBoundText , _color::black , fontSize );
	}
	
	if( !that->snap )
		return use_default;
	
	
	// Draw Indicators
	if( that->dimension == _dimension::horizontal )
	{
		_length pixelLength = that->getWidth() - OFFSETY - OFFSETY2;
		_length rangeSize = that->upperBound - that->lowerBound;
		
		for( _s32 val = that->lowerBound; val <= that->upperBound; val += that->snap )
		{
			float perc = float( val - that->lowerBound ) / rangeSize;
			
			bP.drawVerticalLine( OFFSETX - 1 + perc * pixelLength , 12 , 3 , _color::gray );
			bP.drawVerticalLine( OFFSETX + perc * pixelLength , 12 , 3 , _color::white );
		}
	}
	else
	{
		_length pixelLength = that->getHeight() - OFFSETY - OFFSETY2;
		_length rangeSize = that->upperBound - that->lowerBound;
		
		for( _s32 val = that->lowerBound; val <= that->upperBound; val += that->snap )
		{
			float perc = float( val - that->lowerBound ) / rangeSize;
			
			bP.drawHorizontalLine( 16 , OFFSETY - 1 + perc * pixelLength ,  2 , _color::gray );
			bP.drawHorizontalLine( 16 , OFFSETY + perc * pixelLength , 2 , _color::white );
			
			bP.drawHorizontalLine( 0 , OFFSETY - 1 + perc * pixelLength ,  2 , _color::gray );
			bP.drawHorizontalLine( 0 , OFFSETY + perc * pixelLength , 2 , _color::white );
		}
	}
	
	
	return use_default;
}

_callbackReturn _slider::updateHandler( _event event )
{
	_slider* that = event.getGadget<_slider>();
	
	_length rangeSize = that->upperBound - that->lowerBound;
	that->intValue = that->lowerBound + ( that->percentage * rangeSize ) + 0.5f;
	
	// Check limits
	that->intValue = mid( that->lowerBound , that->intValue , that->upperBound );
	
	if( that->dimension == _dimension::horizontal )
	{
		_length pixelLength = that->getWidth() - OFFSETX - OFFSETX2;
		
		float perc = float( that->intValue - that->lowerBound ) / rangeSize;
		
		that->sliderHandle->moveTo( OFFSETX + perc * pixelLength - that->sliderHandle->getWidth()/2 , 0 );
	}
	else
	{
		_length pixelLength = that->getHeight() - OFFSETY - OFFSETY2;
		
		float perc = 1 - float( that->intValue - that->lowerBound ) / rangeSize;
		
		that->sliderHandle->moveTo( (!!that->snap) * 3 , OFFSETX + perc * pixelLength - that->sliderHandle->getHeight()/2 );
	}
	
	_fontHandle font = _fontController::getStandardFont();
	_u8 fontSize = _fontController::getStandardFontSize();
	
	// Update Size of the slider, because the labels might have changed
	if( that->dimension == _dimension::horizontal )
		that->setHeight( H_SLIDER_BASE_HEIGHT(that->snap) + ( that->lowerBoundText.empty() && that->upperBoundText.empty() ? 0 : font->getHeight() + 1 ) );
	else
		that->setWidth(
			V_SLIDER_BASE_WIDTH(that->snap)
			+ max(
				font->getStringWidth( that->lowerBoundText , fontSize ) + 1
				, font->getStringWidth( that->upperBoundText , fontSize ) + 1
			)
		); 
	
	return handled;
}