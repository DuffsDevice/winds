#include "_gadget/gadget.list.item.h"
#include "_gadget/gadget.list.h"
#include "_type/type.system.h"

_gadgetEventReturnType _selectItem::mouseHandler( _gadgetEvent event )
{	
	_selectItem* that = event.getGadget<_selectItem>();
	
	if( event.getType() == "mouseClick" )
	{
		that->active = true;
		((_select*)that->parent)->setSelected( that->intVal );
	}
	else if( event.getType() == "unselect" )
		that->active = false;
	
	that->bubbleRefresh( true );
	
	return handled;
}

_gadgetEventReturnType _selectItem::refreshHandler( _gadgetEvent event ){
	
	// Receive Gadget
	_selectItem* that = event.getGadget<_selectItem>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.getArgs().hasClippingRects() )
		bP.addClippingRects( event.getArgs().getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.resetClippingRects();
	
	_length myH = bP.getHeight();
	_length myW = bP.getWidth();
	
	if( that->active )
	{
		bP.fill( RGB255( 10 , 36 , 106 ) );
		bP.drawString( 1 , 1 , _system_->_runtimeAttributes_->defaultFont , that->getStrValue() , COLOR_WHITE , _system_->_runtimeAttributes_->defaultFontSize );
	}
	else
	{
		bP.fill( COLOR_WHITE );
		bP.drawString( 1 , 1 , _system_->_runtimeAttributes_->defaultFont , that->getStrValue() , COLOR_BLACK , _system_->_runtimeAttributes_->defaultFontSize );
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
	this->registerEventHandler( "mouseClick" , &_selectItem::mouseHandler );
	this->registerEventHandler( "unselect" , &_selectItem::mouseHandler );
	
	// Refresh Me
	this->refreshBitmap();
}