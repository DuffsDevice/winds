// Check if already included
#ifndef _WIN_T_GADGET_
#define _WIN_T_GADGET_

#include "_type/type.bitmap.h"
#include "_type/type.style.h"
#include "_type/type.bitmapPort.h"
#include "_type/type.event.h"
#include "_type/type.callback.h"
#include "_type/type.assocvector.h"

//! Predefines
class _gadget;
class _gadgetScreen;

/**
 * Specifies the Type of an API-Element (aka _gadget)
 */
enum class _gadgetType : _u8
{
	button,
	label,
	checkbox,
	radiobox,
	textbox,
	textarea,
	counter,
	calendar,
	colorpicker,
	selectbox,
	selectitem,
	progressbar,
	keyboard,
	desktop,
	fileview,
	fileobject,
	imagegadget,
	scrollarea,
	scrollbutton,
	scrollbar,
	window,
	screen,
	contextmenu,
	_plain // No type set (is probably not used)
};

extern _map<_gadgetType,string> gadgetType2string;

//! Typedef
typedef _list<_gadget*> _gadgetList;
typedef _assocVector<_eventType,_callback*> _eventHandlerMap;

class _gadget
{
	friend class _lua_gadget;
	
	protected:
	
		// Attributes
		union{
			_padding			padding;
			struct
			{
				_length 		padLeft;
				_length 		padTop;
			};
		};
		union
		{
			_rect				dimensions;
			struct{
				//! Size
				_length 		width;
				_length 		height;
				//! Position
				_coord 			x;
				_coord 			y;
			};
		};
		_rect 					normalDimensions; // When something is maximized its old dimensions are written in here
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
		static _array<_fastEventCallback
						,13>	defaultEventHandlers; // Default event-handlers (an array because of speed)
		
		/**
		 * Standard/Default EventHandler that will handle
		 * specific events if the 'normal' eventHandlers don't
		 */
		static _callbackReturn	gadgetRefreshHandler( _event&& event ) ITCM_CODE ;
		static _callbackReturn	gadgetDragHandler( _event&& event );
		static _callbackReturn	gadgetMouseHandler( _event&& event );
		static _callbackReturn	gadgetKeyHandler( _event&& event ){
			event.getGadget()->parent && event.getGadget()->parent->handleEvent( event );
			return not_handled;
		}
		
		//! Bitmap of the Gadget
		_bitmap	 				bitmap;
		
		//! Class to forward any event to an refresh-event
		class eventForwardRefresh : public _fastEventCallback
		{
			private:
				static _callbackReturn refreshForwardHandler( _event&& event ){ event.getGadget()->bubbleRefresh( true ); return handled; }
			public:
				// Ctor
				eventForwardRefresh() :
					_fastEventCallback( &eventForwardRefresh::refreshForwardHandler )
				{}
		};
		
		//! Class to forward any event to an refresh-event thrown on a specific gadget
		class eventForwardRefreshGadget : public _classCallback
		{
			private:
				void refreshForwardHandler(){ ((_gadget*)this)->bubbleRefresh( true ); }
			public:
				// Ctor
				eventForwardRefreshGadget( _gadget* g ) :
					_classCallback( g , reinterpret_cast<void (_gadget::*)()>( &eventForwardRefreshGadget::refreshForwardHandler ) )
				{}
		};
		
		//! Class to forward any event to any other
		template<_eventType eT>
		class eventForward : public _fastEventCallback
		{
			private:
				static _callbackReturn eventForwardHandler( _event&& event ){ event.getGadget()->handleEvent( (_event&&)event.setType( eT ) ); return handled; }
			public:
				// Ctor
				eventForward() :
					_fastEventCallback( &eventForward::eventForwardHandler )
				{}
		};
	
	public:
		
		/**
		 * Constructor
		 */
		_gadget( _gadgetType type , int width , int height , int posX , int posY , true_type doNotAllocateBitmap , _style&& style = _style() );
		_gadget( _gadgetType type , int width , int height , int posX , int posY , _style&& style = _style() );
		// Delegating Constructors!!!! C++0x I love you!
		_gadget( int width , int height , int posX , int posY , true_type doNotAllocateBitmap , _style&& style = _style() )
			: _gadget( _gadgetType::_plain , width , height , posX , posY , doNotAllocateBitmap , (_style&&)style )
		{ }
		_gadget( int width , int height , int posX , int posY , _style&& style = _style() )
			: _gadget( _gadgetType::_plain , width , height , posX , posY , (_style&&)style )
		{ }
		
		/**
		 * Destructor
		 */
		virtual ~_gadget();
		
		/**
		 * Set the Padding of the Gadget
		 */
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
		 */
		const _padding& getPadding() const { return this->padding; }
		
		
		/** Method to check whether the Gadget has Focus */
		bool hasFocus() const { return this->focused || this->type == _gadgetType::screen; }
		
		/** Check whether this Gadget is currently pressed */
		bool isPressed() const { return this->pressed; }
		
		/** Check whether this Gadget is not dragged but one of its children */
		bool isChildDragged() const { return this->dragTemp != nullptr; }
		
		/** Check whether this Gadget can also be on the reserved area of the parent */
		bool isEnhanced() const { return this->enhanced; }
		
		/** Check whether this Gadget was hidden by a previous call to hide() */
		bool isHidden() const { return this->hidden; }
		
		/** Check whether this Gadget is invisible, either because it is minimized or it is made 'hidden' */
		bool isInvisible() const { return this->state & 0x3; }
		
		/** Check whether this Gadget is minimized ( probably only available with _window's ) */
		bool isMinimized() const { return this->minimized; }
		
		/** Check whether this Gadget is maximized ( probably only available with _window's ) */
		bool isMaximized() const { return this->maximized; }
		
		
		/** Check whether this Gadget is minimized ( probably only available with _window's ) */
		bool isMinimizeable() const { return this->style.minimizeable; }
		
		/** Check whether this Gadget is minimized ( probably only available with _window's ) */
		bool isDestroyable() const { return this->style.destroyable; }
		
		/** Check whether this Gadget can be clicked (if true, which is default, it receives mouse-events) */
		bool isClickable() const { return this->style.clickable; }
		
		/** true if resizeable in x- or y-direction */
		bool isResizeable() const { return this->style.resizeableX || this->style.resizeableY; }
		
		/** true if resizeable in x-direction */
		bool isResizeableX() const { return this->style.resizeableX; }
		
		/** true if resizeable in y-direction */
		bool isResizeableY() const { return this->style.resizeableY; }
		
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
		
		/**
		 * Print Contents but make the parent also refresh
		 * the parts that have been changed
		 */
		void bubbleRefresh( bool includeThis , _event&& e ) ITCM_CODE ;
		void bubbleRefresh( bool includeThis , const _event& e ){ this->bubbleRefresh( includeThis , _event( e ) ); }
		void bubbleRefresh( bool includeThis = false ){
			this->bubbleRefresh( includeThis , _event::refreshEvent( this->getAbsoluteDimensions() ) );
		}
		
		/**
		 * Method to refresh itself
		 */
		void refreshBitmap(){
			this->handleEvent( refresh );
		}
		
		/**
		 * Receive a Bitmap Port
		 */
		_bitmapPort getBitmapPort(){ return this->bitmap; }
		
		/**
		 * Get The Bitmap of the Gadget
		 */
		_bitmap& getBitmap(){ return this->bitmap; }
		
		/**
		 * Get the style of that Gadget
		 */
		const _style& getStyle() const { return this->style; }
		
		/**
		 * Set the style of that Gadget
		 */
		void setStyle( _style&& style ){ this->style = (_style&&)style; this->triggerEvent( onStyleSet ); }
		
		/**
		 * Returns the Toppest Parent, which is usually the Screen/Windows itself
		 */
		noinline _gadgetScreen* getScreen() const 
		{
			if( this->parent != nullptr )
				return this->parent->getScreen();
			if( this->type == _gadgetType::screen )
				return (_gadgetScreen*)this;
			return nullptr;
		}
		
		
		/**
		 * Register an internal Event Handler to catch some events thrown on this Gadget
		 * This method should only be called in system-internal routines
		 */
		template<typename T>
		noinline void setInternalEventHandler( _eventType type , T&& handler )
		{
			typedef typename T::_callback def;
			// Remove any Current Handler
			_callback* &data = this->eventHandlers[type]; // reference to pointer
			
			if( data )
				delete data; // Delete Current Event-Handler
			
			// Insert The Handler
			data = new T( move(handler) );
		}
		
		/**
		 * Register an user-specific Event Handler to catch some events thrown on this Gadget
		 */
		template<typename T>
		noinline void setUserEventHandler( _eventType type , T&& handler )
		{
			typedef typename T::_callback def;
			
			// Remove any Current Handler
			_callback* &data = this->eventHandlers[ eventType2userET(type) ]; // reference to pointer
			
			if( data )
				delete data; // Delete Current Event-Handler
			
			// Insert The Handler
			data = new T( move(handler) );
		}
		
		/**
		 * Unbind an EventHandler from this Gadget
		 * that was registered by default
		 */
		void removeInternalEventHandler( _eventType type )
		{
			// Unbind the Handler
			_eventHandlerMap::iterator data = this->eventHandlers.find( type );
			
			if( data != this->eventHandlers.end() && data->second )
			{
				delete data->second;
				this->eventHandlers.erase( data );
			}
		}
		
		/**
		 * Unbind an EventHandler from this Gadget
		 * that was registered by the user
		 */
		void removeUserEventHandler( _eventType type )
		{
			// Unbind the Handler
			_eventHandlerMap::iterator data = this->eventHandlers.find( eventType2userET(type) );
			
			if( data != this->eventHandlers.end() && data->second )
			{
				delete data->second;
				this->eventHandlers.erase( data );
			}
		}
		
		/**
		 * Trigger an Event (its destination will be set automatically)
		 */
		void triggerEvent( _event&& event );
		void triggerEvent( const _event& event ){ this->triggerEvent( _event( event ) ); }
		
		/**
		 * Check if this Gadget has an EventHandler registered reacting on the specified type
		 */
		bool canReactTo( _eventType type ) const { return this->eventHandlers.count( type ); }
		
		/**
		 * Make The Gadget act onto a specific GadgetEvent
		 */
		_callbackReturn handleEvent( _event&& event , bool noDefault = false ) ITCM_CODE ; // For rvalues (like handleEvent( onBlur ) )
		_callbackReturn handleEvent( const _event& event , bool noDefault = false ){ return this->handleEvent( _event( event ) /* Make copy */ , noDefault ); } // for variables like '_event e; g->handleEvent( e );'
		
		/**
		 * Make The Gadget act onto a specific GadgetEvent
		 * by only using user-registered event-handler if available
		 */
		_callbackReturn handleEventUser( _event&& event ); // For rvalues (like handleEvent( onBlur ) )
		_callbackReturn handleEventUser( const _event& event ){ return this->handleEventUser( _event( event ) /* Make copy */ ); } // for variables like '_event e; g->handleEventUser( e );'
		
		/**
		 * Make The Gadget act onto a specific GadgetEvent
		 * by only using user-registered event-handler if available
		 */
		_callbackReturn handleEventInternal( _event&& event ); // For rvalues (like handleEvent( onBlur ) )
		_callbackReturn handleEventInternal( const _event& event ){ return this->handleEventInternal( _event( event ) /* Make copy */ ); } // for variables like '_event e; g->handleEventInternal( e );'
		
		/**
		 * Make The Gadget act onto a specific GadgetEvent
		 * by only using the Default gadget-event-handler if available
		 */
		_callbackReturn handleEventDefault( _event&& event ); 
		_callbackReturn handleEventDefault( const _event& event ){ return this->handleEventDefault( _event( event ) /* Make copy */ ); } // for variables like '_event e; g->handleEventDefault( e );'
		
		/**
		 * Get the absolute X-position
		 */
		noinline _coord getAbsoluteX() const
		{
			if( this->parent != nullptr )
				return this->parent->getAbsoluteX() + this->x + ( this->isEnhanced() ? 0 : this->parent->getPadding().left );
			
			return this->x;
		}
		
		/**
		 * Get the absolute Y-position
		 */
		noinline _coord getAbsoluteY() const
		{
			if( this->parent != nullptr )
				return this->parent->getAbsoluteY() + this->y + ( this->isEnhanced() ? 0 : this->parent->getPadding().top );
			
			return this->y;
		}
		
		/**
		 * Get both y and y coordinate
		 */
		_2s32 getAbsolutePosition() const ITCM_CODE;
		
		/**
		 * Get the Relative X-position
		 */
		_coord getX() const { return this->x; }
		
		/**
		 * Get the Relative Y-position
		 */
		_coord getY() const { return this->y; }
		
		/**
		 * Set the Relative X-Position
		 */
		void setX( _coord val );
		
		/**
		 * Set the Relative Y-Position
		 */
		void setY( _coord val );
		
		/**
		 * Hide the Gadget
		 */
		void hide()
		{
			if( !this->hidden )
			{
				this->hidden = true;
				this->bubbleRefresh();
			}
		}
		
		/**
		 * Unhide the Gadget
		 */
		void show()
		{
			if( this->hidden )
			{
				this->hidden = false;
				this->bubbleRefresh();
			}
		}
		
		/**
		 * Minimize the Gadget
		 */
		void minimize();
		
		/**
		 * Restore the Gadget
		 */
		void restore();
		
		/**
		 * Maximize the Gadget
		 */
		void maximize();
		
		/**
		 * unMaximize (Restore) the Gadget
		 */
		void unMaximize();
		
		/**
		 * Get the Gadgets Parent
		 */
		_gadget* getParent() const { return this->parent; }
		
		/**
		 * Set the Gadgets Parent
		 */
		void setParent( _gadget* val );
		void enhanceToParent( _gadget* val );
		
		/**
		 * Remove a specific children
		 */
		virtual void removeChild( _gadget* child );
		virtual void removeChildren( bool remove = false );
		virtual void removeEnhancedChildren( bool remove = false );
		
		/**
		 * Add a child-gadget to this one
		 */
		virtual void addChild( _gadget* child );
		virtual void addEnhancedChild( _gadget* child );
		
		// Tries to focus a child and returns whether it succeded
		/// @param inform whether to also trigger the 'blur'/'focus' event on the gadget
		bool focusChild( _gadget* child );
		bool blurChild();
		
		/**
		 * Get Dimensions of the Gadget (including Coords)
		 */
		_rect getDimensions() const { return this->dimensions; }
		
		/**
		 * Get Absolute Dimensions of the Gadget (including Absolute Coords)
		 */
		_rect getAbsoluteDimensions() const 
		{
			_2s32 val = this->getAbsolutePosition();
			return _rect( val.first , val.second , this->width , this->height );
		}
		
		/**
		 * Get The Size of the Gadget as a _rect being at coordinates {0,0}
		 */
		_rect getSize() const { return _rect( 0 , 0 , this->width , this->height ); }
		
		/**
		 * Set Dimensions of the Gadget (including Coords)
		 */
		virtual void setDimensions( _rect rc );
		
		/**
		 * Get the height of the Gadget
		 */
		_length getHeight() const { return this->height; }
		
		/**
		 * Get the width of the Gadget
		 */
		_length getWidth() const { return this->width; }
		
		/**
		 * Set the gadgets position withing the parents dimensions
		 */
		void moveTo( _coord x , _coord y )
		{
			this->moveRelative( x - this->x , y - this->y );
		}
		
		/**
		 * Move the Gadget relatively to its current position
		 */
		void moveRelative( _s16 deltaX , _s16 deltaY );
		
		/**
		 * Set Height " " "
		 */
		virtual void setHeight( _length val );
		
		/**
		 * Set Width " " "
		 */
		virtual void setWidth( _length val );
		
		/**
		 * Blur the Gadget
		 */
		bool blur()
		{
			if( this->parent && this->parent->focusedChild == this )
				return this->parent->blurChild();
			return this->type == _gadgetType::screen; // You can do everything with a screen!
		}
		
		/**
		 * Focus the Gadget
		 */
		bool focus()
		{
			if( this->parent )
				return this->parent->focusChild( this );
			return this->type == _gadgetType::screen; // You can do everything with a screen!
		}
		
		/**
		 * Get the toppest child owned by the parent
		 */
		_gadget* getToppestChild() const {	return this->children.front(); }
		
		/**
		 * Get the Type of the Gadget (enum)
		 * @see type.h:111
		 */
		_gadgetType getType() const { return this->type; }
		
		/**
		 * Let the gadget blink! This is used if anything can't loose focus
		 */
		void blink();
		
	protected:
		
		/**
		 * There's no public setType, because you shouldn't change the type of a Gadget
		 */
		void setType( _gadgetType type ){ this->type = type; }
	
	
	private: // More attributes!
		
		//! Internal callbacks for removeChildren( true/false )
		static bool		removeDeleteCallback( _gadget* g );
		static bool		removeCallback( _gadget* g );
		
		//! // Used to receive the gadget that a mouse position is in
		_gadget*		getGadgetOfMouseDown( _coord posX , _coord posY );
		
		//! Determine whether a gadget can currently be clicked either because it has focus or because it cannot receive focus at all
		bool			hasClickRights() const ;
		
		//! Let the gadget blink! This is used if anything can't loose focus
		void			blinkHandler(); 
		
		//! _gadget::dragHandler and
		//! _gadget::blinkHandler use this
		union{
			_gadget*	dragTemp;
			int			counter;
		};
		
		//! Type of the Gadget
		_gadgetType 	type;
		
		//! State
		union 
		{
			_u8 state; // used to reset everything quickly
			struct{
				bool hidden : 1;
				bool minimized : 1;
				bool pressed : 1;
				bool enhanced : 1;
				bool dragged : 1;
				bool focused : 1;
				bool maximized : 1;
			} __attribute__(( packed ));
		};
};

inline _gadgetType typeOfGadget( const _gadget* g ){ return g->getType(); }

#endif