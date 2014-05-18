#include "_gadget/gadget.contextmenu.h"
#include "_gadget/gadget.contextmenu.entry.divider.h"

_contextMenu::_contextMenu( _optValue<_length> width , const _menuEntryList& list , _gadget* owner , bool preserveValue , _int initialValue , _style&& style ) :
	_popup( width , ignore , owner )
	, selectedEntry( nullptr )
	, activeEntry( nullptr )
	, preserveValue( preserveValue )
{
	_gadget::setType( _gadgetType::contextmenu );
	_gadget::setPadding( _padding( 1 , 1 , 1 , 1 ) );
	
	// Register Handlers
	this->setInternalEventHandler( onDraw , make_callback( &_contextMenu::refreshHandler ) );
	this->setInternalEventHandler( onOpen , make_callback( &_contextMenu::openHandler ) );
	this->setInternalEventHandler( onKeyDown , make_callback( &_contextMenu::keyHandler ) );
	this->setInternalEventHandler( onKeyRepeat , make_callback( &_contextMenu::keyHandler ) );
	
	// Generate _contextMenuEntry's
	this->generateChildren( list );
	
	// Select initial value
	if( preserveValue )
	{
		const _gadgetList& children = this->getChildren( false );
		
		_gadgetList::const_iterator entry = find_if(
			children.cbegin()
			, children.cend()
			, [initialValue]( _gadget* entry )->bool{
				return ((_contextMenuEntry*)entry)->getIntValue() == initialValue;
			}
		);
		
		if( entry == children.cend() || !*entry )
			return;
		
		// Write new entry in attribute
		this->activeEntry = this->selectedEntry = (_contextMenuEntry*)(*entry);
		
		// Redraw the new one since it should be highlighted
		this->activeEntry->redraw();
	}
}

_contextMenu::~_contextMenu(){
	this->removeChildren(true);
}

void _contextMenu::selectEntry( _contextMenuEntry* gadget )
{
	// something changed?
	if( gadget != this->selectedEntry )
	{
		// Apply value
		this->selectedEntry = gadget;
		
		// Inform that something changed
		this->triggerEvent( onEdit );
	}
	
	// Hide contextmenu
	this->shelve( true );
}

void _contextMenu::highlightEntry( _contextMenuEntry* gadget )
{
	// Check if that entry isn't already highlighted
	if( this->activeEntry != gadget )
	{
		// Store previous _contextMenuEntry in temporary variable
		_gadget* temp = this->activeEntry;
		
		// Write new entry in attribute
		this->activeEntry = gadget;
		
		// Refresh both the old and the new entry
		if( temp )
			temp->redraw();
		if( this->activeEntry )
			this->activeEntry->redraw();
	}
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
	
	if( event.getKeyCode() == _key::down )
	{
		_contextMenuEntry* next = that->activeEntry;
		if( next ){
			do
				next = (_contextMenuEntry*) next->getSubcedentChild();
			while( next && next->getStrValue() == "----" );
			if( next ){
				that->highlightEntry( next );
				return handled;
			}
		}
		next = (_contextMenuEntry*) that->getToppestChild( false );
		if( next ){
			while( next && next->getStrValue() == "----" )
				next = (_contextMenuEntry*) next->getSubcedentChild();
			if( next )
				that->highlightEntry( next );
		}
	}
	else if( event.getKeyCode() == _key::up )
	{
		_contextMenuEntry* next = that->activeEntry;
		if( next ){
			do
				next = (_contextMenuEntry*) next->getPrecedentChild();
			while( next && next->getStrValue() == "----" );
			if( next ){
				that->highlightEntry( next );
				return handled;
			}
		}
		next = (_contextMenuEntry*) that->getLowestChild( false );
		if( next ){
			while( next && next->getStrValue() == "----" )
				next = (_contextMenuEntry*) next->getPrecedentChild();
			if( next )
				that->highlightEntry( next );
		}
	}
	else if( event.getKeyCode() == _key::a ) //!@todo move KEY_A into rtA!
		that->selectEntry( that->activeEntry );
	else if( event.getKeyCode() == _key::b ) //!@todo move KEY_B into rtA!
		that->shelve( true );
	
	return handled;
}

_callbackReturn _contextMenu::closeHandler( _event event )
{
	// Fetch Gadget
	_contextMenu* that = event.getGadget<_contextMenu>();
	
	// Restore active entry
	if( that->activeEntry != that->selectedEntry && that->preserveValue )
	{
		_contextMenuEntry* entry = that->activeEntry;
		that->activeEntry = that->selectedEntry;
		
		entry->redraw();
		that->activeEntry->redraw();
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
		for( _gadget* entry : that->getChildren( false ) )
			w = max( entry->getWidth() , w );
		
		that->setWidthIfAuto( w + 2 );
	}
	
	// Adjust height
	that->setHeight( that->getLowestChild( false )->getDimensions().getY2() + 3 );
	
	// Possibly unhighlight the old entry
	if( !that->preserveValue )
	{
		that->selectedEntry = nullptr;
		that->highlightEntry( nullptr );
	}
	else
		that->highlightEntry( that->selectedEntry );
	
	return handled;
}

void _contextMenu::setIntValue( _int id ){
	const _gadgetList& list = this->getChildren( false );
	
	_gadgetList::const_iterator entry = find_if(
		list.begin()
		, list.end()
		, [id]( _gadget* entry )->bool{
			return ((_contextMenuEntry*)entry)->getIntValue() == id;
		}
	);
	if( entry != this->getChildren( false ).end() )
		this->selectEntry( ((_contextMenuEntry*)*entry) );
}

void _contextMenu::generateChildren( const _menuEntryList& list )
{
	this->removeChildren( true );
	
	_optValue<_length> val = this->hasAutoWidth() ? _optValue<_length>( ignore ) : _optValue<_length>( this->getWidth() - 2 );
	for( const _pair<_int,string>& entry : list )
	{
		_contextMenuEntry* cM;
		
		if( entry.second != "----" )
			cM = new _contextMenuEntry( _optValue<_length>(val) , entry.first , entry.second );
		else
			cM = new _contextMenuEntryDivider();
		
		cM->setInternalEventHandler( onParentAdd , _gadgetHelpers::moveBesidePrecedent( _direction::down , 0 , 0 , false , 0 , 0 ) );
		this->addChild( cM , true );
	}
}

_menuEntryList _contextMenu::getList()
{
	_menuEntryList list;
	
	for( _gadget* g : this->getChildren( false ) )
	{
		_contextMenuEntry* entry = ((_contextMenuEntry*)g);
		list.insert( make_pair(entry->getIntValue(), entry->getStrValue()) );
	}
	
	return list;
}