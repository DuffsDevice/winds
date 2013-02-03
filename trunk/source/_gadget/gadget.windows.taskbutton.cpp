#include "_gadget/gadget.windows.taskbutton.h"
#include "_type/type.system.h"
#include "_type/type.color.h"

_callbackReturn _windowsTaskButton::mouseHandler( _event event ){
	
	// Receive Gadget
	_windowsTaskButton* that = event.getGadget<_windowsTaskButton>();
	
	
	if( that->reference->isMinimized() )
	{
		that->reference->restore();
		that->reference->handleEvent( focus );
	}
	else if( that->reference->hasFocus() )
		that->reference->minimize();
	else
		that->reference->handleEvent( focus );
	
	return handled;
}



_callbackReturn _windowsTaskButton::refreshHandler( _event event )
{
	// Receive Gadget
	_windowsTaskButton* that = event.getGadget<_windowsTaskButton>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.normalizeClippingRects();
	
	_length myW = bP.getWidth();
	_length myH = bP.getHeight();
	
	if( that->reference->hasFocus() )
	{
		bP.fill( _system::_runtimeAttributes_->windowBar[3] );
		
		// String
		bP.drawString( that->reference->hasIcon() ? 11 : 3 , 1 , _system::getFont() , that->reference->getStrValue() , RGB( 27 , 27 , 27 ) );
		
		// Topper Line Bright
		bP.drawHorizontalLine( 1 , 0 , myW - 2 , _system::_runtimeAttributes_->windowBar[1] );
		
		// Top Line Dark
		bP.drawHorizontalLine( 1 , 1 , myW - 2 , _system::_runtimeAttributes_->windowBar[9] );
		
		// Bottom Dark Line
		bP.drawHorizontalLine( 0 , myH - 1 , myW - 1 , _system::_runtimeAttributes_->windowBar[9] );
		
		// Left Dark Line
		bP.drawVerticalLine( 0 , 2 , myH - 3 , _system::_runtimeAttributes_->windowBar[9] );
		
		// Right Dark Line
		bP.drawVerticalLine( myW - 1 , 2 , myH - 3 , _system::_runtimeAttributes_->windowBar[9] );
	}
	else
	{
		bP.fill( _system::_runtimeAttributes_->windowBar[1] );
		
		// String
		bP.drawString( that->reference->hasIcon() ? 11 : 3 , 1 , _system::getFont() , that->reference->getStrValue() , RGB( 29  , 29 , 29 ) );
		
		// Create brighter blue
		_color c;
		c.setColor( _system::_runtimeAttributes_->windowBar[1] );
		c.setL( c.getL() + 10 );
		
		// Top Line Dark
		bP.drawHorizontalLine( 1 , 0 , myW - 2 , _system::_runtimeAttributes_->windowBar[8] );
		
		// Top+1 Line Bright
		bP.drawHorizontalLine( 2 , 1 , myW - 3 , c.getColor() );
		
		// Bottom Dark Line
		bP.drawHorizontalLine( 0 , myH - 1 , myW - 1 , _system::_runtimeAttributes_->windowBar[9] );
		
		// Left Line Dark
		bP.drawVerticalLine( 0 , 2 , myH - 3 , _system::_runtimeAttributes_->windowBar[8] );
		
		// Left+1 Line Bright
		bP.drawVerticalLine( 1 , 2 , myH - 3 , c.getColor() );
		
		// Right Dark Line
		bP.drawVerticalLine( myW - 1 , 1 , myH - 2 , _system::_runtimeAttributes_->windowBar[3] );
	}
	
	if( that->reference->hasIcon() )
		bP.copyTransparent( 3 , 2 , *that->reference->getIcon() );
	
	// Corners!
	bP.drawPixel( myW - 1 , myH - 1 , NO_COLOR );
	bP.drawPixel( myW - 1 , 0 , NO_COLOR );
	bP.drawPixel( 0 , 0 , NO_COLOR );
	bP.drawPixel( 0 ,  myH - 1 , NO_COLOR );
	
	return use_default;
}



_windowsTaskButton::_windowsTaskButton( _coord x , _coord y , _window* reference , _style style ) :
	_button( 20 , 10 , x , y , "" , style )
	, reference( reference )
{
	_style st = this->getStyle();
	st.canTakeFocus = false;
	this->setStyle( st );
	this->setFontColor( RGB( 27 , 27 , 27 ) );
	this->setAlign( _align::left );
	this->registerEventHandler( onAction , new _staticCallback( &_windowsTaskButton::mouseHandler ) );
	this->registerEventHandler( refresh , new _staticCallback( &_windowsTaskButton::refreshHandler ) );
	
	this->refreshBitmap();
}