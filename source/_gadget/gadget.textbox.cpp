#include "_gadget/gadget.textbox.h"
#include "_gadget/gadget.windows.h"

void _textbox::onResize(){
	
}

void _textbox::standardPaint( _gadgetEvent event )
{
	_bitmapPort bP = this->getBitmapPort();
	
	if( event.getArgs().isBubbleRefresh() )
		bP.addClippingRects( event.getArgs().getDamagedRects().toRelative( this->getAbsoluteDimensions() ) );
	else
		bP.resetClippingRects();
	
	_length myH = bP.getHeight();
	_length myW = bP.getWidth();
	
	
	bP.drawFilledRect( 1 , 1 , myW - 2 , myH - 2 , RGB( 31 , 31 , 31 ) );
	
	if( !this->pressed )
		bP.drawRect( 0 , 0 , myW , myH , RGB( 13 , 16 , 23 ) );
	else
		bP.drawRect( 0 , 0 , myW , myH , RGB( 9 , 13 , 19 ) );
	
	this->gadgetRefreshHandler( event );
	
}

_gadgetEventReturnType _textbox::refreshHandler( _gadgetEvent e )
{
	this->standardPaint( e );
	
	if( !e.getArgs().isBubbleRefresh() )
		this->bubbleRefresh();
	
	return handled;
}

_gadgetEventReturnType _textbox::keyHandler( _gadgetEvent e )
{
	string val = this->getStrValue();
	
	switch( e.getArgs().getKeyCode() ){
		case DSWindows::KEY_BACKSPACE:
			val = val.substr( 0 , val.length() - 1 );
			break;
		default:
			val.insert( val.end() , e.getArgs().getKeyCode() );
			break;
	}
	
	this->setStrValue( val );
	
	return handled;
}

_gadgetEventReturnType _textbox::dragHandler( _gadgetEvent event )
{
	if( event.getType() == dragStart )
		return handled;
	else if( event.getType() == dragging ){
		
		if( !this->getAbsoluteDimensions().contains( event.getArgs().getPosX() , event.getArgs().getPosY() ) )
		{
			// I'm not pressed anymore!
			this->pressed = false;
			
			// Refresh my parents
			this->handleEvent( refresh );
		}
		return handled;
	}
	
	return not_handled;
}

_gadgetEventReturnType _textbox::mouseHandler( _gadgetEvent e )
{
	if( e.getType() == mouseClick ){
		_windows* win = (_windows*)this->getWindows();
		
		if( !win )
			return not_handled;
		
		win->getTaskBoard()->openDialog( this );
		
		return handled;
	}
	else if( e.getType() == mouseDown ){
		this->pressed = true;
		// Refresh
		this->handleEvent( refresh );
	}
	else if( e.getType() == mouseUp ){
		this->pressed = false;
		// Refresh
		this->handleEvent( refresh );
	}
	
	return handled;
}

_textbox::_textbox( _coord x , _coord y , _length width , string text , _gadgetStyle style ) :
	_gadget( _gadgetType::textbox , width , 10 , x , y , style )
	, _interface_input( "" )
{
	// Create A Label
	this->label = new _label( 2 , 1 , text );
	this->label->setVAlign( middle );
	this->label->setAlign( left );
	this->addChild( this->label );
	
	this->pressed = false;
	
	
	// Regsiter Handling Functions for events
	this->registerDefaultEventHandler( refresh , static_cast<_gadgetDefaultEventHandler>(&_textbox::refreshHandler) );
	this->registerDefaultEventHandler( mouseClick , static_cast<_gadgetDefaultEventHandler>(&_textbox::mouseHandler) );
	this->registerDefaultEventHandler( mouseDown , static_cast<_gadgetDefaultEventHandler>(&_textbox::mouseHandler) );
	this->registerDefaultEventHandler( mouseUp , static_cast<_gadgetDefaultEventHandler>(&_textbox::mouseHandler) );
	this->registerDefaultEventHandler( keyClick , static_cast<_gadgetDefaultEventHandler>(&_textbox::keyHandler) );
	this->registerDefaultEventHandler( dragStart , static_cast<_gadgetDefaultEventHandler>(&_textbox::dragHandler) );
	this->registerDefaultEventHandler( dragging , static_cast<_gadgetDefaultEventHandler>(&_textbox::dragHandler) );
	
	// Refresh Myself
	this->refreshBitmap();
}

_textbox::~_textbox(){
	delete this->label;
}