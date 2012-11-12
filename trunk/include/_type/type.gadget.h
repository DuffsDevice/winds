// Check if already included
#ifndef _WIN_T_GADGET_
#define _WIN_T_GADGET_

#include "_type/type.bitmap.h"
#include "_type/type.gadgetStyle.h"
#include "_type/type.bitmapPort.h"
#include "_type/type.event.h"

//! For the Lua-Part of the gadget
#include "_lua/lua.hpp"

// STD-Standards
#include <nds/memory.h>

class _gadget;

extern _callbackReturn lua_callEventHandler( lua_State* L , int handler , _event e );

// _gagdetEventHandler
typedef _callbackReturn (*_gadgetEventHandler)(_event);

// _gadgetList
typedef _list<_gadget*> _gadgetList;

// _gadgetDefaultEventHandler
typedef _callbackReturn (*_gadgetDefaultEventHandler)(_event&);

class _gadget{
	
	private:
	
		// Type of the Gadget
		_gadgetType 	type;
		
		
		// Internal
		static bool 	removeEnhancedCallback( _gadget* g )
		{
			if( g->style.enhanced )
				return false;
			
			return _gadget::removeCallback( g );
		}
		static bool 	removeCallback( _gadget* g )
		{
			if( g->style.focused && g->parent )
				g->parent->blurChild();
			
			g->parent = nullptr;
			return true;
		}
	
	public:
	
		// Attributes
		_padding		padding;
		_rect 			dimensions;
		_style 	style;
		
		// Children
		_gadgetList		children;
		_gadget*		focusedChild;
		
		_area			dirtyRects;
		
		// Parent
		_gadget*		parent;
		
		// Only for the dragHandler
		_gadget*		dragTemp;
		
		// Event-Handlers
		_map<_eventType,pair<int,lua_State*>> 				luaEventHandlers;
		_map<_eventType,_gadgetEventHandler> 				eventHandlers;
		static _map<_eventType,_gadgetDefaultEventHandler> defaultEventHandlers;
		
		// Standard EventHandler
		static _callbackReturn	gadgetRefreshHandler( _event& event );
		static _callbackReturn	gadgetDragHandler( _event& event ) ITCM_CODE ;
		static _callbackReturn	gadgetMouseHandler( _event& event ) ITCM_CODE ;
		static _callbackReturn	gadgetFocusHandler( _event& event ) ITCM_CODE ;
		static _callbackReturn	gadgetKeyHandler( _event& event ) ITCM_CODE ;
		
		// Bitmap of the Gadget
		_bitmap*		bitmap;
	
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
		void setPadding( _padding p ){ this->padding = p; }
		
		/**
		 * Get the Padding of the Gadget
		**/
		_padding getPadding(){ return this->padding; }
		
		/**
		 * Check whether this Gadget can also be on the reserved area of the parent
		**/
		bool isEnhanced() const { return this->style.enhanced; }
		
		/**
		 * Set whether this Gadget can also be on the reserved area of the parent
		**/
		void setEnhanced( bool flag = true ){ this->style.enhanced = flag; }
		
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
		void refreshBitmap();
		
		/**
		 * Receive a Bitmap Port
		**/
		_bitmapPort getBitmapPort() const { return this->bitmap; }
		
		/**
		 * Get The Bitmap of the Gadget
		**/
		_bitmap* getBitmap() const { return this->bitmap; }
		
		/**
		 * Get the style of that Gadget
		**/
		_style getStyle() const { return this->style; }
		
		/**
		 * Set the style of that Gadget
		**/
		void setStyle( _style style ){ this->style = style; /*this->triggerEvent( _event( refresh ) );*/ }
		
		/**
		 * Returns the Toppest Parent, which is usually the Screen/Windows itself
		**/
		_gadget* getScreen();
		
		/**
		 * Method to check whether the Gadget has Focus
		**/
		bool hasFocus(){ return this->style.focused; }
		
		/**
		 * Register a Event Handler to catch some events thrown on this Gadget
		**/
		void registerEventHandler( _eventType type , _gadgetEventHandler handler ){
			// Insert The Handler no matter if there was already one acting to that eventType
			this->eventHandlers[type] = handler;
		}
		
		/**
		 * Register a Lua Event Handler to catch some events thrown on this Gadget
		**/
		void registerLuaEventHandler( _eventType type , int handler , lua_State* L ){
			// Insert The Handler no matter if there was already one acting to that eventType
			this->luaEventHandlers[type] = make_pair( handler , L );
		}
		
		/**
		 * Register a Default Event Handler to catch 
		 * some events thrown on this Gadget and not handled by 
		 * any other EventHandlers
		**/
		static void registerDefaultEventHandler( _eventType type , _gadgetDefaultEventHandler handler ){
			// Insert The Handler no matter if there was already one acting to that eventType
			_gadget::defaultEventHandlers[type] = handler;
		}
		
		/**
		 * Unbind an EventHandler from this Gadget
		**/
		void unregisterEventHandler( _eventType type ){
			// Unbind the Handler
			this->eventHandlers.erase( type );
		}
		
		/**
		 * Unbind a LuaEventHandler from this Gadget
		**/
		int unregisterLuaEventHandler( _eventType type );
		
		/**
		 * Method to push an event onto the stack
		**/
		void generateEvent( _event event );
		
		/**
		 * Trigger an Event (its destination will be set automatically)
		**/
		void triggerEvent( _event event );
		
		/**
		 * Check if this Gadget has an EventHandler registered reacting on the specified type
		**/
		bool canReactTo( _eventType type ) const { return this->eventHandlers.count( type ) || this->luaEventHandlers.count( type ); }
		
		/**
		 * Make The Gadget act onto a specific GadgetEvent
		**/
		_callbackReturn handleEvent( _event event ) ;
		
		/**
		 * Make The Gadget act onto a specific GadgetEvent by using the Normal C++ event-handler if available
		**/
		_callbackReturn handleEventNormal( _event event ) ;
		
		/**
		 * Make The Gadget act onto a specific GadgetEvent by using the Default event-handler if available
		**/
		_callbackReturn handleEventDefault( _event& event ) ;
		
		/**
		 * Get the absolute X-position
		**/
		_coord getAbsoluteX() const ;
		
		/**
		 * Get the absolute Y-position
		**/
		_coord getAbsoluteY() const ;
		
		/**
		 * Get the Relative X-position
		**/
		_coord getX() const { return this->dimensions.x; }
		
		/**
		 * Get the Relative Y-position
		**/
		_coord getY() const { return this->dimensions.y; }
		
		/**
		 * Set the Relative X-Position
		**/
		void setX( _coord val );
		
		/**
		 * Set the Relative Y-Position
		**/
		void setY( _coord val );
		
		/**
		 * Get the Gadgets Parent
		**/
		_gadget* getParent() const { return this->parent; }
		
		/**
		 * Set the Gadgets Parent
		**/
		void setParent( _gadget* val );
		
		/**
		 * Remove a specific children
		**/
		virtual void removeChild( _gadget* child );
		virtual void removeChildren( bool preserveEnhanced = false , bool remove = false );
		
		/**
		 * Add a child-gadget to this one
		**/
		virtual void addChild( _gadget* child );
		
		// Tries to focus a child and returns whether it succeded
		/// @param inform whether to also trigger the 'blur'/'focus' event on the gadget
		bool focusChild( _gadget* child );
		bool blurChild();
		
		/**
		 * Get Dimensions of the Gadget (including Coords)
		**/
		_rect getDimensions() const { return this->dimensions; }
		
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
		void setDimensions( _rect rc );
		
		/**
		 * Get the height of the Gadget
		**/
		_length getHeight() const { return this->dimensions.height; }
		
		/**
		 * Get the width of the Gadget
		**/
		_length getWidth() const { return this->dimensions.width; }
		
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
		void setHeight( _length val );
		
		/**
		 * Set Width " " "
		**/
		void setWidth( _length val );
		
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
		 * There's no setType, because you can't change the type of a Gadget
		**/
};

inline _gadgetType typeOfGadget( _gadget* g );

#endif