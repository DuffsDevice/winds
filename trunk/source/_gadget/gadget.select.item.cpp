#include "_gadget/gadget.select.item.h"
#include "_gadget/gadget.select.h"
#include "_type/type.system.h"

_callbackReturn _selectItem::mouseHandler( _event event )
{	
	_selectItem* that = event.getGadget<_selectItem>();
	_select* parent = (_select*)that->parent;
	
	if( !that->active )
	{
		parent->setSelected( that->intVal );
		
		//! Trigger onChange-Event
		parent->triggerEvent( _event( onChange ) );
	}
	
	return handled;
}

_callbackReturn _selectItem::refreshHandler( _event event ){
	
	// Receive Gadget
	_selectItem* that = event.getGadget<_selectItem>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsolutePosition() ) );
	else
		bP.normalizeClippingRects();
	
	if( that->active )
	{
		bP.fill( RGB255( 10 , 36 , 106 ) );
		bP.drawString( 1 , 1 , _system::getFont() , that->getStrValue() , COLOR_WHITE , _system::_rtA_->getDefaultFontSize() );
	}
	else
	{
		bP.fill( COLOR_WHITE );
		bP.drawString( 1 , 1 , _system::getFont() , that->getStrValue() , COLOR_BLACK , _system::_rtA_->getDefaultFontSize() );
	}
	
	return use_default;
}

_selectItem::_selectItem( _s32 nth , _length width , string str , _s32 value , _style style ) :
	_gadget( _gadgetType::selectitem , width , _system::getUser()->sOH , 0 , nth * _system::getUser()->sOH , style )
	, strVal( str )
	, intVal( value )
	, active( false )
{
	// Register Event - Handlers
	this->registerEventHandler( refresh , new _staticCallback( &_selectItem::refreshHandler ) );
	this->registerEventHandler( mouseDown , new _staticCallback( &_selectItem::mouseHandler ) );
	
	// Refresh Me
	this->refreshBitmap();
}