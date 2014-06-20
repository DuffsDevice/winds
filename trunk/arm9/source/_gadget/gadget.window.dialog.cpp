#include "_gadget/gadget.window.dialog.h"

_callbackReturn _dialogWindow::keyHandler( _event event )
{
	_dialogWindow* that = event.getGadget<_dialogWindow>();
	
	if( event.getKeyCode() == _key::b ){
		that->close();
		return handled;
	}
	
	return use_default;
}

_dialogWindow::_dialogWindow( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string title , _style&& style ) :
	_window( x , y , width , height , move(title) , false , true , move(style) )
{
	this->setInternalEventHandler( onKeyClick , make_callback( &_dialogWindow::keyHandler ) );
}