#include <_screen/screen.desktop.taskbutton.h>
#include <_type/type.color.h>
#include <_controller/controller.font.h>
#include <_controller/controller.gui.h>

_callbackReturn _desktopScreenTaskButton::mouseHandler( _event event ){
	
	// Receive Gadget
	_desktopScreenTaskButton* that = event.getGadget<_desktopScreenTaskButton>();
	
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



_callbackReturn _desktopScreenTaskButton::refreshHandler( _event event )
{
	// Receive Gadget
	_desktopScreenTaskButton* that = event.getGadget<_desktopScreenTaskButton>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	_length myW = bP.getWidth();
	_length myH = bP.getHeight();
	
	// Cache the windows-bitmap that describes how windows look like
	_constBitmap& design = _guiController::getWindowsDesign( true );
	
	if( that->reference->hasFocus() || that->isPressed() )
	{		
		if( that->isPressed() ) // Darker background if the button is pressed
			bP.fill( design[9] );
		else
			bP.fill( design[3] );
		
		// String
		bP.drawString( that->reference->hasIcon() ? 11 : 3 , 0 , _fontController::getStandardFont() , that->reference->getStrValue() , _color::fromRGB( 27 , 27 , 27 ) );
		
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
		bP.drawString( that->reference->hasIcon() ? 11 : 3 , 0 , _fontController::getStandardFont() , that->reference->getStrValue() , _color::fromRGB( 29  , 29 , 29 ) );
		
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
		bP.copyTransparent( 3 , 2 , that->reference->getIcon() );
	
	// Corners!
	bP.drawPixel( myW - 1 , myH - 1 , _color::transparent );
	bP.drawPixel( myW - 1 , 0 , _color::transparent );
	bP.drawPixel( 0 , 0 , _color::transparent );
	bP.drawPixel( 0 ,  myH - 1 , _color::transparent );
	
	return use_default;
}



_desktopScreenTaskButton::_desktopScreenTaskButton( _optValue<_coord> x , _optValue<_coord> y , _window* reference , _style&& style ) :
	_button( x , y , 20 , 10 , "" , style | _style::notFocusable | _style::canNotTakeFocus  )
	, reference( reference )
{
	this->setFontColor( _color::fromRGB( 27 , 27 , 27 ) );
	this->setAlign( _align::left );
	
	// Register some event handlers
	this->setInternalEventHandler( onMouseClick , make_callback( &_desktopScreenTaskButton::mouseHandler ) );
	this->setInternalEventHandler( onDraw , make_callback( &_desktopScreenTaskButton::refreshHandler ) );
	
	// Refresh
	this->redraw();
}