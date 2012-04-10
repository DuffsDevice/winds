#include "_gadget/gadget.textbox.h"
#include "_gadget/gadget.windows.h"

void _textbox::onResize(){
	
}

_gadgetEventReturnType _textbox::refreshHandler( _gadgetEvent event )
{
	_textbox* that = (_textbox*)event.getGadget();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.getArgs().isBubbleRefresh() )
		bP.addClippingRects( event.getArgs().getDamagedRects().toRelative( that->getAbsoluteDimensions() ) );
	else
		bP.resetClippingRects();
	
	_length myH = bP.getHeight();
	_length myW = bP.getWidth();
	
	
	bP.drawFilledRect( 1 , 1 , myW - 2 , myH - 2 , RGB( 31 , 31 , 31 ) );
	
	if( !that->pressed )
		bP.drawRect( 0 , 0 , myW , myH , RGB( 13 , 16 , 23 ) );
	else
		bP.drawRect( 0 , 0 , myW , myH , RGB( 9 , 13 , 19 ) );
	
	return use_default;
}

_gadgetEventReturnType _textbox::keyHandler( _gadgetEvent event )
{
	_textbox* that = (_textbox*)event.getGadget();
	
	string val = that->getStrValue();
	
	switch( event.getArgs().getKeyCode() ){
		case DSWindows::KEY_BACKSPACE:
			val = val.substr( 0 , val.length() - 1 );
			break;
		default:
			val.insert( val.end() , event.getArgs().getKeyCode() );
			break;
	}
	
	that->setStrValue( val );
	
	return handled;
}

_gadgetEventReturnType _textbox::dragHandler( _gadgetEvent event )
{
	_textbox* that = (_textbox*)event.getGadget();
	
	if( event.getType() == dragStart )
		return handled;
	else if( event.getType() == dragging ){
		
		if( !that->getAbsoluteDimensions().contains( event.getArgs().getPosX() , event.getArgs().getPosY() ) )
		{
			// I'm not pressed anymore!
			that->pressed = false;
			
			// Refresh my parents
			that->handleEvent( refresh );
		}
		return handled;
	}
	
	return not_handled;
}

_gadgetEventReturnType _textbox::mouseHandler( _gadgetEvent event )
{
	_textbox* that = (_textbox*)event.getGadget();
	
	if( event.getType() == mouseClick ){
		_windows* win = (_windows*)that->getWindows();
		
		if( !win )
			return not_handled;
		
		win->getTaskBoard()->openDialog( that );
		
		return handled;
	}
	else if( event.getType() == mouseDown ){
		that->pressed = true;
		// Refresh
		that->handleEvent( refresh );
	}
	else if( event.getType() == mouseUp ){
		that->pressed = false;
		// Refresh
		that->handleEvent( refresh );
	}
	
	return handled;
}

_textbox::_textbox( _coord x , _coord y , _length width , string text , _gadgetStyle style ) :
	_gadget( _gadgetType::textbox , width , 10 , x , y , style )
	, _interface_input( "" )
	, label( new _label( 2 , 1 , text ) )
	, pressed( false )
{
	// Set Label
	this->label->setVAlign( middle );
	this->label->setAlign( left );
	this->addChild( this->label );
	
	
	// Regsiter Handling Functions for events
	this->unregisterEventHandler( mouseDoubleClick );
	this->registerEventHandler( refresh , &_textbox::refreshHandler );
	this->registerEventHandler( mouseClick , &_textbox::mouseHandler );
	this->registerEventHandler( mouseDown , &_textbox::mouseHandler );
	this->registerEventHandler( mouseUp , &_textbox::mouseHandler );
	this->registerEventHandler( keyClick , &_textbox::keyHandler );
	this->registerEventHandler( dragStart , &_textbox::dragHandler );
	this->registerEventHandler( dragging , &_textbox::dragHandler );
	
	// Refresh Myself
	this->refreshBitmap();
}

_textbox::~_textbox(){
	delete this->label;
}