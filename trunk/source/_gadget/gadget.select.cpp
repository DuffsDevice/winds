#include "_gadget/gadget.select.h"
#include "_gadget/gadget.button.h"
#include "_resource/BMP_ScrollButtons.h"
#include "_type/type.system.h"

_callbackReturn _select::refreshHandler( _event event )
{
	// Receive Gadget
	_select* that = event.getGadget<_select>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().relativate( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.normalizeClippingRects();
	
	bP.fill( COLOR_WHITE );
	
	bP.drawRect( 0 , 0 , bP.getWidth() , bP.getHeight() , RGB( 9 , 13 , 19 ) );
	
	return use_default;
}

_select::_select( _length width , _u8 height , _coord x , _coord y , _contextMenuEntryList lst , _style style ) :
	_scrollArea( width , height * _system::_runtimeAttributes_->user->sOH + 2 , x , y , _scrollType::prevent , _scrollType::meta , style )
	, entries( lst )
	, selected( -1 )
{
	this->setPaddingOffset( _padding( 1 , 1 , 1 , 1 ) );
	this->setType( _gadgetType::selectbox );
	
	this->registerEventHandler( refresh , new _staticCallback( &_select::refreshHandler ) );
	
	// Refresh Me
	this->refreshList();
}

void _select::setSelected( _s32 val )
{
	if( val != this->selected )
	{
		_selectItem* s;
		for( _gadget* g : this->children )
			if( ( s = (_selectItem*)g) -> getIntValue() == val )
				s->setActive( true );
			else
				s->setActive( false );
		this->selected = val;
	}
}

void	_select::refreshList()
{
	this->removeChildren( true );
	
	int nth = 0;
	
	_selectItem* test = nullptr;
	
	for( pair<const _s32,string>& entry : this->entries )
	{
		if( selected == entry.first )
			this->addChild( test = new _selectItem( nth++ , this->dimensions.width - 2 , entry.second , entry.first ) );
		else
			this->addChild( new _selectItem( nth++ , this->dimensions.width - 2 , entry.second , entry.first ) );
	}
	
	if( !test )
		this->selected = -1;
	else
		test->setActive( true );
}