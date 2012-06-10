#include "_gadget/gadget.select.h"
#include "_gadget/gadget.button.h"
#include "_graphic/BMP_ScrollButtons.h"

BMP_ScrollButtonBottom* graphic = new BMP_ScrollButtonBottom();

_gadgetEventReturnType _select::mouseHandler( _gadgetEvent event )
{
	// Receive Gadget
	_select* that = (_select*)event.getGadget();
	
	that->popDownList->toggle();
	
	return handled;
}

_gadgetEventReturnType _select::dialogCloseHandler( _gadgetEvent event )
{
	// Receive Gadget
	_select* that = (_select*)event.getGadget();
	
	_s32 val = event.getArgs().getIntValue();
	
	if( val != -1 ){
		that->label->setStrValue( that->entries[ val ] );
		that->selected = val;
	}
	
	return handled;
}

_gadgetEventReturnType _select::refreshHandler( _gadgetEvent event ){
	
	// Receive Gadget
	_select* that = (_select*)event.getGadget();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.getArgs().hasClippingRects() )
		bP.addClippingRects( event.getArgs().getDamagedRects().toRelative( that->getAbsoluteDimensions() ) );
	else
		bP.resetClippingRects();
	
	_length myH = bP.getHeight();
	_length myW = bP.getWidth();
	
	// Bg...
	bP.drawFilledRect( 1 , 1 , myW - 2 , myH - 2 , RGB( 31 , 31 , 31 ) );
	
	// Little Button on the right side
	bP.copy( myW - 9 , 1 , graphic );
	
	// Border
	bP.drawRect( 0 , 0 , myW , myH , RGB( 20 , 20 , 20 ) );
	
	return use_default;
}

_select::_select( _coord x , _coord y , _length width , _contextMenuEntryList lst , _gadgetStyle style ) :
	_gadget( _gadgetType::selectbox , width , _system_->_runtimeAttributes_->selectObjectHeight + 2 , x , y , style )
	, entries( lst )
	, label( new _label( width , _system_->_runtimeAttributes_->selectObjectHeight , 1 , 0 , lst[0] ) )
	, selected( 0 )
{
	this->popDownList = new _selectPopDown( this , &this->entries );
	// Set Padding
	this->setPadding( _padding( 1 , 1 , 10 , 1 ) );
	
	// Add the Label
	this->addChild( this->label );
	this->label->setVAlign( _valign::middle );
	
	// Register Event - Handlers
	this->registerEventHandler( "refresh" , &_select::refreshHandler );
	this->registerEventHandler( "mouseClick" , &_select::mouseHandler );
	this->registerEventHandler( "dialogClose" , &_select::dialogCloseHandler );
	
	// Refresh Me
	this->refreshBitmap();
}

_select::~_select(){
	delete this->popDownList;
}

_contextMenuEntryList& _select::getList(){
	return this->entries;
}

void _select::setList( _contextMenuEntryList lst ){
	this->entries = lst;
	this->popDownList->refreshChildren();
}

void _select::setList( _contextMenuEntryList& lst ){
	this->entries = lst;
	this->popDownList->refreshChildren();
}

string	_select::getStrValue(){
	return this->entries[this->selected];
}
_s32	_select::getIntValue(){
	return this->selected;
}

void	_select::setIntValue( int value ){
	this->selected = value;
	this->label->setStrValue( this->entries[value] );
	this->popDownList->refreshChildren();
}

void	_select::refreshList(){
	this->popDownList->refreshChildren();
}


/////////////////////////////////////////
//           popDownList               //
/////////////////////////////////////////

_gadgetEventReturnType _selectPopDown::refreshHandler( _gadgetEvent event )
{
	// Receive Gadget
	_selectPopDown* that = (_selectPopDown*)event.getGadget();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.getArgs().hasClippingRects() )
		bP.addClippingRects( event.getArgs().getDamagedRects().toRelative( that->getAbsoluteDimensions() ) );
	else
		bP.resetClippingRects();
	
	_length myH = bP.getHeight();
	_length myW = bP.getWidth();
	
	// Bg...
	bP.drawFilledRect( 1 , 1 , myW - 2 , myH - 2 , RGB( 31 , 31 , 31 ) );
	
	// Border
	bP.drawRect( 0 , 0 , myW , myH , RGB( 5 , 5 , 5 ) );
	
	return use_default;
}

_gadgetEventReturnType _selectPopDown::clickHandler( _gadgetEvent event )
{
	_label* that = (_label*)event.getGadget();
	
	if( event.getType() == "mouseClick" )
	{
		((_contextMenu*)that->getParent())->closeAndSend( that->getIntValue() , that->getStrValue() );
		return handled;
	}
	else if( event.getType() == "focus" )
	{
		that->setBgColor( RGB255( 10 , 36 , 106 ) );
		that->setColor( COLOR_WHITE );
	}
	else
	{
		that->setBgColor( COLOR_WHITE );
		that->setColor( COLOR_BLACK );
	}
	
	return use_default;
}

void _selectPopDown::refreshChildren()
{
	// Temp...
	_gadget* owner = this->getOwner();
	_gadget* g = nullptr;
	
	// Assert
	if( !owner || !this->entries )
		return;
	
	_rect absDim = owner->getAbsoluteDimensions();
	
	this->setHeight( this->entries->size() * _system_->_runtimeAttributes_->selectObjectHeight + 2 );
	
	while( ( g = this->getToppestChild() ) != nullptr )
	{
		this->removeChild( g );
		delete g;
	}
	
	int y = 0;
	
	for( pair<const _s32,string>& entry : *this->entries )
	{
		_label* l = new _label( absDim.getWidth() - 2 , _system_->_runtimeAttributes_->selectObjectHeight , 0 , y , entry.second );
		l->setVAlign( _valign::middle );
		l->setIntValue( entry.first );
		l->registerEventHandler( "mouseClick" , &_selectPopDown::clickHandler );
		l->registerEventHandler( "blur" , &_selectPopDown::clickHandler );
		l->registerEventHandler( "focus" , &_selectPopDown::clickHandler );
		this->addChild( l );
		y += _system_->_runtimeAttributes_->selectObjectHeight;
	}
}

void _selectPopDown::toggle()
{
	
	// Temp...
	_gadget* owner = this->getOwner();
	
	// Assert
	if( !owner )
		return;
	
	_rect absDim = owner->getAbsoluteDimensions();
	
	this->setWidth( absDim.getWidth() );
	
	this->refreshBitmap();
	
	_contextMenu::toggle( absDim.getX() , absDim.getY2() + 1 );
}

_selectPopDown::_selectPopDown( _gadget* owner , _contextMenuEntryList* lst ) :
	_contextMenu( 1 , 2 , owner )
	, entries( lst )
{
	this->setPadding( _padding( 1 , 1 , 1 , 1 ) );
	this->registerEventHandler( "refresh" , &_selectPopDown::refreshHandler );
	
	this->refreshChildren();
	
	// Refresh Me
	this->refreshBitmap();
}