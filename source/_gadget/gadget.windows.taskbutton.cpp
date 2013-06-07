#include "_gadget/gadget.windows.taskbutton.h"
#include "_type/type.system.h"
#include "_type/type.color.h"

_callbackReturn _windowsTaskButton::mouseHandler( _event event ){
	
	// Receive Gadget
	_windowsTaskButton* that = event.getGadget<_windowsTaskButton>();
	
	if( that->reference->isMinimized() ) // Restore our window if it was minimized
	{
		that->reference->restore();
		that->reference->focus();
	}
	else if( that->reference->hasFocus() ) // Minimize if the reference (our window) has focus
		that->reference->minimize();
	else
		that->reference->focus(); // If it wasn't minimized and hadn't focus, focus it
	
	return handled;
}



_callbackReturn _windowsTaskButton::refreshHandler( _event event )
{
	// Receive Gadget
	_windowsTaskButton* that = event.getGadget<_windowsTaskButton>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsolutePosition() ) );
	else
		bP.normalizeClippingRects();
	
	_length myW = bP.getWidth();
	_length myH = bP.getHeight();
	
	// Cache the windows-bitmap that describes how windows look like
	const _bitmap& design = _system::_rtA_->getWindowsDesignActive();
	
	if( that->reference->hasFocus() || that->isPressed() )
	{		
		if( that->isPressed() ) // Darker background if the button is pressed
			bP.fill( design[9] );
		else
			bP.fill( design[3] );
		
		// String
		bP.drawString( that->reference->hasIcon() ? 11 : 3 , 1 , _system::getFont() , that->reference->getStrValue() , RGB( 27 , 27 , 27 ) );
		
		// Topper Line Bright
		bP.drawHorizontalLine( 1 , 0 , myW - 2 , design[1] );
		
		// Top Line Dark
		bP.drawHorizontalLine( 1 , 1 , myW - 2 , design[9] );
		
		// Bottom Dark Line
		bP.drawHorizontalLine( 0 , myH - 1 , myW - 1 , design[9] );
		
		// Left Dark Line
		bP.drawVerticalLine( 0 , 2 , myH - 3 , design[9] );
		
		// Right Dark Line
		bP.drawVerticalLine( myW - 1 , 2 , myH - 3 , design[9] );
	}
	else
	{
		// Quite bright backgrund
		bP.fill( design[1] );
		
		// String
		bP.drawString( that->reference->hasIcon() ? 11 : 3 , 1 , _system::getFont() , that->reference->getStrValue() , RGB( 29  , 29 , 29 ) );
		
		// Create brighter blue
		_color c = design[1];
		c.setL( c.getL() + 10 );
		
		// Top Line Dark
		bP.drawHorizontalLine( 1 , 0 , myW - 2 , design[8] );
		
		// Top+1 Line Bright
		bP.drawHorizontalLine( 2 , 1 , myW - 3 , c.getColor() );
		
		// Bottom Dark Line
		bP.drawHorizontalLine( 0 , myH - 1 , myW - 1 , design[9] );
		
		// Left Line Dark
		bP.drawVerticalLine( 0 , 2 , myH - 3 , design[8] );
		
		// Left+1 Line Bright
		bP.drawVerticalLine( 1 , 2 , myH - 3 , c.getColor() );
		
		// Right Dark Line
		bP.drawVerticalLine( myW - 1 , 1 , myH - 2 , design[3] );
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
	_button( 20 , 10 , x , y , "" , style | _styleAttr::canNotReceiveFocus | _styleAttr::canNotTakeFocus  )
	, reference( reference )
{
	this->setFontColor( RGB( 27 , 27 , 27 ) );
	this->setAlign( _align::left );
	this->registerEventHandler( onAction , new _staticCallback( &_windowsTaskButton::mouseHandler ) );
	this->registerEventHandler( refresh , new _staticCallback( &_windowsTaskButton::refreshHandler ) );
	
	this->refreshBitmap();
}