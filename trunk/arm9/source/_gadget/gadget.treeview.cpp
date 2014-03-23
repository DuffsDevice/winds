#include "_gadget/gadget.treeview.h"
#include "_type/type.gadget.helpers.h"
#include <math.h>

void _treeViewNode::addTo( _gadget* dest , _u32 curLevel )
{
	if( this->value )
	{
		// Register default dom tree position change handler
		this->value->setUserEventHandler( onEdit , make_callback( &_treeView::nodePreVisibilityHandler ) );
		
		// Register some handlers
		this->value->setInternalEventHandler( onMouseClick , make_callback( &_treeView::nodeClickHandler ) );
		this->value->setUserEventHandler( onKeyDown , make_callback( &_treeView::nodeClickHandler ) );
		this->value->setUserEventHandler( onKeyRepeat , make_callback( &_treeView::nodeClickHandler ) );
		
		//// Set Hierarchy Level
		this->value->setStyle( _style::storeInt( this->children.empty() ? -curLevel : curLevel , this->value->getStyle() ) );
		
		// Set X-Position
		this->value->setX( _treeView::getLevelX(curLevel) );
		
		// Y-Position will be auto-computed
		this->value->requestAutoY();
		
		// Add as a child
		dest->addChild( this->value , true );
		
		// Force right position
		this->value->handleEvent( onEdit );
		
		// We set value to nullptr to prevent it from being
		// destroyed when this _treeViewNode gets destructed
		// This prohibits memory leaks
		this->value = nullptr;
		
		// Add all other children
		for( _treeViewNode& child : this->children )
			child.addTo( dest , curLevel + 1 );
	}
}

_callbackReturn _treeView::refreshHandler( _event event )
{
	// Receive Gadget
	_treeView* that = event.getGadget<_treeView>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	bP.fill( _color::white );
	
	return use_default;
}

_gadget* _treeView::getNextVisibleOrHigherLevel( _gadget* current )
{
	_gadget* parent = current->getParent();
	
	if( parent )
	{
		// Level of the node in the tree
		_int initialLevel = abs( current->getStyle().val );
		
		// Get List of adjacent children
		const _gadgetList& ch = parent->getChildren( current->isEnhanced() );
		
		// Find current gadget
		auto it = find( ch.cbegin() , ch.cend() , current );
		
		// Ensure the current gadget is not the first one,
		// because then it would not have a precedent child
		if( it == ch.cend() )
			return nullptr;
		
		// Iterate upstairs and search for
		// a node with a level lower than our node
		while( ++it != ch.cend() )
		{
			if( (*it) && ( !(*it)->isHidden() || abs( (*it)->getStyle().val ) < initialLevel ) )
				return *it;
		}
	}
	return nullptr;
}

_gadget* _treeView::getRootOf( _gadget* gadget )
{
	if( !gadget )
		return nullptr;
	
	_gadget* parent = gadget->getParent();
	
	if( parent )
	{
		// Level of the node in the tree
		_int initialLevel = abs( gadget->getStyle().val );
		
		// Get List of adjacent children
		const _gadgetList& ch = parent->getChildren( gadget->isEnhanced() );
		
		// Find current gadget
		auto it = find( ch.crbegin() , ch.crend() , gadget );
		
		// Ensure the current gadget is not the first one,
		// because then it would not have a precedent child
		if( it == ch.crend() )
			return nullptr;
		
		// Iterate upstairs and search for
		// a node with a level lower than our node
		while( ++it != ch.crend() )
		{
			if( (*it) && abs( (*it)->getStyle().val ) < initialLevel )
				return *it;
		}
	}
	return nullptr;
}

bool _treeView::hasChildren( _gadget* root )
{
	if( !root )
		return false;
	
	// Get adjacent subcedent node
	_gadget* next = root->getSubcedentChild();
	
	return next && abs( next->getStyle().val ) > root->getStyle().val;
}

_callbackReturn _treeView::nodeAddRemoveHandler( _event event )
{
	_gadget* child = event.getGadgetParam();
	
	if( child )
	{
		if( event == onChildAdd )
		{
			// Hide it before we show it!
			child->hide();
			
			if( !child->getStyle().val ) // Append at my tree level (same X-Position as this node)
			{
				_gadget* pre = child->getPrecedentChild();
				_int preLevel = abs( pre->getStyle().val );
				
				child->setStyle( _style::storeInt( preLevel , child->getStyle() ) );
				child->setX( pre->getX() );
			}
			else // Set X-Coordinate to the X-Position of a node of the same level
				child->setX( _treeView::getLevelX( child->getStyle().val ) );
			
			// Request the child to auto-compute its y-coordinate
			child->requestAutoY();
			
			// Register default dom tree position change handler
			child->setUserEventHandler( onEdit , make_callback( &_treeView::nodePreVisibilityHandler ) );
			
			// Register some handlers
			child->setInternalEventHandler( onMouseClick , make_callback( &_treeView::nodeClickHandler ) );
			child->setUserEventHandler( onKeyDown , make_callback( &_treeView::nodeClickHandler ) );
			child->setUserEventHandler( onKeyRepeat , make_callback( &_treeView::nodeClickHandler ) );
			
			// Move to the right position
			child->triggerEvent( onEdit );
		}
		else if( event == onChildRemove )
			child->setStyle( _style::storeInt( 0 , child->getStyle() ) );
	}
	
	return use_internal;
}

_callbackReturn _treeView::nodePreVisibilityHandler( _event event )
{
	_gadget* that = event.getGadget();
	_gadget* root = _treeView::getRootOf( that );
	
	bool hide = root != nullptr && ( root->getStyle().val < 0 || root->isHidden() );
	
	// Flag that, if set to true will skip all children nodes and continue with the next visible node
	bool takeNextVisible = false;
	
	if( hide ){
		if( that->isHidden() )
			takeNextVisible = true;
		else
			that->hide();
	}
	else
	{
		that->show();
		
		// If shown, move it to the right position
		_gadget* preVisible = that->getPrecedentChild( true );
		
		if( preVisible )
			that->setYIfAuto( preVisible->getY() + preVisible->getHeight() + 1 );
		else
			that->setYIfAuto( 1 );
	}
	
	_gadget* next;
	if( takeNextVisible )
		next = _treeView::getNextVisibleOrHigherLevel( that );
	else
		next = that->getSubcedentChild();
	
	if( next )
		next->triggerEvent( onEdit );

	return use_default;
}

_callbackReturn _treeView::nodeClickHandler( _event event )
{
	_gadget* that = event.getGadget();
	
	_int opened = that->getStyle().val;
	_int oldOpened = opened;
	
	// Key was Pressed!
	if( event == onKeyDown || event == onKeyRepeat )
	{
		// Expand Node!
		if( event.getKeyCode() == _key::right )
		{
			if( opened < 0 )
				opened = -opened;
			else{
				_gadget* first = that->getSubcedentChild();
				if( first && first->getX() > that->getX() ) // Ensure the current node has children
					first->focus();
			}
		}
		// Collapse Node!
		else if( event.getKeyCode() == _key::left )
		{
			if( opened > 0 )
				opened = -opened;
			else{
				_gadget* root = _treeView::getRootOf( that );
				if( root )
					root->focus();
			}
		}
		else
			return use_internal;
	}
	else
		opened = -opened;
	
	if( oldOpened != opened )
	{
		// Adjust my state
		that->setStyle( _style::storeInt( opened , that->getStyle() ) );
		
		if( opened > 0 )
			that->handleEvent( onOpen );
		else
			that->handleEvent( onClose );
		
		// Notify subceding children
		_gadget* next = that->getSubcedentChild();
		if( next )
			next->handleEvent( onEdit );
	}
	
	return handled;
}

void _treeView::setTree( _vector<_treeViewNode> treeStructure )
{
	// Remove Handler
	this->removeUserEventHandler( onChildAdd );
	this->removeUserEventHandler( onChildRemove );
	
	// Remove All nodes
	this->removeChildren( true );
	
	// Add all children nodes
	for( _treeViewNode& child : treeStructure )
		child.addTo( this );
	
	// Add Handler
	this->setUserEventHandler( onChildRemove , make_callback( &_treeView::nodeAddRemoveHandler ) );
	this->setUserEventHandler( onChildAdd , make_callback( &_treeView::nodeAddRemoveHandler ) );
}

_treeView::_treeView(
		_optValue<_coord> x , _optValue<_coord> y
		, _optValue<_length> width , _optValue<_length> height
		, _vector<_treeViewNode> treeStructure
		, _scrollType scrollTypeX , _scrollType scrollTypeY
		, _style&& style
	) :
	_scrollArea( x , y , width , height , scrollTypeX , scrollTypeY , move(style) )
{
	// Set Type of gadget
	this->setType( _gadgetType::treeview );
	
	// Add all children nodes
	for( _treeViewNode& child : treeStructure )
		child.addTo( this );
	
	// Add Handler
	this->setUserEventHandler( onChildAdd , make_callback( &_treeView::nodeAddRemoveHandler ) );
	this->setUserEventHandler( onChildRemove , make_callback( &_treeView::nodeAddRemoveHandler ) );
}