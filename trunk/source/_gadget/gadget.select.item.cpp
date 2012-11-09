#include "_gadget/gadget.select.item.h"
#include "_gadget/gadget.select.h"
#include "_type/type.system.h"

_gadgetEventReturnType _selectItem::mouseHandler( _gadgetEvent event )
{	
	_selectItem* that = event.getGadget<_selectItem>();
	_select* parent = (_select*)that->parent;
	
	parent->triggerEvent( _gadgetEvent( "listener" ) );
	parent->setSelected( that->intVal );
	
	return handled;
}

_gadgetEventReturnType _selectItem::refreshHandler( _gadgetEvent event ){
	
	// Receive Gadget
	_selectItem* that = event.getGadget<_selectItem>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.normalizeClippingRects();
	
	if( that->active )
	{
		bP.fill( RGB255( 10 , 36 , 106 ) );
		bP.drawString( 1 , 1 , _system_->getFont() , that->getStrValue() , COLOR_WHITE , _system_->_runtimeAttributes_->defaultFontSize );
	}
	else
	{
		bP.fill( COLOR_WHITE );
		bP.drawString( 1 , 1 , _system_->getFont() , that->getStrValue() , COLOR_BLACK , _system_->_runtimeAttributes_->defaultFontSize );
	}
	
	return use_default;
}

_selectItem::_selectItem( _s32 nth , _length width , string str , _s32 value , _gadgetStyle style ) :
	_gadget( _gadgetType::selectitem , width , _system_->_runtimeAttributes_->user->sOH , 0 , nth * _system_->_runtimeAttributes_->user->sOH , style )
	, strVal( str )
	, intVal( value )
	, active( false )
{
	// Register Event - Handlers
	this->registerEventHandler( "refresh" , &_selectItem::refreshHandler );
	this->registerEventHandler( "mouseDown" , &_selectItem::mouseHandler );
	
	// Refresh Me
	this->refreshBitmap();
}