#include "_gadget/gadget.select.h"
#include "_gadget/gadget.button.h"
#include "_resource/BMP_ScrollButtons.h"

_bitmap* graphic;// = new BMP_ScrollButtonBottom();

_callbackReturn _select::refreshHandler( _event event ){
	
	// Receive Gadget
	_select* that = event.getGadget<_select>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.normalizeClippingRects();
	
	// Border
	bP.drawRect( 0 , 0 , bP.getWidth() , bP.getHeight() , RGB( 20 , 20 , 20 ) );
	
	return use_default;
}

_select::_select( _length width , _u8 height , _coord x , _coord y , _contextMenuEntryList lst , _style style ) :
	_gadget( _gadgetType::selectbox , width , height * _system::_runtimeAttributes_->user->sOH + 2 , x , y , style )
	, entries( lst )
	, selected( -1 )
{
	// Set Padding
	this->setPadding( _padding( 1 , 1 , 1 , 1 ) );
	
	// Register Event - Handlers
	this->registerEventHandler( refresh , &_select::refreshHandler );
	
	// Refresh Me
	this->refreshBitmap();
	this->refreshList();
}

void _select::setSelected( _s32 val )
{
	if( val != this->selected )
	{
		_selectItem* s;
		for( _gadget* g : this->children )
			if( ( s = ((_selectItem*)g)) -> getIntValue() == val )
				s->setActive( true );
			else
				s->setActive( false );
		this->selected = val;
	}
}

_contextMenuEntryList& _select::getList(){
	return this->entries;
}

void _select::setList( _contextMenuEntryList lst ){
	this->entries = lst;
	this->refreshList();
}

void _select::setList( _contextMenuEntryList& lst ){
	this->entries = lst;
	this->refreshList();
}

string	_select::getStrValue(){
	return this->entries[this->selected];
}
_s32	_select::getIntValue(){
	return this->selected;
}

void	_select::setIntValue( int value ){
	this->setSelected( value );
}

void	_select::refreshList()
{
	this->removeChildren( true );
	
	int nth = 0;
	
	for( pair<const _s32,string>& entry : this->entries )
		this->addChild( new _selectItem( nth++ , this->dimensions.width - 2 , entry.second , entry.first ) );
}