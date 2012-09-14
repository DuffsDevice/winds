#include "_gadget/gadget.startupScreen.h"

#include <nds/arm9/console.h>

_pixel SUpalette[] = { 
	RGB( 0 , 6 , 19 ) , RGB( 0 , 6 , 15 ) , RGB( 21 , 24 , 31 ) , // --- Topper Bar ( Main color , dark Main , bright Accent )
	RGBHEX( 0x5A7EDC ) , // Bright Background
	RGBHEX( 0x3733ac ) , RGBHEX( 0x0f319f ) , RGBHEX( 0xd9946b ) // Bottom Bar ( Main - left side , right side , orange accent )
};

_u8 accntPos = 110;

_gadgetEventReturnType _startupScreen::refreshHandler( _gadgetEvent event )
{
	_gadget* that = event.getGadget();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.getArgs().hasClippingRects() )
		bP.addClippingRects( event.getArgs().getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.resetClippingRects();
	
	bP.fill( SUpalette[3] );
	
	// ---------------- The Topper Dark Bar
	bP.drawFilledRect( 0 , 0 , SCREEN_WIDTH , 17 , SUpalette[0] );
	
	bP.drawHorizontalGradient( 0 , 17 , accntPos , 1 , SUpalette[0] , SUpalette[1] );
	bP.drawHorizontalGradient( accntPos , 17 , SCREEN_WIDTH - accntPos , 1 , SUpalette[1] , SUpalette[0] );
	
	bP.drawHorizontalLine( 0 , 18 , SCREEN_WIDTH , SUpalette[0] );
	
	bP.drawHorizontalGradient( 0 , 19 , accntPos , 1 , SUpalette[3] , SUpalette[2] );
	bP.drawHorizontalGradient( accntPos , 19 , SCREEN_WIDTH - accntPos , 1 , SUpalette[2] , SUpalette[3] );
	// ----------------
	
	// ---------------- The Bottom Less Dark Bar
	bP.drawHorizontalGradient( 0 , SCREEN_HEIGHT - 23 , SCREEN_WIDTH , 23 , SUpalette[4] , SUpalette[5] );
	
	bP.drawHorizontalGradient( 0 , SCREEN_HEIGHT - 24 , accntPos , 1 , SUpalette[5] , SUpalette[6] );
	bP.drawHorizontalGradient( accntPos , SCREEN_HEIGHT - 24 , SCREEN_WIDTH - accntPos , 1 , SUpalette[6] , SUpalette[5] );
	// ----------------
	
	return use_default;
}

_startupScreen::_startupScreen( _u8 bgId , _gadgetStyle style ) :
	_gadgetScreen( bgId , style )
{	
	_gadget* refresher = new _gadget( SCREEN_WIDTH , SCREEN_HEIGHT , 0 , 0 );
	refresher->registerEventHandler( "refresh" , &_startupScreen::refreshHandler );
	refresher->refreshBitmap();
	this->addChild( refresher );
	
	//! Refresh me
	this->refreshBitmap();
}

bool _startupScreen::focusChild( _gadget* child )
{
	if( !child )
		return false;
	
	_gadgetList::iterator itTemp = find( this->children.begin() , this->children.end() , child );
	
	// Blur the Previously focused gadget
	if( !child->hasFocus() )
		this->blurEventChild();
	else
		return false;
	
	child->focused = true;
	
	if( child->getType() == _gadgetType::_plain || *itTemp == *( ++this->children.rbegin() ) )
		return true;
	
	// Move the gadget to "top"
	this->children.splice( this->children.end() , this->children , itTemp );
	
	// Move the keyboard top the "top", so the child will be at pos No. 2
	//this->children.splice( this->children.end() , this->children , find( this->children.begin() , this->children.end() , this->taskboard ) );
	
	// Refresh the Gadget
	child->bubbleRefresh();
	
	return true;
}