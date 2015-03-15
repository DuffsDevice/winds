#include <_gadget/gadget.window.dialog.h>
#include <_gadget/gadget.button.h>
#include <_type/type.windows.soundbank.h>
#include <_func/func.inputemulator.h>

_callbackReturn _dialogWindow::submitEscapeHandler( _event event )
{
	_dialogWindow* that = event.getGadget<_dialogWindow>();
	
	if( event.getKeyCode() == _key::b )
		that->close();
	else if( event.getKeyCode() == _key::a )
	{
		bool onlyDisabledButtonFound = false;
		
		for( _gadget* child : that->getChildren( false ) )
		{
			if( child->getType() != _gadgetType::button )
				continue;
			
			_button* btn = (_button*)child;
			
			if( !btn->isAutoSelect() )
				continue;
			
			if( btn->isEnabled() ){
				emulateMouseClick( btn ); // Emulates a click on the button
				break;
			}
			else
				onlyDisabledButtonFound = true;
		}
		
		if( onlyDisabledButtonFound )
			_windowsSoundBank::play( _windowsSound::ping );
	}
	else
		return use_default;
	
	return handled;
}

_dialogWindow::_dialogWindow( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , wstring title , _style&& style ) :
	_window( x , y , width , height , move(title) , false , true , move(style) )
{
	this->setUserEventHandler( onKeyDown , make_callback( &_dialogWindow::submitEscapeHandler ) );
}