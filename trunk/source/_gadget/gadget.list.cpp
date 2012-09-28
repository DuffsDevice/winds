#include "_gadget/gadget.list.h"
#include "_gadget/gadget.button.h"
#include "_resource/BMP_ScrollButtons.h"

_bitmap* graphic;// = new BMP_ScrollButtonBottom();

_gadgetEventReturnType _select::dragHandler( _gadgetEvent event )
{
	
	return use_default;
}

_gadgetEventReturnType _select::refreshHandler( _gadgetEvent event ){
	
	// Receive Gadget
	_select* that = event.getGadget<_select>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.getArgs().hasClippingRects() )
		bP.addClippingRects( event.getArgs().getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.resetClippingRects();
	
	// Border
	bP.drawRect( 0 , 0 , bP.getWidth() , bP.getHeight() , RGB( 20 , 20 , 20 ) );
	
	return use_default;
}

_select::_select( _length width , _length height , _coord x , _coord y , _contextMenuEntryList lst , _gadgetStyle style ) :
	_gadget( _gadgetType::selectbox , width , height , x , y , style )
	, entries( lst )
	, selected( 0 )
{
	// Set Padding
	this->setPadding( _padding( 1 , 1 , 1 , 1 ) );
	
	// Register Event - Handlers
	this->registerEventHandler( "refresh" , &_select::refreshHandler );
	this->registerEventHandler( "dragging" , &_select::dragHandler );
	this->registerEventHandler( "dragStart" , &_select::dragHandler );
	this->registerEventHandler( "dragStop" , &_select::dragHandler );
	
	// Refresh Me
	this->refreshBitmap();
	
	this->refreshList();
}

void _select::setSelected( _s32 val )
{
	if( val != this->selected )
	{
		auto it = find_if( this->children.begin() , this->children.end() , [&]( _gadget* g )->bool{ return ((_selectItem*)g)->getIntValue() == this->selected; } );
		
		if( it != this->children.end() )
			((_selectItem*)(*it))->setActive( false );
		
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
	this->selected = value;
	this->refreshList();
}

void	_select::refreshList()
{
	this->removeChildren( true );
	
	int nth = 0;
	
	for( pair<const _s32,string>& entry : this->entries )
		this->addChild( new _selectItem( nth++ , this->dimensions.width - 2 , entry.second , entry.first ) );
}