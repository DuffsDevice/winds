#include "_type/type.gadget.h"

_gadget::_gadget( _gadgetType type , int width , int height , int posX , int posY , _gadgetStyle style , bool doNotAllocateBitmap )
	: type( type ) , dimensions( _rect( posX , posY , max( 1 , width ) , max( 1 , height ) ) ) , parent( nullptr ) , style( style ) , dragTemp ( nullptr )
{
	if( !doNotAllocateBitmap )
		this->bitmap = new _bitmap( max( 1 , width ) , max( 1 , height ) );
	
	this->registerDefaultEventHandler( focus , &_gadget::gadgetFocusHandler );
	this->registerDefaultEventHandler( mouseDown , &_gadget::gadgetMouseHandler );
	this->registerDefaultEventHandler( mouseUp , &_gadget::gadgetMouseHandler );
	this->registerDefaultEventHandler( mouseClick , &_gadget::gadgetMouseHandler );
	this->registerDefaultEventHandler( refresh , &_gadget::gadgetRefreshHandler );
	this->registerDefaultEventHandler( dragStart , &_gadget::gadgetDragHandler );
	this->registerDefaultEventHandler( dragging , &_gadget::gadgetDragHandler );
	this->registerDefaultEventHandler( dragStop , &_gadget::gadgetDragHandler );
}

_gadget::_gadget( int width , int height , int posX , int posY , _gadgetStyle style , bool doNotAllocateBitmap )
	: type( _plain ) , dimensions( _rect( posX , posY , max( 1 , width ) , max( 1 , height ) ) ) , parent( nullptr ) , style( style ) , dragTemp ( nullptr )
{
	if( !doNotAllocateBitmap )
		this->bitmap = new _bitmap( max( 1 , width ) , max( 1 , height ) );
}

void _gadget::setPadding( _padding p ){
	this->padding = p;
}

_padding _gadget::getPadding(){ 
	return this->padding;
}

_gadget::~_gadget(){
	if( this->parent != nullptr )
		this->parent->removeChild( this );
	delete this->bitmap;
}

_gadgetType _gadget::getType(){ return this->type; }

void _gadget::triggerEvent( _gadgetEvent event ){
	_gadgetEventArgs args = event.getArgs();
	args.setDestination( this );
	event.setArgs( args );
	this->generateEvent( event );
}

void _gadget::bubbleEvent( _gadgetEvent e , bool includeThis ){
	if( includeThis )
		this->triggerEvent( e );
	if( this->parent != nullptr )
		this->parent->bubbleEvent( e , true );
}

void _gadget::refreshBitmap(){
	_gadgetEvent e = _gadgetEvent( this , refresh , this->getAbsoluteDimensions() , this->getAbsoluteDimensions() );
	this->handleEvent( e );
}

void _gadget::bubbleRefresh( _gadgetEvent e , bool includeThis ){
	if( includeThis )
		this->triggerEvent( e );
	e.getArgs().setSource( this );
	if( this->parent != nullptr )
		this->parent->bubbleRefresh( e , true );
}

void _gadget::bubbleRefresh( bool includeThis ){
	this->bubbleRefresh( _gadgetEvent( this , refresh , this->getAbsoluteDimensions() ) , includeThis );
}

void _gadget::bubbleRealRefresh( bool includeThis ){
	this->bubbleEvent( _gadgetEvent( nullptr , refresh , _rect() , this->getAbsoluteDimensions() ) , includeThis );
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
	this->handleEvent( refresh );
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

bool _gadget::focusChild( _gadget* child )
{
	_gadgetList::iterator itTemp = find( this->children.begin() , this->children.end() , child );
	
	if( child == nullptr || *itTemp == *this->children.rbegin() )
		return false;
	
	this->children.erase( itTemp );
	this->children.push_back( child );
	
	// Refresh the Window
	child->bubbleRefresh();
	
	return true;
}

void _gadget::generateEvent( _gadgetEvent event , bool works ){
	if( !works )
		return;
	_gadget* top = this->getWindows();
	if( top != nullptr )
		top->generateEvent( event , false );
}

bool _gadget::canReactTo( _gadgetEventType type ) const {
	return this->eventHandlers.count( type ) || this->luaEventHandlers.count( type );
}

_gadgetEventReturnType _gadget::handleEventDefault( _gadgetEvent event ){
	// Use the default EventHandler if available
	if( this->defaultEventHandlers.count( event.getType() ) )
	{
		_gadgetDefaultEventHandler handler = this->defaultEventHandlers[ event.getType() ];
		return (this->*handler)( event );
	}
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

void _gadget::setX( int val ){
	_rect dim = this->getAbsoluteDimensions();
	this->dimensions.x = val;
	this->bubbleEvent( _gadgetEvent( this , refresh , this->getAbsoluteDimensions() , dim - this->getAbsoluteDimensions() ) );
}

void _gadget::setY( int val ){
	_rect dim = this->getAbsoluteDimensions();
	this->dimensions.y = val;
	this->bubbleEvent( _gadgetEvent( this , refresh , this->getAbsoluteDimensions() , dim - this->getAbsoluteDimensions() ) );
}

void _gadget::moveRelative( int dX , int dY ){
	_rect dim = this->getAbsoluteDimensions();
	this->dimensions.x += dX;
	this->dimensions.y += dY;
	this->bubbleEvent( _gadgetEvent( this , refresh , this->getAbsoluteDimensions() , dim - this->getAbsoluteDimensions() ) );
}

void _gadget::moveTo( int x , int y ){
	_rect dim = this->getAbsoluteDimensions();
	this->dimensions.x = x;
	this->dimensions.y = y;
	this->bubbleEvent( _gadgetEvent( this , refresh , this->getAbsoluteDimensions() , dim - this->getAbsoluteDimensions() ) );
}

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

void _gadget::removeChild( _gadget* child )
{
	// Find it!
	_gadgetList::iterator it = find( this->children.begin() , this->children.end() , child );
	
	// Found?
	if( it == this->children.end() )
		return;
		
	// Erase it on my bitmap
	this->bubbleEvent( _gadgetEvent( child , refresh , _rect() , child->getAbsoluteDimensions() ) , true );
		
	// Erase the Connection on both sides
	child->parent = nullptr;
	this->children.erase( it );
}

void _gadget::addChild( _gadget* child )
{
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
	this->bubbleEvent( _gadgetEvent( this , refresh , _rect() , dim - this->getAbsoluteDimensions() ) );
}

int _gadget::getHeight() const {
	return this->dimensions.height;
}

int _gadget::getWidth() const {
	return this->dimensions.width;
}

void _gadget::setHeight( int val ){
	if( val != this->dimensions.width ){
		_rect dim = this->getAbsoluteDimensions();
		this->dimensions.height = val;
		this->bitmap->setHeight( val );
		this->onResize();
		
		// Delete the parts that originally were gadget, but became damaged
		this->bubbleEvent( _gadgetEvent( this , refresh , _rect() , dim - this->getAbsoluteDimensions() ) );
	}
}

void _gadget::setWidth( int val ){
	if( val != this->dimensions.width ){
		_rect dim = this->getAbsoluteDimensions();
		this->dimensions.width = val;
		this->bitmap->setWidth( val );
		this->onResize();
		
		// Delete the parts that originally were gadget, but became damaged
		this->bubbleEvent( _gadgetEvent( this , refresh , _rect() , dim - this->getAbsoluteDimensions() ) );
	}
}

_gadgetEventReturnType _gadget::gadgetRefreshHandler( _gadgetEvent event )
{
	// If this Refresh-Event wasn't auto-generated, refresh my parents
	if( !event.getArgs().isBubbleRefresh() )
		this->bubbleRefresh();
	
	// Receive Bitmap-Port
	_bitmapPort bP = this->getBitmapPort();
	
	// Temp...
	_gadget* gadget;
	_gadgetList::reverse_iterator it;
	
	_area availableRects = event.getArgs().getDamagedRects().toRelative( this->getAbsoluteDimensions() );
	_area availableRefreshRect = event.getArgs().getRefreshRects().toRelative( this->getAbsoluteDimensions() );
	
	if( event.getArgs().isBubbleRefresh() )
		bP.addClippingRects( availableRects );
	else
		bP.resetClippingRects();
	
	for( it = this->children.rbegin() ; it != this->children.rend() ; it++ )
	{
		
		// Receive target
		gadget = (*it);
		
		// Draw the gadget that requested the paint?
		if( event.getArgs().isBubbleRefresh() && event.getArgs().getSource() == gadget )
		{
			// Reset clipping Rects
			bP.deleteClippingRects();
			
			// Special Area for the Specific Gadget which wants itself to be refreshed
			bP.addClippingRects( availableRefreshRect );
			bP.copyTransparent( gadget->getX() , gadget->getY() , gadget->getBitmapPort() );
			
			// Return to normal refresh
			bP.deleteClippingRects();
			bP.addClippingRects( availableRects );
		}
		else
		{
			// Draw...
			bP.copyTransparent( gadget->getX() , gadget->getY() , gadget->getBitmapPort() );
		}
		
		// Since we startat the very top Gadget:
		// Reduce the Available Area to
		// the still visible Parts!
		availableRects -= gadget->getDimensions();
		availableRefreshRect -= gadget->getDimensions();
		
		if( event.getArgs().isBubbleRefresh() ){
			bP.deleteClippingRects();
			bP.addClippingRects( availableRects );
		}
	}
	
	return handled;
}

_gadgetEventReturnType _gadget::gadgetMouseHandler( _gadgetEvent e )
{
	// Temp...
	_gadget* gadget;
	_gadgetList::reverse_iterator it;
	
	for( it = this->children.rbegin(); it != this->children.rend() ; it++ )
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
			
			if( e.getType() == mouseDown )
				this->focusChild( gadget );
			
			// Trigger the Event
			return gadget->handleEvent( e );
		}
	}
	
	return not_handled;
}


_gadgetEventReturnType _gadget::gadgetDragHandler( _gadgetEvent e )
{
	// Temp...
	_gadget* gadget;
	_gadgetList::reverse_iterator it;
	
	
	// Temp...
	_gadgetEventArgs args = e.getArgs();
	
	// Start Dragging
	if( e.getType() == dragStart )
	{
		
		for( it = this->children.rbegin(); it != this->children.rend() ; it++ )
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
				_gadgetEventReturnType ret = gadget->handleEvent( _gadgetEvent( this , e.getType() , args ) );
				
				if( ret != not_handled )
				{
					// Set Gadget, which receives all other drag Events until dragStop is called
					this->dragTemp = gadget;
				}
				
				return ret;
			}
		}
	}
	// Stop Dragging
	else if( e.getType() == dragStop && this->dragTemp != nullptr )
	{
		// Rewrite Destination
		args.setDestination( this->dragTemp );
		
		// Absolute Position to Relative Position
		args.setPosX( args.getPosX() - dragTemp->getX() );
		args.setPosY( args.getPosY() - dragTemp->getY() );
		
		// Trigger the Event
		_gadgetEventReturnType ret = this->dragTemp->handleEvent( _gadgetEvent( this , e.getType() , args ) );
		
		// No Gadget will receive Events anymore
		dragTemp = nullptr;
		
		return ret;
	}
	else if( e.getType() == dragging && this->dragTemp != nullptr )
	{
		// Rewrite Destination
		args.setDestination( this->dragTemp );
		
		
		// Trigger the Event
		return dragTemp->handleEvent( _gadgetEvent( this , e.getType() , args ) );
	}
	return not_handled;
}

_gadgetEventReturnType _gadget::gadgetFocusHandler( _gadgetEvent e )
{
	if( this->parent != nullptr )
		return _gadgetEventReturnType( this->parent->focusChild( this ) );
	
	return not_handled;
}