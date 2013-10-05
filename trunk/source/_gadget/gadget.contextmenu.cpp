#include "_gadget/gadget.contextmenu.h"
#include "_type/type.system.h"

_contextMenu::_contextMenu( const _contextMenuList& list , _gadget* owner , _optValue<_length> width , bool preserveValue , _s32 initialValue , _style&& style ) :
	_popup( move(width) , _system::getUser()->fOH * list.size() + 2 , owner )
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
		_gadgetList::iterator entry = find_if(
			this->children.begin()
			, this->children.end()
			, [initialValue]( _gadget* entry )->bool{
				return ((_contextMenuEntry*)entry)->getIntValue() == initialValue;
			}
		);
		
		// Write new entry in attribute
		this->activeEntry = this->selectedEntry = (_contextMenuEntry*)(*entry);
		
		// Redraw the new one since it should be highlighted
		if( this->activeEntry )
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
	
	// Fill Bg
	bP.fill( COLOR_WHITE );
	
	// Fill Border
	bP.drawRect( 0 , 0 , that->getWidth() , that->getHeight() , RGB( 19 , 19 , 19 ) );
	
	return use_default;
}

_callbackReturn _contextMenu::keyHandler( _event event )
{
	// Fetch Gadget
	_contextMenu* that = event.getGadget<_contextMenu>();
	
	if( event.getKeyCode() == DSWindows::KEY_DOWN )
	{
		_contextMenuEntry* next = (_contextMenuEntry*)( that->activeEntry ? that->activeEntry->getSubcedentChild() : nullptr );
		
		if( next )
			that->highlightEntry( next );
		else if( that->getToppestChild() )
			that->highlightEntry( (_contextMenuEntry*)that->getToppestChild() );
	}
	else if( event.getKeyCode() == DSWindows::KEY_UP )
	{
		_contextMenuEntry* prev = (_contextMenuEntry*)( that->activeEntry ? that->activeEntry->getPrecedentChild() : nullptr );
		
		if( prev )
			that->highlightEntry( prev );
		else if( that->getLowestChild() )
			that->highlightEntry( (_contextMenuEntry*)that->getLowestChild() );
	}
	else if( event.getKeyCode() == DSWindows::KEY_A ) //!@todo move KEY_A into rtA!
		that->selectEntry( that->activeEntry );
	else if( event.getKeyCode() == DSWindows::KEY_B ) //!@todo move KEY_B into rtA!
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
		for( _gadget* entry : that->children )
			w = max( entry->getWidth() , w );
		
		that->setWidthIfAuto( w + 2 );
	}
	
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

void _contextMenu::setIntValue( _s32 id ){
	_gadgetList::iterator entry = find_if(
		this->children.begin()
		, this->children.end()
		, [id]( _gadget* entry )->bool{
			return ((_contextMenuEntry*)entry)->getIntValue() == id;
		}
	);
	if( entry != this->children.end() )
		this->selectEntry( ((_contextMenuEntry*)*entry) );
}

void _contextMenu::generateChildren( const _contextMenuList& list )
{
	this->removeChildren( true );
	
	_optValue<_length> val = this->hasAutoWidth() ? _optValue<_length>( ignore ) : _optValue<_length>( this->getWidth() - 2 );
	for( const _pair<_s32,string>& entry : list )
	{
		_contextMenuEntry* cM = new _contextMenuEntry( entry.first , entry.second , _optValue<_length>( val ) );
		cM->setInternalEventHandler( onParentSet , _gadgetHelpers::moveBesidePrecedent( _dimension::vertical , 0 , 0 , false , 0 , 0 ) );
		this->addChild( cM , true );
	}
}

_contextMenuList _contextMenu::getList()
{
	_contextMenuList list;
	
	for( _gadget* g : this->children )
	{
		_contextMenuEntry* entry = ((_contextMenuEntry*)g);
		list[entry->getIntValue()] = entry->getStrValue();
	}
	
	return list;
}