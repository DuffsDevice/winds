// Check if already included
#ifndef _WIN_T_GADGET_
#define _WIN_T_GADGET_

#include "_type/type.bitmap.h"
#include "_type/type.gadgetStyle.h"
#include "_type/type.bitmapPort.h"
#include "_type/type.event.h"
#include "_type/type.callback.h"

// STD-Standards
#include <nds/memory.h>

class _gadget;
class _gadgetScreen;
class _lua_gadget;

// _gadgetList
typedef _list<_gadget*> _gadgetList;

class _gadget{
	
	friend class _lua_gadget;
	
	private:
	
		// Type of the Gadget
		_gadgetType 	type;
		
		
		// Internal
		static bool 	removeDeleteCallback( _gadget* g );
		static bool 	removeCallback( _gadget* g );
		_gadget* getGadgetOfMouseDown( _coord posX , _coord posY );
		
		// Let the gadget blink! this is used if anything can't loose focus
		void blinkHandler();
	
	public:
	
		// Attributes
		_padding				padding;
		_rect 					dimensions;
		_rect 					normalDimensions; // When something is maximized its old dimensions are written in here
		_style					style;
		
		// Children
		_gadgetList				children;
		_gadgetList				enhancedChildren;
		_gadget*				focusedChild;
		
		// Parent
		_gadget*				parent;
		
		// Only for the dragHandler
		_gadget*				dragTemp;
		
		// Event-Handlers
		_map<_eventType
				,_callback*>	eventHandlers;
		// Default event-handlers
		static _map<_eventType
				,_callback*> 	defaultEventHandlers;
		
		// Standard EventHandler
		static _callbackReturn	gadgetRefreshHandler( _event event ) ITCM_CODE ;
		static _callbackReturn	gadgetDragHandler( _event event ) ITCM_CODE ;
		static _callbackReturn	gadgetMouseHandler( _event event ) ITCM_CODE ;
		static _callbackReturn	gadgetKeyHandler( _event event ) ITCM_CODE ;
		
		// Bitmap of the Gadget
		_bitmap	 				bitmap;
		
		//! Class to forward any event to an refresh-event
		class eventForwardRefresh : public _staticCallback
		{
			private:
				static _callbackReturn	refreshForwardHandler( _event event ){ event.getGadget()->bubbleRefresh( true ); return handled; }
			public:
				// Ctor
				eventForwardRefresh() :
					_staticCallback( &eventForwardRefresh::refreshForwardHandler )
				{}
		};
		
		//! Class to forward any event to an refresh-event thrown on a specific gadget
		class eventForwardRefreshGadget : public _classCallback
		{
			private:
				_callbackReturn	refreshForwardHandler( _event event ){ ((_gadget*)this)->bubbleRefresh( true ); return handled; }
			public:
				// Ctor
				eventForwardRefreshGadget( _gadget* g ) :
					_classCallback( g , reinterpret_cast<_callbackReturn (_gadget::*)( _event )>( &eventForwardRefreshGadget::refreshForwardHandler ) )
				{}
		};
		
		//! Class to forward any event to any other
		template<_eventType eT>
		class eventForward : public _staticCallback
		{
			private:
				static _callbackReturn eventForwardHandler( _event event ){ event.getGadget()->handleEvent( event.setType( eT ) ); return handled; }
			public:
				// Ctor
				eventForward() :
					_staticCallback( &eventForward::eventForwardHandler )
				{}
		};
	
	public:
		
		/**
		 * Constructor
		**/
		_gadget( _gadgetType type , int width , int height , int posX , int posY , _style style = _style() , bool doNotAllocateBitmap = false );
		_gadget( int width , int height , int posX , int posY , _style style = _style() , bool doNotAllocateBitmap = false );
		
		/**
		 * Destructor
		**/
		virtual ~_gadget();
		
		/**
		 * Set the Padding of the Gadget
		**/
		void setPadding( const _padding& p )
		{
			if( this->padding != p )
			{
				this->padding = p;
				this->bubbleRefresh( true );
			}
		}
		
		/**
		 * Get the Padding of the Gadget
		**/
		const _padding& getPadding() const { return this->padding; }
	
		/** Method to check whether the Gadget has Focus **/
		bool hasFocus(){ return this->style.focused; }
		
		/** Check whether this Gadget can also be on the reserved area of the parent **/
		bool isEnhanced() const { return this->style.enhanced; }
		
		/** Check whether this Gadget can also be on the reserved area of the parent **/
		bool isVisible() const { return this->style.visible; }
		
		/** Check whether this Gadget is minimized ( probably only available with _window's ) **/
		bool isMinimized() const { return this->style.minimized; }
		
		/** Check whether this Gadget is maximized ( probably only available with _window's ) **/
		bool isMaximized() const { return this->style.maximized; }
		
		/** Check whether this Gadget is minimized ( probably only available with _window's ) **/
		bool isMinimizeable() const { return this->style.minimizeable; }
		
		/** Check whether this Gadget is minimized ( probably only available with _window's ) **/
		bool isDestroyable() const { return this->style.destroyable; }
		
		/** true if resizeable in x- or y-direction **/
		bool isResizeable() const { return this->style.resizeableX || this->style.resizeableY; }
		
		/** true if resizeable in x-direction **/
		bool isResizeableX() const { return this->style.resizeableX; }
		
		/** true if resizeable in y-direction **/
		bool isResizeableY() const { return this->style.resizeableY; }
		
		/** true if the gadget has a special small drag Triger after which it fires an dragStart-Event **/
		bool hasSmallDragTrig() const { return this->style.smallDragTrig; }
		
		/** true if the gadget wants to have repeating clicks instead of just one mouseDown event and then nothing... **/
		bool isMouseClickRepeat() const { return this->style.mouseClickRepeat; }
		
		/** Check whether this Gadget is currently pressed **/
		bool isPressed() const { return this->style.pressed; }
		
		/** Check whether this Gadget is not dragged but one of its children **/
		bool isChildDragged() const { return this->dragTemp != nullptr; }
		
		/**
		 * Check whether this Gadget is doubleclickable
		 * If Not every DoubleClick will be convertetd to a 'mouseClick'-event
		**/
		bool isDoubleClickable() const { return this->style.doubleClickable; }
		
		/**
		 * Let an Event bubble from child to parent and so on...
		**/
		void bubbleEvent( _event e , bool includeThis = false ) ;
		
		/**
		 * Print Contents but make the parent also refresh
		 * the parts that have been changed
		**/
		void bubbleRefresh( bool includeThis = false , _event e = _event() );
		
		/**
		 * Method to refresh itself
		**/
		noinline void refreshBitmap()
		{
			this->handleEvent( _event( refresh ).preventBubble( true ) );
		}
		
		/**
		 * Receive a Bitmap Port
		**/
		_bitmapPort getBitmapPort(){ return this->bitmap; }
		
		/**
		 * Get The Bitmap of the Gadget
		**/
		_bitmap& getBitmap(){ return this->bitmap; }
		
		/**
		 * Get the style of that Gadget
		**/
		const _style& getStyle() const { return this->style; }
		
		/**
		 * Set the style of that Gadget
		**/
		void setStyle( _style style ){ this->style = style; this->triggerEvent( _event( onStyleSet ) ); }
		
		/**
		 * Returns the Toppest Parent, which is usually the Screen/Windows itself
		**/
		noinline _gadgetScreen* getScreen()
		{
			if( this->type == _gadgetType::screen )
				return (_gadgetScreen*)this;
			else if( this->parent != nullptr )
				return this->parent->getScreen();
			return nullptr;
		}
		
		/**
		 * Register a Event Handler to catch some events thrown on this Gadget
		**/
		void registerEventHandler( _eventType type , _callback* handler );
		
		/**
		 * Unbind an EventHandler from this Gadget
		**/
		void unregisterEventHandler( _eventType type );
		
		/**
		 * Method to push an event onto the stack
		 * This usually has nothing to do with the gadget it is invoked on!
		**/
		void populateEvent( _event event );
		
		/**
		 * Trigger an Event (its destination will be set automatically)
		**/
		void triggerEvent( _event event );
		
		/**
		 * Check if this Gadget has an EventHandler registered reacting on the specified type
		**/
		bool canReactTo( _eventType type ) const { return this->eventHandlers.count( type ); }
		
		/**
		 * Make The Gadget act onto a specific GadgetEvent
		**/
		_callbackReturn handleEvent( _event event );
		
		/**
		 * Make The Gadget act onto a specific GadgetEvent by using the Default event-handler if available
		**/
		_callbackReturn handleEventDefault( _event event );
		
		/**
		 * Get the absolute X-position
		**/
		noinline _coord getAbsoluteX() const
		{
			if( this->parent != nullptr )
				return this->parent->getAbsoluteX() + this->dimensions.x + ( this->isEnhanced() ? 0 : this->parent->getPadding().left );
			
			return this->dimensions.x;
		}
		
		/**
		 * Get the absolute Y-position
		**/
		noinline _coord getAbsoluteY() const
		{
			if( this->parent != nullptr )
				return this->parent->getAbsoluteY() + this->dimensions.y + ( this->isEnhanced() ? 0 : this->parent->getPadding().top );
			
			return this->dimensions.y;
		}
		
		/**
		 * Get the Relative X-position
		**/
		const _coord& getX() const { return this->dimensions.x; }
		
		/**
		 * Get the Relative Y-position
		**/
		const _coord& getY() const { return this->dimensions.y; }
		
		/**
		 * Set the Relative X-Position
		**/
		void setX( _coord val );
		
		/**
		 * Set the Relative Y-Position
		**/
		void setY( _coord val );
		
		/**
		 * Hide the Gadget
		**/
		void hide()
		{
			if( this->style.visible )
			{
				this->style.visible = false;
				this->bubbleRefresh();
			}
		}
		
		/**
		 * Unhide the Gadget
		**/
		void show()
		{
			if( !this->style.visible )
			{
				this->style.visible = true;
				this->bubbleRefresh();
			}
		}
		
		/**
		 * Minimize the Gadget
		**/
		void minimize();
		
		/**
		 * Restore the Gadget
		**/
		void restore();
		
		/**
		 * Maximize the Gadget
		**/
		void maximize();
		
		/**
		 * unMaximize (Restore) the Gadget
		**/
		void unMaximize();
		
		/**
		 * Get the Gadgets Parent
		**/
		_gadget* getParent() const { return this->parent; }
		
		/**
		 * Set the Gadgets Parent
		**/
		void setParent( _gadget* val );
		void enhanceToParent( _gadget* val );
		
		/**
		 * Remove a specific children
		**/
		virtual void removeChild( _gadget* child );
		void removeChildren( bool remove = false );
		void removeEnhancedChildren( bool remove = false );
		
		/**
		 * Add a child-gadget to this one
		**/
		virtual void addChild( _gadget* child );
		virtual void addEnhancedChild( _gadget* child );
		
		// Tries to focus a child and returns whether it succeded
		/// @param inform whether to also trigger the 'blur'/'focus' event on the gadget
		bool focusChild( _gadget* child );
		bool blurChild();
		
		/**
		 * Get Dimensions of the Gadget (including Coords)
		**/
		const _rect& getDimensions() const { return this->dimensions; }
		
		/**
		 * Get Absolute Dimensions of the Gadget (including Absolute Coords)
		**/
		_rect getAbsoluteDimensions() const ;
		
		/**
		 * Get The Size of the Gadget as a _rect being at coordinates {0,0}
		**/
		_rect getSize() const { return _rect( 0 , 0 , this->dimensions.width , this->dimensions.height ); }
		
		/**
		 * Set Dimensions of the Gadget (including Coords)
		**/
		virtual void setDimensions( _rect rc );
		
		/**
		 * Get the height of the Gadget
		**/
		const _length& getHeight() const { return this->dimensions.height; }
		
		/**
		 * Get the width of the Gadget
		**/
		const _length& getWidth() const { return this->dimensions.width; }
		
		/**
		 * Move the Gadget relatively to its current position
		**/
		void moveTo( _coord dX , _coord dY );
		
		/**
		 * Relative moving fo the Gadget
		**/
		void moveRelative( _s16 deltaX , _s16 deltaY );
		
		/**
		 * Set Height " " "
		**/
		virtual void setHeight( _length val );
		
		/**
		 * Set Width " " "
		**/
		virtual void setWidth( _length val );
		
		/**
		 * Blur the Gadget
		**/
		void blur()
		{
			this->parent && this->parent->focusedChild == this && this->parent->blurChild();
		}
		
		/**
		 * Focus the Gadget
		**/
		void focus()
		{
			this->parent && this->parent->focusChild( this );
		}
		
		/**
		 * Get the toppest child owned by the parent
		**/
		_gadget* getToppestChild(){	return this->children.front(); }
		
		/**
		 * Get the Type of the Gadget (enum)
		 * @see type.h:111
		**/
		_gadgetType getType(){ return this->type; }
		
		/**
		 * Let the gadget blink!
		**/
		void blink();
		
	protected:
		
		/**
		 * There's no public setType, because you shouldn't change the type of a Gadget
		**/
		void setType( _gadgetType type ){ this->type = type; }
		
	private:
		
		/**
		 * Set whether this Gadget can also be on the reserved area of the parent
		**/
		void setEnhanced( bool flag = true ){ this->style.enhanced = flag; }
};

inline _gadgetType typeOfGadget( _gadget* g );

#endif
