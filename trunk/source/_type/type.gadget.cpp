#include "_type/type.gadget.h"
#include "_type/type.system.h"
#include <nds.h>

_gadget::_gadget( _gadgetType type , int width , int height , int posX , int posY , _gadgetStyle style , bool doNotAllocateBitmap )
	: type( type ) , padding( _padding( 0 ) ) , dimensions( _rect( posX , posY , max( 1 , width ) , max( 1 , height ) ) ) , enhanced( false ) , focused( false ) , style( style ) , parent( nullptr ) , dragTemp ( nullptr )
{
	if( !doNotAllocateBitmap )
		this->bitmap = new _bitmap( max( 1 , width ) , max( 1 , height ) );
	
	this->registerDefaultEventHandler( focus , &_gadget::gadgetFocusHandler );
	this->registerDefaultEventHandler( blur , &_gadget::gadgetFocusHandler );
	this->registerDefaultEventHandler( mouseDown , &_gadget::gadgetMouseHandler );
	this->registerDefaultEventHandler( mouseUp , &_gadget::gadgetMouseHandler );
	this->registerDefaultEventHandler( mouseClick , &_gadget::gadgetMouseHandler );
	this->registerEventHandler( mouseDoubleClick , &_gadget::gadgetMouseHandler );
	this->registerDefaultEventHandler( refresh , &_gadget::gadgetRefreshHandler );
	this->registerDefaultEventHandler( dragStart , &_gadget::gadgetDragHandler );
	this->registerDefaultEventHandler( dragging , &_gadget::gadgetDragHandler );
	this->registerDefaultEventHandler( dragStop , &_gadget::gadgetDragHandler );
}

_gadget::_gadget( int width , int height , int posX , int posY , _gadgetStyle style , bool doNotAllocateBitmap )
	: type( _plain ) , padding( _padding( 0 ) ) , dimensions( _rect( posX , posY , max( 1 , width ) , max( 1 , height ) ) ) , enhanced( false ) , focused( false ) , style( style ) , parent( nullptr ) , dragTemp ( nullptr )
{
	if( !doNotAllocateBitmap )
		this->bitmap = new _bitmap( max( 1 , width ) , max( 1 , height ) );
	
	this->bitmap->reset( RGB( 31 , 0 , 0 ) );
	
	this->registerDefaultEventHandler( focus , &_gadget::gadgetFocusHandler );
	this->registerDefaultEventHandler( blur , &_gadget::gadgetFocusHandler );
	this->registerDefaultEventHandler( mouseDown , &_gadget::gadgetMouseHandler );
	this->registerDefaultEventHandler( mouseUp , &_gadget::gadgetMouseHandler );
	this->registerDefaultEventHandler( mouseClick , &_gadget::gadgetMouseHandler );
	this->registerEventHandler( mouseDoubleClick , &_gadget::gadgetMouseHandler );
	this->registerDefaultEventHandler( refresh , &_gadget::gadgetRefreshHandler );
	this->registerDefaultEventHandler( dragStart , &_gadget::gadgetDragHandler );
	this->registerDefaultEventHandler( dragging , &_gadget::gadgetDragHandler );
	this->registerDefaultEventHandler( dragStop , &_gadget::gadgetDragHandler );
}

void _gadget::setPadding( _padding p ){
	this->padding = p;
}

_padding _gadget::getPadding(){ 
	return this->padding;
}

bool _gadget::isEnhanced() const { 
	return this->enhanced;
}

void _gadget::setEnhanced( bool flag ){
	this->enhanced = flag;
}

_gadget::~_gadget()
{
	if( this->parent != nullptr )
		this->parent->removeChild( this );
	delete this->bitmap;
}

_gadgetType _gadget::getType(){ return this->type; }

void _gadget::triggerEvent( _gadgetEvent event )
{
	event.getArgs().setDestination( this );
	_system_->generateEvent( event );
}

void _gadget::bubbleEvent( _gadgetEvent e , bool includeThis )
{
	if( includeThis )
		this->triggerEvent( e );
	if( this->parent != nullptr )
		this->parent->bubbleEvent( e , true );
}

void _gadget::refreshBitmap()
{
	_gadgetEvent event = _gadgetEvent( this , refresh , _gadgetEventArgs() );
	event.getArgs().preventBubble( true );
	this->handleEvent( event );
}

_rect _gadget::getSize() const {
	return _rect( 0 , 0 , this->dimensions.width , this->dimensions.height );
}

void _gadget::generateEvent( _gadgetEvent event )
{
	_system_->generateEvent( event );
}

void _gadget::bubbleRefresh( bool includeThis , _gadgetEvent e )
{
	// Event not generated yet
	if( !e.getArgs().isBubblePrevented() )
		e = _gadgetEvent( this , refresh , this->getAbsoluteDimensions() );
	
	if( includeThis )
		this->handleEvent( e );
	
	// Bubble!
	if( this->parent != nullptr )
	{
		_rect size = this->parent->getAbsoluteDimensions();
		if( size.intersectsWith( e.getArgs().getDamagedRects() ) )
			this->parent->bubbleRefresh( true , e );
	}
}

_bitmapPort _gadget::getBitmapPort() const {
	return this->bitmap;
}

_bitmap* _gadget::getBitmap(){
	return this->bitmap;
}

_gadgetStyle _gadget::getStyle() const {
	return this->style;
}

void _gadget::setStyle( _gadgetStyle style ){
	this->style = style;
	this->triggerEvent( refresh );
}

_gadget* _gadget::getWindows(){
	if( this->type == windows )
		return this;
	else if( this->parent != nullptr )
		return this->parent->getWindows();
	else
		return nullptr;
}

bool _gadget::registerEventHandler( _gadgetEventType type , _gadgetEventHandler handler ){
	// Insert The Handler no matter if there was already one acting to that eventType
	this->eventHandlers[type] = handler;
	return true;
}

bool _gadget::registerLuaEventHandler( _gadgetEventType type , int handler , lua_State* L ){
	// Insert The Handler no matter if there was already one acting to that eventType
	this->luaEventHandlers[type] = make_pair( handler , L );
	return true;
}

bool _gadget::registerDefaultEventHandler( _gadgetEventType type , _gadgetDefaultEventHandler handler ){
	// Insert The Handler no matter if there was already one acting to that eventType
	this->defaultEventHandlers[type] = handler;
	return true;
}

void _gadget::unregisterEventHandler( _gadgetEventType type ){
	// Unbind the Handler
	this->eventHandlers.erase( type );
}

int _gadget::unregisterLuaEventHandler( _gadgetEventType type ){
	
	if( this->luaEventHandlers.count( type ) == 0 )
		return LUA_NOREF;
	
	int d = this->luaEventHandlers[ type ].first;
	
	// Unbind the Handler
	this->luaEventHandlers.erase( type );
	
	return d;
}

bool _gadget::blurEventChild()
{
	_gadgetList::iterator child = find_if( this->children.begin() , this->children.end() , [](_gadget* g)->bool{return g->hasFocus();} );
	
	if( child == this->children.end() || !this->children.size() )
		return true;
	
	return !(*child)->focused || (*child)->handleEvent( blur ) == _gadgetEventReturnType::handled;
}

bool _gadget::focusEventChild( _gadget* child ){ if( !child ) return false; return child->focused || child->handleEvent( focus ) == _gadgetEventReturnType::handled; }

bool _gadget::blurChild()
{
	_gadgetList::iterator child = find_if( this->children.begin() , this->children.end() , [](_gadget* g)->bool{return g->hasFocus();} );
	
	if( child == this->children.end() || !this->children.size() )
		return true;
	
	(*child)->focused = false;
	_system::_currentFocus_ = nullptr;
	return true;
}

bool _gadget::focusChild( _gadget* child )
{
	if( !child )
		return false;
	
	_gadgetList::iterator itTemp = find( this->children.begin() , this->children.end() , child );
	
	// Blur the Previously focused gadget
	if( !child->focused )
		this->blurEventChild();
	else
		return false;
	
	_system::_currentFocus_ = child;
	child->focused = true;
	
	if( *itTemp == this->children.back() )
		return true;
	
	this->children.splice( this->children.end(), this->children , itTemp );
	
	// Refresh the Gadget
	child->bubbleRefresh();
	
	return true;
}

bool _gadget::canReactTo( _gadgetEventType type ) const {
	return this->eventHandlers.count( type ) || this->luaEventHandlers.count( type );
}

_gadgetEventReturnType _gadget::handleEventDefault( _gadgetEvent event )
{
	// Use the default EventHandler if available
	if( this->defaultEventHandlers.count( event.getType() ) )
		return this->defaultEventHandlers[ event.getType() ]( event );
	// If the Handler doesn't exist, return 
	return not_handled;
}

_gadgetEventReturnType _gadget::handleEvent( _gadgetEvent event )
{
	event.setGadget( this );
	pair<int,lua_State*> data;
	_gadgetEventReturnType ret = not_handled;
	
	// Check for Normal Event Handlers
	if( this->eventHandlers.count( event.getType() ) )
	{
		ret = this->eventHandlers[ event.getType() ]( event );
		if( ret == use_default )
			return this->handleEventDefault( event ); // Use the default EventHandler if available
		return ret;
	}
	// Check for a Handler by lua
	else if( this->luaEventHandlers.count( event.getType() ) )
	{
		ret = lua_callEventHandler( (data = this->luaEventHandlers[ event.getType() ]).second , data.first , event );
		if( ret == use_default )
			return this->handleEventDefault( event ); // Use the default EventHandler if available
		return ret;
	}
	// Use the default EventHandler if available
	return this->handleEventDefault( event );
}

_coord _gadget::getAbsoluteX() const {
	if( this->parent != nullptr ){
		return this->parent->getAbsoluteX() + this->dimensions.x;
	}
	return this->dimensions.x;
}

_coord _gadget::getAbsoluteY() const {
	if( this->parent != nullptr ){
		return this->parent->getAbsoluteY() + this->dimensions.y;
	}
	return this->dimensions.y;
}

_coord _gadget::getX() const {
	return this->dimensions.x;
}

_coord _gadget::getY() const {
	return this->dimensions.y;
}

void _gadget::setX( _coord val ){
	_rect dim = this->getAbsoluteDimensions();
	this->dimensions.x = val;
	this->bubbleRefresh( false , _gadgetEvent( this , refresh , this->getAbsoluteDimensions() | dim ) );
}

void _gadget::setY( _coord val ){
	_rect dim = this->getAbsoluteDimensions();
	this->dimensions.y = val;
	this->bubbleRefresh( false , _gadgetEvent( this , refresh , this->getAbsoluteDimensions() | dim ) );
}

void _gadget::moveRelative( _s16 dX , _s16 dY ){
	_rect dim = this->getAbsoluteDimensions();
	this->dimensions.x += dX;
	this->dimensions.y += dY;
	this->bubbleRefresh( false , _gadgetEvent( this , refresh , this->getAbsoluteDimensions() | dim ) );
}

void _gadget::moveTo( _coord x , _coord y )
{
	_rect dim = this->getAbsoluteDimensions();
	this->dimensions.x = x;
	this->dimensions.y = y;
	this->bubbleRefresh( false , _gadgetEvent( this , refresh , this->getAbsoluteDimensions() | dim ) );
}

bool& _gadget::hasFocus(){ return this->focused; }

_gadget* _gadget::getParent() const {
	return this->parent;
}

void _gadget::setParent( _gadget* val ){
	if( this->parent != nullptr )
		this->parent->removeChild( this );
	this->parent = val;
	if( val )
		this->parent->addChild( this );
}

void _gadget::_setParent( _gadget* val ){ this->parent = val; }

bool _gadget::removeEnhanced( _gadget* g ) { if( g->enhanced ) return false; g->parent = nullptr; g->focused = false; return true; }

void _gadget::removeChildren( bool preserveEnhanced )
{
	if( preserveEnhanced )
		this->children.remove_if( this->removeEnhanced  );
	else
		this->children.erase( this->children.begin(), this->children.end() );
	
	// Signalize there are no children left!
	this->bubbleRefresh( true );
}

void _gadget::removeChild( _gadget* child )
{
	if( !child )
		return;
	// Find it!
	_gadgetList::iterator it = find( this->children.begin() , this->children.end() , child );
	
	// Found?
	if( it == this->children.end() )
		return;
		
	// Erase it on my bitmap
	_rect absDim = child->getAbsoluteDimensions();
	
	this->children.erase( it );
	
	this->bubbleRefresh( true , _gadgetEvent( this , refresh , absDim ) );
	
	// Erase the Connection on both sides
	child->parent = nullptr;
	child->focused = false;
}

void _gadget::addChild( _gadget* child )
{
	if( !child )
		return;
	
	// Add it!
	this->children.push_back( child );
	
	child->parent = this;
	
	//! Paint it on my bmp
	child->bubbleRefresh();
}

_rect _gadget::getDimensions() const {
	return this->dimensions;
}

_rect _gadget::getAbsoluteDimensions() const { 
	_rect out = this->dimensions;
	out.x = this->getAbsoluteX();
	out.y = this->getAbsoluteY();
	return out;
}

void _gadget::setDimensions( _rect rc ){
	_rect dim = this->getAbsoluteDimensions();
	this->dimensions = rc;
	this->bitmap->resize( rc.width , rc.height );
	this->onResize();
	
	// Delete the parts that originally were gadget, but became damaged
	this->bubbleEvent( _gadgetEvent( this , refresh , dim - this->getAbsoluteDimensions() ) );
}

_length _gadget::getHeight() const {
	return this->dimensions.height;
}

_length _gadget::getWidth() const {
	return this->dimensions.width;
}

void _gadget::setHeight( _length val )
{
	if( val != this->dimensions.width ){
		_rect dim = this->getAbsoluteDimensions();
		this->dimensions.height = val;
		this->bitmap->setHeight( val );
		this->onResize();
		
		// Delete the parts that originally were gadget, but became damaged
		this->bubbleEvent( _gadgetEvent( this , refresh , dim - this->getAbsoluteDimensions() ) );
	}
}

void _gadget::setWidth( _length val )
{
	if( val != this->dimensions.width ){
		_rect dim = this->getAbsoluteDimensions();
		this->dimensions.width = val;
		this->bitmap->setWidth( val );
		this->onResize();
		
		// Delete the parts that originally were gadget, but became damaged
		this->bubbleEvent( _gadgetEvent( this , refresh , dim - this->getAbsoluteDimensions() ) );
	}
}

_gadgetEventReturnType _gadget::gadgetRefreshHandler( _gadgetEvent event )
{
	_gadget* that = event.getGadget();
	
	// If this Refresh-Event wasn't auto-generated, refresh my parents
	if( !event.getArgs().isBubblePrevented() )
		that->bubbleRefresh();
	
	// Receive Bitmap-Port
	_bitmapPort bP = that->getBitmapPort();
	
	// Temp...
	_gadget* gadget;
	_gadgetList::reverse_iterator it;
	
	// Receive Areas
	_area damagedRects;
	_area damagedEnhancedRects;
	_rect areaAvailable = _rect( 0 , 0 , that->dimensions.width , that->dimensions.height ) - that->getPadding();
	
	if( event.getArgs().hasClippingRects() ){
		damagedRects = event.getArgs().getDamagedRects().toRelative( that->getAbsoluteDimensions() );
		damagedEnhancedRects = damagedRects;
		damagedRects &= areaAvailable;
	}
	else{
		damagedRects.push_back( areaAvailable );
		damagedEnhancedRects = _rect( 0 , 0 , that->dimensions.width , that->dimensions.height );
	}
	
	for( it = that->children.rbegin() ; it != that->children.rend() ; it++ )
	{
		// Reset clipping Rects
		bP.deleteClippingRects();
		
		// Receive target
		gadget = (*it);
		
		// Has the Gadget special Privilegs e.g. it can draw on the Parents reserved areas?
		if( gadget->isEnhanced() )
		{
			// Special Area for Enhanced Gadgets
			bP.addClippingRects( damagedEnhancedRects );
			
			// Copy...
			bP.copyTransparent( gadget->getX() , gadget->getY() , gadget->getBitmap() );
			
			// Reduce Painting Area
			damagedEnhancedRects -= gadget->getDimensions();
		}
		else{
			
			bP.addClippingRects( damagedRects );
			
			// Draw...
			bP.copyTransparent( gadget->getX() , gadget->getY() , gadget->getBitmapPort() );
			
			// Reduce Painting Area
			damagedRects -= gadget->getDimensions();
		}
	}
	
	return handled;
}

_gadgetEventReturnType _gadget::gadgetMouseHandler( _gadgetEvent e )
{
	_gadget* that = e.getGadget();
	
	// Temp...
	_gadget* gadget = nullptr;
	_gadgetList::reverse_iterator it;
	
	for( it = that->children.rbegin(); it != that->children.rend() ; it++ )
	{
		// Receive target
		gadget = ( *it );
		
		// Check if event position was inside this Gadget's Area
		if( gadget->getDimensions().contains( e.getArgs().getPosX() , e.getArgs().getPosY() ) )
		{
			// Rewrite Destination
			e.getArgs().setDestination( gadget );
			
			// Absolute Position to Relative Position
			e.getArgs().setPosX( e.getArgs().getPosX() - gadget->getX() );
			e.getArgs().setPosY( e.getArgs().getPosY() - gadget->getY() );
			
			// It doesn't make sense to focus a child of some _gadget that can't be focused
			if( e.getType() == mouseDown && ( that->focused || that->type == _gadgetType::windows ) )
				that->focusEventChild( gadget );
			else if( e.getType() == mouseDoubleClick && !gadget->canReactTo( mouseDoubleClick ) )
				e.setType( mouseClick );
			
			// Trigger the Event
			return gadget->handleEvent( e );
		}
	}
	
	// If no Gadget received the Mousedown, blur the Focussed Child
	if( e.getType() == mouseDown )
		that->blurEventChild();
	
	return not_handled;
}


_gadgetEventReturnType _gadget::gadgetDragHandler( _gadgetEvent e )
{
	// Temp...
	_gadget* gadget;
	_gadgetList::reverse_iterator it;
	
	_gadget* that = e.getGadget();
	
	// Temp...
	_gadgetEventArgs args = e.getArgs();
	
	// Start Dragging
	if( e.getType() == dragStart )
	{
		
		for( it = that->children.rbegin(); it != that->children.rend() ; it++ )
		{
			// Receive target
			gadget = ( *it );
			
			
			// Check if event position was inside this Gadget's Area
			if( gadget->getDimensions().contains( e.getArgs().getPosX() , e.getArgs().getPosY() ) )
			{
				// Rewrite Destination
				args.setDestination( gadget );
				
				// Absolute Position to Relative Position
				args.setPosX( args.getPosX() - gadget->getX() );
				args.setPosY( args.getPosY() - gadget->getY() );
				
				// Trigger the Event
				_gadgetEventReturnType ret = gadget->handleEvent( _gadgetEvent( that , e.getType() , args ) );
				
				if( ret != not_handled )
				{
					// Set Gadget, which receives all other drag Events until dragStop is called
					that->dragTemp = gadget;
				}
				
				return ret;
			}
		}
	}
	// Stop Dragging
	else if( e.getType() == dragStop && that->dragTemp != nullptr )
	{
		// Rewrite Destination
		args.setDestination( that->dragTemp );
		
		// Absolute Position to Relative Position
		args.setPosX( args.getPosX() - that->dragTemp->getX() );
		args.setPosY( args.getPosY() - that->dragTemp->getY() );
		
		// Trigger the Event
		_gadgetEventReturnType ret = that->dragTemp->handleEvent( _gadgetEvent( that , e.getType() , args ) );
		
		// No Gadget will receive Events anymore
		that->dragTemp = nullptr;
		
		return ret;
	}
	else if( e.getType() == dragging && that->dragTemp != nullptr )
	{
		// Rewrite Destination
		args.setDestination( that->dragTemp );
		
		
		// Trigger the Event
		return that->dragTemp->handleEvent( _gadgetEvent( that , e.getType() , args ) );
	}
	return not_handled;
}

_gadgetEventReturnType _gadget::gadgetFocusHandler( _gadgetEvent e )
{
	_gadget* that = e.getGadget();
	
	if( !that->parent )
		return not_handled;
	
	//printf("-%s ",eventType2string[e.getType()].c_str());
	//printf(",%s\n",gadgetType2string[that->getType()].c_str() );
	
	switch( e.getType() )
	{
		case focus:
			that->parent->focusChild( that );
			return handled;
			break;
		case blur:
			that->parent->blurChild();
			that->blurEventChild();
			return handled;
			break;
		default:
			break;
	}
	
	return not_handled;
}