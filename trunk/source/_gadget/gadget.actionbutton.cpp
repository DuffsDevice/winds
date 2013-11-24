#include "_gadget/gadget.actionButton.h"
#include "_resource/BMP_ActionButtons.h"

_constbitmap _actionButton::bitmaps[5] = { BMP_ActionBtnNext() , BMP_ActionBtnPrev() , BMP_ActionBtnShutdown() , BMP_ActionBtnLogOff() , BMP_ActionBtnSwitchUser() };

_callbackReturn _actionButton::refreshHandler( _event event )
{
	// Receive Gadget
	_actionButton* that = event.getGadget<_actionButton>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	bP.copyTransparent( 0 , 0 , that->bitmaps[ _u8( that->type ) ] );
	
	return use_default;
}

_actionButton::_actionButton( _optValue<_coord> x , _optValue<_coord> y , _actionButtonType type , _style&& style ) :
	_button( x , y , 9 , 9 , "" , (_style&&)style )
	, type( type )
{
	// Register handler
	this->setInternalEventHandler( onDraw , make_callback( &_actionButton::refreshHandler ) );
	
	// Refresh Me
	this->redraw();
}

_map<_actionButtonType,string> actionButtonType2string =
{
	{ _actionButtonType::next , "next" },
	{ _actionButtonType::prev , "prev" },
	{ _actionButtonType::logoff , "logoff" },
	{ _actionButtonType::shutdown , "shutdown" },
	{ _actionButtonType::switchuser , "switchuser" },
};

_map<string,_actionButtonType> string2actionButtonType =
{
	{ "next" , _actionButtonType::next },
	{ "prev" , _actionButtonType::prev },
	{ "logoff" , _actionButtonType::logoff },
	{ "shutdown" , _actionButtonType::shutdown },
	{ "switchuser" , _actionButtonType::switchuser },
};