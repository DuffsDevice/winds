#include <_gadget/gadget.contextmenu.h>
#include <_gadget/gadget.contextmenu.entry.divider.h>
#include <_controller/controller.debug.h>

_contextMenu::_contextMenu( _optValue<_length> width , _menu* content , _gadget* owner , bool preserveValue , _int initialValue , _u16 mainList , _style&& style ) :
	_popup( width , ignore , owner )
	, preserveValue( preserveValue )
	, wasMenuAllocated( true )
	, mainList( mainList )
	, menu( content )
	, currentValue( initialValue )
{
	_gadget::setType( _gadgetType::contextmenu );
	_gadget::setPadding( _padding( 1 , 1 , 1 , 1 ) );
	
	// Register Update handler
	this->setInternalEventHandler( onUpdate , make_callback( &_contextMenu::updateHandler ) );
	
	// Generate _contextMenuEntry's
	this->updateNow();
	
	// Register Handlers
	this->setInternalEventHandler( onDraw , make_callback( &_contextMenu::refreshHandler ) );
	this->setInternalEventHandler( onOpen , make_callback( &_contextMenu::openHandler ) );
	this->setInternalEventHandler( onClose , make_callback( &_contextMenu::closeHandler ) );
	this->setInternalEventHandler( onKeyDown , make_callback( &_contextMenu::keyHandler ) );
	this->setInternalEventHandler( onKeyRepeat , make_callback( &_contextMenu::keyHandler ) );
	this->setUserEventHandler( onBlur , make_callback( &_contextMenu::blurHandler ) );
}

_contextMenuEntry* _contextMenu::getEntryFromValue( _u16 value ) const
{
	for( _gadget* gadget : this->getChildren( false ) ){
		_contextMenuEntry* entry = (_contextMenuEntry*)gadget;
		if( (_u16)entry->getIntValue() == value )
			return entry;
	}
	return nullptr;
}

_contextMenu::~_contextMenu(){
	this->removeChildren(true);
}

void _contextMenu::submit()
{
	_contextMenuEntry* child = (_contextMenuEntry*)this->getSelectedChild();
	
	if( !child ){
		this->shelve( true ); // Hide contextmenu
		return;
	}
	
	// something changed?
	if( child->getIntValue() != this->currentValue ){
		this->currentValue = child->getIntValue();
		
		if( !child->hasSubMenu() ){
			this->triggerEvent( onEdit ); // Inform that I changed my value
			this->menu->callHandler( this->mainList , child->getIntValue() );
		}
	}
	
	if( child->hasSubMenu() ){
		if( this->curOpenedSubMenu && this->curOpenedSubMenu->getListIndex() == child->getSubMenu() ){
			this->curOpenedSubMenu->shelve( true );
			this->curOpenedSubMenu = nullptr;
		}
		else{
			this->curOpenedSubMenu = new _contextMenu( this->getWidth() , this->menu , this , this->preserveValue , -1 , child->getSubMenu() );
			this->curOpenedSubMenu->setUserEventHandler( onClose , make_callback( this , &_contextMenu::subMenuCloseHandler ) );
			this->curOpenedSubMenu->show( child->getAbsoluteDimensions() ); // Show
		}
	}
	else{
		this->setOwner( nullptr );
		this->shelve( true ); // Hide contextmenu
	}
}

_callbackReturn _contextMenu::subMenuCloseHandler( _event event )
{
	if( !this->hasFocus() )
		this->shelve( this->getOwner() && this->getOwner()->getType() != _gadgetType::contextmenu );
	
	return use_internal;
}

_callbackReturn _contextMenu::blurHandler( _event event )
{
	// Fetch Gadget
	_contextMenu* that = event.getGadget<_contextMenu>();
	
	if( that->curOpenedSubMenu && that->curOpenedSubMenu->isOpened() )
		return handled;
	
	return use_internal;
}
_callbackReturn _contextMenu::refreshHandler( _event event )
{
	// Fetch Gadget
	_contextMenu* that = event.getGadget<_contextMenu>();
	
	// Get Port to draw to
	_bitmapPort bP = that->getBitmapPort( event );
	
	_length myW = bP.getWidth();
	_length myH = bP.getHeight();
	
	// Fill Bg
	bP.fill( _color::white );
	
	// Fill Border
	bP.drawRect( 0 , 0 , myW , myH , _color::fromRGB( 19 , 19 , 19 ) );
	
	//// Draw "Shadow"
	//bP.drawHorizontalLine( 1 , myH - 1 , myW - 1 , _color::fromRGB( 9 , 9 , 9 ) );
	//bP.drawVerticalLine( myW - 1 , 1 , myH - 1 , _color::fromRGB( 9 , 9 , 9 ) );
	//
	//// Erase corners
	//bP.drawPixel( myW - 1 , 0 , _color::transparent );
	//bP.drawPixel( 0 , myH - 1 , _color::transparent );
	
	return use_default;
}

_callbackReturn _contextMenu::keyHandler( _event event )
{
	// Fetch Gadget
	_contextMenu* that = event.getGadget<_contextMenu>();
	
	// Variable to determine the next selected entry
	_contextMenuEntry* next = (_contextMenuEntry*) that->getSelectedChild();
	
	if( event.getKeyCode() == _key::down )
	{
		if( next ){
			do
				next = (_contextMenuEntry*) next->getSubcedentChild();
			while( next && next->getStrValue() == _menu::divider );
			if( next ){
				next->select();
				return handled;
			}
		}
		// Start again from the beginning of the list
		next = (_contextMenuEntry*) that->getToppestChild( false );
		if( next ){
			while( next && next->getStrValue() == _menu::divider )
				next = (_contextMenuEntry*) next->getSubcedentChild();
			if( next )
				next->select();
		}
	}
	else if( event.getKeyCode() == _key::up )
	{
		if( next ){
			do
				next = (_contextMenuEntry*) next->getPrecedentChild();
			while( next && next->getStrValue() == _menu::divider );
			if( next ){
				next->select();
				return handled;
			}
		}
		// Start again from the end of the list
		next = (_contextMenuEntry*) that->getLowestChild( false );
		if( next ){
			while( next && next->getStrValue() == _menu::divider )
				next = (_contextMenuEntry*) next->getPrecedentChild();
			if( next )
				next->select();
		}
	}
	else if( event.getKeyCode() == _key::a ) //!@todo move KEY_A into rtA!
		that->submit();
	else if( event.getKeyCode() == _key::right && next && next->hasSubMenu() )
		that->submit();
	else if( event.getKeyCode() == _key::b || event.getKeyCode() == _key::left ) //!@todo move KEY_B into rtA!
		that->shelve( true );
	
	return handled;
}

_callbackReturn _contextMenu::closeHandler( _event event )
{
	// Fetch Gadget
	_contextMenu* that = event.getGadget<_contextMenu>();
	
	_contextMenuEntry* entry = (_contextMenuEntry*) that->getSelectedChild();
	
	// Restore active entry
	if( entry )
	{
		if( (_u16)entry->getIntValue() != that->currentValue && that->preserveValue ){
			_contextMenuEntry* initialEntry = that->getEntryFromValue( that->currentValue );
			if( initialEntry )
				initialEntry->select();
		}
		else
			entry->deselect();
	}
	
	if( that->curOpenedSubMenu ){
		that->curOpenedSubMenu->shelve( false );
		that->curOpenedSubMenu = nullptr;
	}
	
	return handled;
}

_callbackReturn _contextMenu::openHandler( _event event )
{
	// Fetch Gadget
	_contextMenu* that = event.getGadget<_contextMenu>();
	
	// Adjust width
	if( that->hasAutoWidth() )
	{
		_length w = 0;
		for( _gadget* entry : that->getChildren( false ) ){
			entry->requestAutoWidth();
			w = max( entry->getWidth() , w );
		}
		
		that->setWidthIfAuto( w + 2 );
		
		for( _gadget* entry : that->getChildren( false ) )
			entry->setWidth( w );
	}
	
	// Adjust height
	that->setHeight( that->getLowestChild( false )->getDimensions().getY2() + 3 );
	
	// Possibly unhighlight the old entry
	if( !that->preserveValue && that->getSelectedChild() )
		that->getSelectedChild()->deselect();
	
	return handled;
}

void _contextMenu::setIntValue( _u16 entryId )
{
	const _gadgetList& list = this->getChildren( false );
	
	_gadgetList::const_iterator iter = find_if(
		list.begin()
		, list.end()
		, [entryId]( _gadget* entry )->bool{
			return (_u16)((_contextMenuEntry*)entry)->getIntValue() == entryId;
		}
	);
	if( iter != this->getChildren( false ).end() )
		(*iter)->select();
}

_callbackReturn _contextMenu::updateHandler( _event event )
{
	_contextMenu* that = event.getGadget<_contextMenu>();
	
	that->removeChildren( true );
	
	// Determine new width for all entries
	_optValue<_length> val = that->hasAutoWidth() ? _optValue<_length>( ignore ) : _optValue<_length>( that->getWidth() - 2 );
	
	// Iterate through list
	for( const _pair<_u16,_menuEntry>& entry : that->getList() )
	{
		_contextMenuEntry* cM;
		
		if( entry.second.text != _menu::divider )
			cM = new _contextMenuEntry( _optValue<_length>(val) , entry.first , entry.second );
		else
			cM = new _contextMenuEntryDivider();
		
		cM->setInternalEventHandler( onParentAdd , _gadgetHelpers::moveBesidePrecedent( _direction::down , 0 , 0 , false , 0 , 0 ) );
		that->addChild( cM , true );
	}
	
	// Select initial value
	if( that->preserveValue )
	{
		_contextMenuEntry* entryToSelect = that->getEntryFromValue( that->currentValue );
		
		if( entryToSelect )
			entryToSelect->select(); // Focus this entry
	}
	
	return handled;
}