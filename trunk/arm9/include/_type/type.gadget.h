// Check if already included
#ifndef _WIN_T_GADGET_
#define _WIN_T_GADGET_

#include "_type/type.bitmap.h"
#include "_type/type.style.h"
#include "_type/type.bitmap.port.h"
#include "_type/type.event.h"
#include "_type/type.event.calltype.h"
#include "_type/type.callback.h"
#include "_type/type.callback.derives.h"
#include "_type/type.assocvector.h"
#include "_type/type.dependency.h"
#include "_type/type.gadget.type.h"

#include <type_traits>

//! Predefines
class _gadget;
class _gadgetScreen;

//! Typedef
typedef _list<_gadget*> 									_gadgetList;

typedef _callbackReturn 									_eventHandler(_event);
typedef _assocVector<_eventType,_callback<_eventHandler>*> 	_eventHandlerMap;

typedef _array<_staticCallback<_eventHandler>,13>	_defaultEventHandlerMap;

class _gadget
{
	friend class _lua_gadget;
	
	protected:
	
		//! Padding
		union{
			_padding			padding;
			struct
			{
				_length 		padLeft;
				_length 		padTop;
			};
		};
		
		//! Size limits
		_length					minWidth; // If this is 0, its not given
		_length					minHeight; // If this is 0, its not given
		
		//! Position
		_coord 					x;
		_coord 					y;
		
		//! Describes the look and feel of the gadget
		_style					style;
		
		//! Children
		_gadgetList				children;
		_gadgetList				enhancedChildren;
		
		//! Shortcut to the currently focused child (or nullptr if no child is focused)
		_gadget*				focusedChild;
		
		//! A Gadgets Parent
		_gadget*				parent;
		
		//! Event-Handlers
		_eventHandlerMap		eventHandlers;
		
		//! Default Event-Handlers
		static _defaultEventHandlerMap
								defaultEventHandlers; // Default event-handlers (an array because of speed)
		
		/**
		 * Standard/Default EventHandler that will handle
		 * specific events if the 'normal' eventHandlers don't
		 */
		static _callbackReturn	gadgetRefreshHandler( _event event ) ITCM_CODE ;
		static _callbackReturn	gadgetDragStartHandler( _event event );
		static _callbackReturn	gadgetDraggingHandler( _event event );
		static _callbackReturn	gadgetDragStopHandler( _event event );
		static _callbackReturn	gadgetMouseHandler( _event event );
		static _callbackReturn	gadgetKeyHandler( _event event );
		
		//! Bitmap of the Gadget
		_bitmap	 				bitmap;
	
	public:
		
		/**
		 * Constructor
		 */
		_gadget( _gadgetType type , _optValue<_coord> posX , _optValue<_coord> posY , _optValue<_length> width , _optValue<_length> height , _bitmap&& bmp , _style&& style = _style() );
		_gadget( _gadgetType type , _optValue<_coord> posX , _optValue<_coord> posY , _optValue<_length> width , _optValue<_length> height , _style&& style = _style() ) :
			_gadget( type , posX , posY , width , height , _bitmap( width , height ) , (_style&&)style )
		{} // Delegating contructor! C++11 I love you!
		
		/**
		 * Destructor
		 */
		virtual ~_gadget();
		
		
		////////////////////////
		//  Attribute getters //
		////////////////////////
		
		/** Method to check whether the Gadget has Focus */
		bool hasFocus() const { return this->focused || this->type == _gadgetType::screen; }
		
		/** Check whether this Gadget is currently pressed */
		bool isPressed() const { return this->pressed; }
		
		/** Check whether this Gadget is not dragged but one of its children */
		bool isChildDragged() const { return this->draggedChild != nullptr && ( this->draggedChild->dragged || this->draggedChild->isChildDragged() ); }
		
		/** Check whether this Gadget can also be on the reserved area of the parent */
		bool isEnhanced() const { return this->enhanced; }
		
		/** Check whether this Gadget was hidden by a previous call to hide() */
		bool isHidden() const { return this->hidden; }
		
		/** Check whether this Gadget is enabled */
		bool isEnabled() const { return this->style.enabled; }
		
		/** Check whether this Gadget can be clicked (if true, which is default, it receives mouse-events) */
		bool isClickable() const { return this->style.clickable; }
		
		/** true if resizeable in x-direction */
		bool isResizeableX() const { return this->style.resizeableX; }
		
		/** true if resizeable in y-direction */
		bool isResizeableY() const { return this->style.resizeableY; }
		
		/** true if resizeable in both x and y-direction */
		bool isResizeable() const { return this->style.resizeableX && this->style.resizeableY; }
		
		/** true if the gadget has a special small drag Triger after which it fires an dragStart-Event */
		bool hasSmallDragTrig() const { return this->style.smallDragTrig; }
		
		/** true if the gadget wants to have repeating clicks instead of just one mouseDown event and then nothing... */
		bool isMouseClickRepeat() const { return this->style.mouseClickRepeat; }
		
		/** Check whether this Gadget requests the keyboard to open on focus */
		bool requestsKeyboard() const { return this->style.keyboardRequest; }
		
		/** Check whether or not this gadget can handle drag-scenarios */
		bool isDraggable() const { return this->style.draggable; }
		
		/**
		 * Check whether this Gadget is doubleclickable
		 * If Not every DoubleClick will be convertetd to a 'mouseClick'-event
		 */
		bool isDoubleClickable() const { return this->style.doubleClickable; }
		
		
		////////////////
		// Refreshing //
		////////////////
		
		/**
		 * Print Contents and make the parent also draw
		 * the parts that have been changed
		 */
		void redraw(){
			this->redraw( getAbsoluteDimensions() );
		}
		void redrawParents(){
			this->redrawParents( getAbsoluteDimensions() );
		}
		
		/**
		 * Method to update some things (optional)
		 */
		void update(){ //! A-Sync-Version (the gadget will be updated on VBL)
			this->triggerEvent( _eventType::onUpdate );
		}
		_callbackReturn updateNow(){ //! The Gadget will be updated *now* and the return is given back after the gadget was updated
			return this->handleEvent( _eventType::onUpdate );
		}
		
		/**
		 * Receive a Bitmap Port
		 */
		_bitmapPort getBitmapPort(){ return this->bitmap; }
		
		//! BitmapPort with area preset
		_bitmapPort getBitmapPort( _area&& areaToPaintTo ){ return _bitmapPort( this->bitmap , move(areaToPaintTo) ); }
		_bitmapPort getBitmapPort( const _area& areaToPaintTo ){ return _bitmapPort( this->bitmap , areaToPaintTo ); }
		
		//! BitmapPort from 'onDraw'-event
		_bitmapPort getBitmapPort( const _event& event ){
			if( event == onDraw && event.hasClippingRects() )
				return _bitmapPort( this->bitmap , event.getDamagedRects().toRelative( getAbsolutePosition() ) );
			return this->bitmap;
		}
		_bitmapPort getBitmapPort( _event& event ){
			if( event == onDraw && event.hasClippingRects() )
				return _bitmapPort( this->bitmap , event.getDamagedRects().toRelative( getAbsolutePosition() ) );
			return this->bitmap;
		}
		
		//! Get The Bitmap of the Gadget
		_constBitmap& getBitmap() const { return this->bitmap; }
		
		/**
		 * Set The Bitmap of the Gadget	
		 * This automatically resizes the gadget to to fit the bitmap
		 */
		void setBitmap( _bitmap );
		
		
		/////////////////////
		//  Gadget - Style //
		/////////////////////
		
		//! Get the style of that Gadget
		const _style& getStyle() const { return this->style; }
		
		//! Set the style of that Gadget
		void setStyle( _style style ){ this->style = (_style&&)style; notifyDependentGadgets( onRestyle ); }
		
		
		////////////////////
		// Event-Handlers //
		////////////////////
		
		/**
		 * Register an internal Event Handler to catch some events thrown on this Gadget
		 * This method should only be called in system-internal routines
		 */
		void setInternalEventHandler( _eventType type , _paramAlloc<_callback<_eventHandler>> handler );
		
		//! Register an user-specific Event Handler to catch some events thrown on this Gadget
		void setUserEventHandler( _eventType type , _paramAlloc<_callback<_eventHandler>> handler ){
			this->setInternalEventHandler( eventType2userET(type) , move(handler) );
		}
		
		/**
		 * Unbind an EventHandler from this Gadget
		 * that was registered by default
		 */
		void removeInternalEventHandler( _eventType type );
		
		/**
		 * Unbind an EventHandler from this Gadget
		 * that was registered by the user
		 */
		void removeUserEventHandler( _eventType type ){
			removeInternalEventHandler( eventType2userET(type) );
		}
		
		/**
		 * Check if this Gadget has an EventHandler registered reacting on the specified type
		 */
		bool canReactTo( _eventType type ) const { return this->eventHandlers.count( type ) || this->eventHandlers.count( eventType2userET(type) ); }
		
		
		////////////////////
		// Event-Handling //
		////////////////////
		
		/**
		 * Make The Gadget act onto a specific GadgetEvent
		 */
		_callbackReturn handleEvent( _event event , bool noDefault = false ) ITCM_CODE ;
		
		/**
		 * Make The Gadget act onto a specific GadgetEvent
		 * by only using user-registered event-handler if available
		 */
		_callbackReturn handleEventUser( _event event );
		
		/**
		 * Make The Gadget act onto a specific GadgetEvent
		 * by only using user-registered event-handler if available
		 */
		_callbackReturn handleEventInternal( _event event );
		
		/**
		 * Make The Gadget act onto a specific GadgetEvent
		 * by only using the Default gadget-event-handler if available
		 */
		_callbackReturn handleEventDefault( _event event ); 
		
		
		/**
		 * Trigger an Event (its destination will be set automatically and it will be handled as soon as there is cpu-power available)
		 */
		//! Will be handled using the method passed or as default by calling 'handleEvent'
		void triggerEvent( _event event , _eventCallType callType = _eventCallType::normal );
		
		
		//! Will be handled by 'handleEvent( ... , false )' or 'handleEvent( ... , true )'
		void triggerEvent( _event event , bool noDefault ){ triggerEvent( move(event) , noDefault ? _eventCallType::normalNoDef : _eventCallType::normal ); }
		
		//! Will be handled by a user-registered event-handler
		void triggerEventUser( _event event ){ triggerEvent( move(event) , _eventCallType::user ); }
		
		//! Will be handled by a gadget-registered handler
		void triggerEventInternal( _event event ){ triggerEvent( move(event) , _eventCallType::internal ); }
		
		//! Will be handled by a default-registered handler
		void triggerEventDefault( _event event ){ triggerEvent( move(event) , _eventCallType::def ); }
		
		
		//////////////////////////////////////
		// Full Dimensions-_rect operations //
		//////////////////////////////////////
		
		//! Get Dimensions of the Gadget (including Coords)
		_rect getDimensions() const { return _rect( this->x , this->y , getWidth() , getHeight() ); }
		
		//! Get Absolute Dimensions of the Gadget
		_rect getAbsoluteDimensions() const {
			_2s32 val = getAbsolutePosition();
			return _rect( val.first , val.second , getWidth() , getHeight() );
		}
		
		/**
		 * Set Dimensions of the Gadget (including Coords)
		 */
		//! Internal call, that does not violate the state of auto-computed values
		void setDimensionsIfAuto( _rect rc )
		{
			if( !this->autoValues.posX )	rc.x = this->getX();
			if( !this->autoValues.posY )	rc.y = this->getY();
			if( !this->autoValues.width )	rc.width = this->getWidth();
			if( !this->autoValues.height )	rc.height = this->getHeight();
			setDimensionsInternal( rc );
		}
		
		//! Sets a specific size and position
		void setDimensions( _rect rc ){
			this->autoValues.sum = false; // Set all four values to false
			setDimensionsInternal( rc );
		}
		
		//! Requests the gadget to both compute position and size
		void requestAutoDimensions(){
			if( !this->autoValues.sum )
				return;
			this->autoValues.sum = ~false; // Set all four values to true
			notifyDependentGadgets( onResize );
			notifyDependentGadgets( onMove );
		}		
		
		
		///////////////////////
		// Absolute Position //
		///////////////////////
		
		//! Get both y and y absolute coordinate
		_2s32 getAbsolutePosition() const ITCM_CODE;
		
		//! Get the absolute X-position
		noinline _coord getAbsoluteX() const {
			if( this->parent != nullptr )
				return this->parent->getAbsoluteX() + this->x + ( isEnhanced() ? 0 : this->parent->getPadding().left );
			
			return this->x;
		}
		
		//! Get the absolute Y-position
		noinline _coord getAbsoluteY() const {
			if( this->parent != nullptr )
				return this->parent->getAbsoluteY() + this->y + ( this->isEnhanced() ? 0 : this->parent->getPadding().top );
			
			return this->y;
		}
		
		
		/////////////////////////////////
		// Position relative to parent //
		/////////////////////////////////
		
		/**
		 * Set the gadgets position withing the parents dimensions
		 */
		//! Internal call, that does not violate the state of auto-computed values
		void moveToIfAuto( _coord x , _coord y ){
			moveRelativeIfAuto( x - this->x , y - this->y );
		}
		
		//! Sets a specific position
		void moveTo( _coord x , _coord y ){
			moveRelative( x - this->x , y - this->y );
		}
		
		//! Requests the gadget to auto-compute the required values
		void requestAutoPosition(){
			if( this->autoValues.posX && this->autoValues.posY )
				return;
			this->autoValues.posX = true;
			this->autoValues.posY = true;
			notifyDependentGadgets( onMove );
		}
		
		/**
		 * Move the Gadget relatively to its current position
		 */
		//! Internal call, that does not violate the state of auto-computed values
		void moveRelativeIfAuto( _s16 deltaX , _s16 deltaY )
		{
			if( !this->autoValues.posX ) // Set to null if this value isn't auto-generated
				deltaX = 0;
			if( !this->autoValues.posY )
				deltaY = 0;
			moveRelativeInternal( deltaX , deltaY );
		}
		
		//! Moves by difference
		void moveRelative( _s16 deltaX , _s16 deltaY ){
			this->autoValues.posX = false;
			this->autoValues.posY = false;
			moveRelativeInternal( deltaX , deltaY );
		}
		
		//! Get the Relative X-position
		_coord getX() const { return this->x; }
		
		//! Get the Relative Y-position
		_coord getY() const { return this->y; }
		
		
		//! Set the Relative X-Position
		void setXIfAuto( _coord val ){
			if( this->autoValues.posX )
				this->setXInternal( val );
		}
		void setX( _coord val ){
			this->autoValues.posX = false;
			setXInternal( val );
		}
		void requestAutoX(){
			if( this->autoValues.posX )
				return;
			this->autoValues.posX = true;
			notifyDependentGadgets( onMove );
		}
		
		//! Set the Relative Y-Position
		void setYIfAuto( _coord val )
		{
			if( this->autoValues.posY )
				this->setYInternal( val );
		}
		void setY( _coord val ){
			this->autoValues.posY = false;
			setYInternal( val );
		}
		void requestAutoY(){
			if( this->autoValues.posY )
				return;
			this->autoValues.posY = true;
			notifyDependentGadgets( onMove );
		}
		
		
		//! Check if position is auto-computed
		bool hasAutoX() const { return this->autoValues.posX; }
		bool hasAutoY() const { return this->autoValues.posY; }
		
		
		//////////////////
		//     Size     //
		//////////////////
		
		/**
		 * Set Width
		 */
		//! Internal call, that does not violate the state of auto-computed values
		void setWidthIfAuto( _length val )
		{
			if( this->autoValues.width )
				this->setWidthInternal( val );
		}
		
		//! Set specific width
		void setWidth( _length val ){
			this->autoValues.width = false;
			setWidthInternal( val );
		}
		
		//! Requests the gadget to auto-compute the required values
		void requestAutoWidth(){
			if( this->autoValues.width )
				return;
			this->autoValues.width = true;
			notifyDependentGadgets( onResize );
		}
		
		/**
		 * Set Height
		 */
		//! Internal call, that does not violate the state of auto-computed values
		void setHeightIfAuto( _length val )
		{
			if( this->autoValues.height )
				setHeightInternal( val );
		}
		
		//! Sets specific height
		void setHeight( _length val ){
			this->autoValues.height = false;
			setHeightInternal( val );
		}
		
		//! Requests the gadget to auto-compute the required values
		void requestAutoHeight(){
			if( this->autoValues.height )
				return;
			this->autoValues.height = true;
			notifyDependentGadgets( onResize );
		}
		
		/**
		 * Set Size
		 */
		//! Internal call, that does not violate the state of auto-computed values
		void setSizeIfAuto( _length width , _length height )
		{
			if( !this->autoValues.width )
				setHeightIfAuto( height );
			else if( !this->autoValues.height )
				setWidthInternal( width );
			else
				setSizeInternal( width , height );
		}
		
		//! Sets a specific size
		void setSize( _length width , _length height ){
			this->autoValues.width = false;
			this->autoValues.height = false;
			setSizeInternal( width , height );
		}
		
		//! Requests the gadget to auto-compute its size
		void requestAutoSize(){
			if( this->autoValues.width && this->autoValues.height )
				return;
			this->autoValues.width = true;
			this->autoValues.height = true;
			notifyDependentGadgets( onResize );
		}
		
		
		//! Get the Size of the Gadget as a _rect inheriting the position (0,0)
		_rect getSizeRect() const { return _rect( 0 , 0 , this->getWidth() , this->getHeight() ); }
		
		//! Get the Size of the Gadget as a pair of (width|height)
		_2s32 getSize() const { return _2s32( this->getWidth() , this->getHeight() ); }
		
		//! Get the relative Area that normal children can inherit
		_rect getClientRect() const { return _rect( 0 , 0 , this->getWidth() , this->getHeight() ).applyPadding( this->padding ); }
		
		
		//! Get the height of the Gadget
		_length getHeight() const { return this->bitmap.getHeight(); }
		
		//! Get the width of the Gadget
		_length getWidth() const { return this->bitmap.getWidth(); }
		
		
		//! Check if size is auto-computed
		bool hasAutoHeight() const { return this->autoValues.height; }
		bool hasAutoWidth() const { return this->autoValues.width; }
		
		
		//////////////////
		// Minimum Size //
		//////////////////
		
		//! Get the minimum height:
		//! The sum of vertical padding, but at least 1px
		_length getDefMinHeight() const { return max( 1 , this->padding.top + this->padding.bottom ); } // Gadget-given minHeight
		
		//! Get the minimum allowed height, specified by the attribute 'minHeight'
		_length getMinHeight() const { return this->minHeight > 0 ? this->minHeight : this->getDefMinHeight(); }
		
		
		//! Get the minimum width:
		//! The sum of horizontal padding, but at least 1px
		_length getDefMinWidth() const { return max( 1 , this->padding.left + this->padding.right ); } // Gadget-given minWidth
		
		//! Get the minimum allowed width, specified by the attribute 'minWidth'
		_length getMinWidth() const { return this->minWidth > 0 ? this->minWidth : this->getDefMinWidth(); }
		
		
		//! Set minimum width
		void setMinWidth( _optValue<_length> val = ignore ){
			this->minWidth = val.isValid() ? max<_length>( 1 , val ) : 0;
			setWidthInternal( this->getWidth() ); // Check if right width given
		}
		
		//! Set minimum height
		void setMinHeight( _optValue<_length> val = ignore ){
			this->minHeight = val.isValid() ? max<_length>( 1 , val ) : 0;
			setHeightInternal( this->getHeight() ); // Check if right height given
		}
		
		
		////////////////
		// DOM - Tree //
		////////////////
		
		//! Get the Gadgets Parent
		_gadget* getParent() const { return this->parent; }
		
		//! Set the Gadgets Parent
		void setParent( _gadget* val );
		
		//! Set the Gadgets Parent and enhance the gadget
		void enhanceToParent( _gadget* val );
		
		//! Returns the Toppest Parent, which is usually the Screen/Windows itself
		noinline _gadgetScreen* getScreen() const {
			if( this->parent != nullptr )
				return this->parent->getScreen();
			if( this->type == _gadgetType::screen )
				return (_gadgetScreen*)this;
			return nullptr;
		}
		
		
		//! Remove a specific child (no matter whether it is enhanced)
		void removeChild( _gadget* child );
		
		//! Remove all 'normal' children and optionally 'delete' them
		void removeChildren( bool remove = false );
		
		//! Remove all enhanced children and optionally 'delete' them
		void removeEnhancedChildren( bool remove = false );
		
		/**
		 * Add a child-gadget to this one
		 * @param pushBack Whether the child should be added to the back of the children instead of the front
		 **/
		void addChild( _gadget* child , bool pushBack = false );
		
		/**
		 * Add a enhanced child-gadget to this one
		 * @param pushBack Whether the child should be added to the back of the children instead of the front
		 **/
		void addEnhancedChild( _gadget* child , bool pushBack = false );
		
		//! Get the toppest child owned by the parent
		_gadget* getToppestChild() const {	return this->children.front(); }
		
		//! Get the lowest child owned by the parent
		_gadget* getLowestChild() const {	return this->children.back(); }
		
		//! Get the focused gadget inside the parent
		_gadget* getFocusedChild() const {	return this->focusedChild; }
		
		//! Get the toppest enhanced child owned by the parent
		_gadget* getToppestEnhancedChild() const {	return this->enhancedChildren.front(); }
		
		//! Get the lowest enhanced child owned by the parent
		_gadget* getLowestEnhancedChild() const {	return this->enhancedChildren.back(); }
		
		//! Helps to find adjacent children
		_gadget*	getPrecedentChild( bool skipHidden = false );
		_gadget*	getSubcedentChild( bool skipHidden = false );
		
		//! Tries to focus a child and returns whether this succeded
		bool focusChild( _gadget* child );
		
		//! Tries to blur the currently focused child, if there is one
		//! @return Whether there is no child focused anymore
		bool blurChild();
		
		
		/////////////////
		//    State    //
		/////////////////		
		
		//! Hide the Gadget
		void hide(){
			if( !this->hidden ){
				this->hidden = true;
				this->redraw();
			}
		}
		
		//! Unhide the Gadget
		void show(){
			if( this->hidden ){
				this->hidden = false;
				redraw();
			}
		}
		
		//! Blur the Gadget
		bool blur(){
			if( this->parent && this->parent->focusedChild == this )
				return this->parent->blurChild();
			return this->type == _gadgetType::screen; // You can do everything with a screen!
		}
		
		//! Focus the Gadget
		bool focus();
		
		//! Set the Padding of the Gadget
		void setPadding( const _padding& p ){
			if( this->padding != p ){
				this->padding = p;
				redraw();
			}
		}
		
		//! Get the Padding of the Gadget
		const _padding& getPadding() const { return this->padding; }
		
		
		//! Get the Type of the Gadget (enum)
		_gadgetType getType() const { return this->type; }
		
		
		//! Let the gadget blink! This is used if anything can't loose focus
		void blink();
		
	protected:
		
		/**
		 * There's no public setType, because you shouldn't change the type of a Gadget
		 */
		void setType( _gadgetType type ){ this->type = type; }
		
	
	private:
		
		//////////////////
		// Dependencies //
		//////////////////
		
		//! Add a dependency, that if it changes throws an 'onDependencyChange'-event
		void addDependency( _eventType type ){
			this->dependencies.set( type , true );
		}
		
		//! Remove a specific dependency
		void removeDependency( _eventType type ){
			this->dependencies.set( type , false );
		}
	public:
		//! Check whether a gadget is dependent of another
		bool isDependentOf( _eventType type ){
			return this->dependencies.get( type );
		}
	private:
		//! Inform dependent gadgets about something that changed
		void		notifyDependentGadgets( _eventType change , bool notifySelf = true , _dependencyParam param = _dependencyParam() ){
			if( notifySelf )
				notifyDependentSelf( change , param );
			notifyDependentParent( change , param );
			notifyDependentAdjacents( change , param );
			notifyDependentChildren( change , param );
		}
		void		notifyDependentSelf( _eventType change , _dependencyParam param = _dependencyParam() );
		void		notifyDependentChildren( _eventType change , _dependencyParam param = _dependencyParam() );
		void		notifyDependentParent( _eventType change , _dependencyParam param = _dependencyParam() , _optValue<_gadget*> parent = ignore );
		void		notifyDependentAdjacents( _eventType change , _dependencyParam param = _dependencyParam() , _optValue<_gadget*> pre = ignore , _optValue<_gadget*> post = ignore );
		
		
		//////////////////////////////////////
		// Internal functions that          //
		// manipulate the Size and Position //
		//////////////////////////////////////
		
		void setWidthInternal( _length val );
		void setHeightInternal( _length val );
		void setXInternal( _coord val );
		void setYInternal( _coord val );
		void setSizeInternal( _length width , _length height );
		void moveRelativeInternal( _s16 deltaX , _s16 deltaY );
		void setDimensionsInternal( _rect rc );
		
		
		//////////////////////////////////
		// More attributes and methods! //
		//////////////////////////////////
		
		//! Internal callbacks for removeChildren( true/false )
		static bool	removeDeleteCallback( _gadget* g );
		static bool	removeCallback( _gadget* g );
		
		
		
		//! Used to receive the gadget that a mouse position is in
		_gadget*	getGadgetOfMouseDown( _coord posX , _coord posY );
		
		//! Determine whether a gadget can currently be clicked either because it has focus or because it cannot receive focus at all
		bool		hasClickRights() const ;
		
		//! Let the gadget blink! This is used if anything can't loose focus
		void		blinkHandler(); 
		
		//! _gadget::dragHandler
		_gadget*	draggedChild;
		
		//! Type of the Gadget
		_gadgetType type;
		
		//! Reset a gadgets state-attribute
		inline void resetState(){
			//this->hidden = false;
			//this->pressed = false;
			//this->enhanced = false;
			//this->dragged = false;
			//this->focused = false;
			this->state = 0; // Does the same
		}
		
		//! Range-bound-refreshes are only private
		void redraw( _area&& areaToRefresh );
		void redrawParents( _area&& areaToRefresh );
		
		
		////////////////////
		//  State-Object  //
		////////////////////
		
		union 
		{
			_u8 state; // used to reset everything quickly
			struct{
				bool	hidden : 1;
				bool	pressed : 1;
				bool	enhanced : 1;
				bool	dragged : 1;
				bool	focused : 1;
				_u8		cntBlnk : 3; // For _gadget::blinkHandler
			} PACKED ;
		};
		
		
		///////////////////////////////////
		// Automatically computed values //
		///////////////////////////////////
		
		union{
			struct
			{
				bool	width : 1;
				bool	height : 1;
				bool	posX : 1;
				bool	posY : 1;
			} PACKED ;
			_u8			sum;
		}				autoValues;
		
		///////////////////////
		// Dependency Object //
		///////////////////////
		
		_dependency		dependencies;
};

// Include useful helpers
#include "_type/type.gadget.helpers.h"

#endif