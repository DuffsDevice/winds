// Check if already included
#ifndef _WIN_T_GADGET_
#define _WIN_T_GADGET_

#include "_type/type.bitmap.h"
#include "_type/type.gadgetStyle.h"
#include "_type/type.bitmapPort.h"
#include "_type/type.event.h"

//! For the Lua-Part of the gadget
#include "_lua/lua.h"
#include "_lua/lauxlib.h"

// STD-Standards
#include <list>
#include <utility>
using namespace std;

#include <nds/memory.h>

class _gadget;

extern _gadgetEventReturnType lua_callEventHandler( lua_State* L , int handler , _gadgetEvent e );

typedef _gadgetEventReturnType (*_gadgetEventHandler)(_gadgetEvent);
typedef list<_gadget*> _gadgetList;
typedef _gadgetEventReturnType (*_gadgetDefaultEventHandler)(_gadgetEvent&);

class _gadget{
	
	private:
	
		// Type of the Gadget
		_gadgetType type;
		
		static bool removeEnhancedCallback( _gadget* g );
		static bool removeCallback( _gadget* g );
	
	public:
	
		// Attributes
		_padding	padding;
		_rect 		dimensions;
		bool 		enhanced;
		bool		focused;
		_gadgetStyle style;
		
		// Children
		_gadgetList	children;
		
		// Parent
		_gadget*	parent;
		
		// Only for the dragHandler
		_gadget*	dragTemp;
		
		// Event-Handlers
		map<_gadgetEventType,_gadgetDefaultEventHandler> defaultEventHandlers;
		map<_gadgetEventType,pair<int,lua_State*>> luaEventHandlers;
		map<_gadgetEventType,_gadgetEventHandler> eventHandlers;
		
		// Standard EventHandler
		static _gadgetEventReturnType	gadgetRefreshHandler( _gadgetEvent& event );
		static _gadgetEventReturnType	gadgetDragHandler( _gadgetEvent& event );
		static _gadgetEventReturnType	gadgetMouseHandler( _gadgetEvent& event );
		static _gadgetEventReturnType	gadgetFocusHandler( _gadgetEvent& event );
		
		// Bitmap of the Gadget
		_bitmap*	bitmap;
		
		// If any Gadget wants to do something on resize,
		// Just implement that one in your class and let it do something
		virtual void onResize(){};
		
		virtual bool blurEventChild();
		virtual bool focusEventChild( _gadget* child );
	
	public:
		
		/**
		 * Constructor
		**/
		_gadget( _gadgetType type , int width , int height , int posX , int posY , _gadgetStyle style = _defaultStyle_ , bool doNotAllocateBitmap = false );
		_gadget( int width , int height , int posX , int posY , _gadgetStyle style = _defaultStyle_ , bool doNotAllocateBitmap = false );
		
		/**
		 * Destructor
		**/
		~_gadget();
		
		/**
		 * Set the Padding of the Gadget
		**/
		void setPadding( _padding p );
		
		/**
		 * Get the Padding of the Gadget
		**/
		_padding getPadding();
		
		/**
		 * Check whether this Gadget can also be on the reserved area of the parent
		**/
		bool isEnhanced() const;
		
		/**
		 * Set whether this Gadget can also be on the reserved area of the parent
		**/
		void setEnhanced( bool flag = true );
		
		/**
		 * Let an Event bubble from child to parent and so on...
		**/
		void bubbleEvent( _gadgetEvent e , bool includeThis = false );
		
		/**
		 * Print Contents but make the parent also refresh
		 * the parts that have been changed
		**/
		void bubbleRefresh( bool includeThis = false , _gadgetEvent e = _gadgetEvent() );
		
		/**
		 * Method to refresh itself
		**/
		void refreshBitmap();
		
		/**
		 * Receive a Bitmap Port
		**/
		_bitmapPort getBitmapPort() const ;
		
		/**
		 * Get The Bitmap of the Gadget
		**/
		_bitmap* getBitmap();
		
		/**
		 * Get the style of that Gadget
		**/
		_gadgetStyle getStyle() const ;
		
		/**
		 * Set the style of that Gadget
		**/
		void setStyle( _gadgetStyle style );
		
		/**
		 * Returns the Toppest Parent, which is usually the Screen/Windows itself
		**/
		_gadget* getScreen();
		
		/**
		 * Method to check whether the Gadget has Focus
		**/
		bool& hasFocus();
		
		/**
		 * Register a Event Handler to catch some events thrown on this Gadget
		**/
		bool registerEventHandler( _gadgetEventType type , _gadgetEventHandler handler );
		
		/**
		 * Register a Lua Event Handler to catch some events thrown on this Gadget
		**/
		bool registerLuaEventHandler( _gadgetEventType type , int handler , lua_State* L );
		
		/**
		 * Register a Default Event Handler to catch 
		 * some events thrown on this Gadget and not handled by 
		 * any other EventHandlers
		**/
		bool registerDefaultEventHandler( _gadgetEventType type , _gadgetDefaultEventHandler handler );
		
		/**
		 * Unbind an EventHandler from this Gadget
		**/
		void unregisterEventHandler( _gadgetEventType type );
		
		/**
		 * Unbind a LuaEventHandler from this Gadget
		**/
		int unregisterLuaEventHandler( _gadgetEventType type );
		
		/**
		 * Method to push an event onto the stack
		**/
		void generateEvent( _gadgetEvent event );
		
		/**
		 * Trigger an Event (its destination will be set automatically)
		**/
		void triggerEvent( _gadgetEvent event );
		
		/**
		 * Check if this Gadget has an EventHandler registered reacting on the specified type
		**/
		bool canReactTo( _gadgetEventType type ) const ;
		
		/**
		 * Make The Gadget act onto a specific GadgetEvent
		**/
		_gadgetEventReturnType handleEvent( _gadgetEvent event );
		
		/**
		 * Make The Gadget act onto a specific GadgetEvent by using the Normal C++ event-handler if available
		**/
		_gadgetEventReturnType handleEventNormal( _gadgetEvent event );
		
		/**
		 * Make The Gadget act onto a specific GadgetEvent by using the Default event-handler if available
		**/
		_gadgetEventReturnType handleEventDefault( _gadgetEvent& event );
		
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
		_coord getX() const ;
		
		/**
		 * Get the Relative Y-position
		**/
		_coord getY() const ;
		
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
		_gadget* getParent() const ;
		
		/**
		 * Set the Gadgets Parent
		**/
		void setParent( _gadget* val );
		void _setParent( _gadget* val );
		
		/**
		 * Remove a specific children
		**/
		virtual void removeChild( _gadget* child );
		virtual void removeChildren( bool preserveEnhanced = false );
		
		/**
		 * Add a child-gadget to this one
		**/
		virtual void addChild( _gadget* child );
		
		// Tries to focus a child and returns whether it succeded
		/// @param inform whether to also trigger the 'blur'/'focus' event on the gadget
		virtual bool focusChild( _gadget* child );
		virtual bool blurChild();
		
		/**
		 * Get Dimensions of the Gadget (including Coords)
		**/
		_rect getDimensions() const ;
		
		/**
		 * Get Absolute Dimensions of the Gadget (including Absolute Coords)
		**/
		_rect getAbsoluteDimensions() const ;
		
		/**
		 * Get The Size of the Gadget as a _rect being at coordinates {0,0}
		**/
		_rect getSize() const;
		
		/**
		 * Set Dimensions of the Gadget (including Coords)
		**/
		void setDimensions( _rect rc );
		
		/**
		 * Get the height of the Gadget
		**/
		_length getHeight() const ;
		
		/**
		 * Get the width of the Gadget
		**/
		_length getWidth() const ;
		
		/**
		 * Move the Gadget relatively to its current position
		**/
		void moveTo( _coord dX , _coord dY ) ITCM_CODE;
		
		/**
		 * Relative moving fo the Gadget
		**/
		void moveRelative( _s16 deltaX , _s16 deltaY ) ITCM_CODE;
		
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
		_gadget* getToppestChild();
		
		/**
		 * Get the Type of the Gadget (enum)
		 * @see type.h:111
		**/
		_gadgetType getType();
		
		/**
		 * There's no setType, because you can't change the type of a Gadget
		**/
};

#endif