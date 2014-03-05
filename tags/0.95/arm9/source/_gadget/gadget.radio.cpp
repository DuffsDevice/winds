#include "_gadget/gadget.radio.h"
#include "_type/type.singlevaluegroup.h"

// Graphics
#include "_resource/resource.image.radiobuttons.h"

_constBitmap _radio::graphic[4] = { BMP_RadioChecked() , BMP_RadioUnchecked() , BMP_RadioBorder() , BMP_RadioBorderPressed() };

_callbackReturn _radio::refreshHandler( _event event )
{
	// Receive Gadget
	_radio* that = (_radio*)event.getGadget();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	bP.fill( _color::transparent );
	
	// Border
	bP.copyTransparent( 0 , 0 , that->graphic[ that->isPressed() + 2 ]);
	
	// Outside
	bP.copyTransparent( 2 , 2 , that->graphic[ that->getIntValue() ]);
	
	return use_default;
}

_callbackReturn _radio::keyHandler( _event event )
{
	_radio* that = event.getGadget<_radio>();
	
	// Abort if no other _radios are connected!
	if( !that->group )
		return not_handled;
	
	if( event.getKeyCode() == _key::down || event.getKeyCode() == _key::right )
		that->group->enableNext();
	else if( event.getKeyCode() == _key::up || event.getKeyCode() == _key::left )
		that->group->enablePrev();
	
	return handled;
}

_callbackReturn _radio::mouseHandler( _event event )
{
	// Receive Gadget
	_radio* that = event.getGadget<_radio>();
	
	// Abort if 'i' belong to no _radiogroup
	if( !that->group )
		return not_handled;
	
	// Ensure it is not check already
	if( !that->getIntValue() )
	{
		// Check myself
		that->group->enableSelector( that ); // Kotzen...
		
		// Notify for a change
		that->triggerEvent( onEdit );
	}
	
	return handled;
}

_radio::_radio( _optValue<_coord> x , _optValue<_coord> y , _singleValueGroup<_radio>* group , _s32 assocValue , _style&& style ) :
	_gadget( _gadgetType::radiobox , x , y , 9 , 9 , (_style&&)style )
	, intValue( 0 )
	, group( nullptr ) // Will be set when the _radio will be added to the _radiogroup
{
	if( group )
		group->addSelector( this , assocValue );
	
	// Register my handler as the default Refresh-Handler
	this->setInternalEventHandler( onDraw , make_callback( &_radio::refreshHandler ) );
	this->setInternalEventHandler( onMouseClick , make_callback( &_radio::mouseHandler ) );
	this->setInternalEventHandler( onKeyDown , make_callback( &_radio::keyHandler ) );
	this->setInternalEventHandler( onKeyRepeat , make_callback( &_radio::keyHandler ) );
	this->setInternalEventHandler( onMouseEnter , _gadgetHelpers::eventForwardRefresh() );
	this->setInternalEventHandler( onMouseLeave , _gadgetHelpers::eventForwardRefresh() );
	
	// Refresh Me
	this->redraw();
}

_radio::~_radio()
{
	if( this->group )
		this->group->removeSelector( this );
}