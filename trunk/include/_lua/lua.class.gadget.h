// Check if already included
#ifndef _WIN_L_GADGET_
#define _WIN_L_GADGET_

#include "_lua/lua.lunar.h"
#include "_type/type.gadget.h"
	
#define GADGET_BASE_ATTR \
	{ "base" , &_lua_gadget::baseFunc , nullptr } \


/**
 * Proxy Classes
 */
class _lua_gadget{
	
	private:
	
		_gadget* gadget;
		bool wasAllocated;
		
	public:
		
		//! C-Ctor
		_lua_gadget( _gadget* g = nullptr , bool wasAllocated = true );
		
		//! Lua-Ctor
		_lua_gadget( lua_State* L );
		
		//! Dtor
		virtual ~_lua_gadget();
		
		
		//! Set Gadget to contain
		void setGadget( _gadget* g , bool wasAllocated = true );
		
		//! Get underlying _gadget instance
		_gadget* getGadget(){ return this->gadget; }
		operator _gadget&(){ return *this->gadget; }
		
		//! Get the internal _gadget pointer casted to a derived class of _gadget
		template<typename T>
		operator T&()
		{
			typedef typename T::_gadget def;
			return * static_cast<T*>(this->gadget);
		}
		
		//! base
		int baseFunc( lua_State* L );
		
		
		//! redraw
		int redraw(lua_State* L);
		
		//! redrawParents
		int redrawParents(lua_State* L);
		
		//! update
		int update( lua_State* );
		
		//! updateNow
		int updateNow( lua_State* );
		
		//! hasFocus
		int hasFocus( lua_State* );
		
		//! applyStyle
		int applyStyle( lua_State* );
		
		//! getBitmap
		int getBitmap( lua_State* L );
		
		//! setBitmap
		int setBitmap( lua_State* L );
		
		//! getBitmapPort
		int getBitmapPort( lua_State* L );
		
		//! getScreen
		int getScreen( lua_State* L );
		
		//! setUserEventHandler
		int setUserEventHandler( lua_State* L );
		
		//! setInternalEventHandler
		int setInternalEventHandler( lua_State* L );
		
		//! removeUserHandler
		int removeUserEventHandler( lua_State* L );
		
		//! removeInternalHandler
		int removeInternalEventHandler( lua_State* L );
		
		//! populateEvent
		int populateEvent(lua_State* L);
		
		//! triggerEvent
		int triggerEvent(lua_State* L);
		
		//! triggerEventUser
		int triggerEventUser( lua_State* L );
		
		//! triggerEventInternal
		int triggerEventInternal( lua_State* L );
		
		//! triggerEventDefault
		int triggerEventDefault( lua_State* L );
		
		//! canReactTo
		int canReactTo( lua_State* L );
		
		//! handleEvent
		int handleEvent( lua_State* L );
		
		//! handleEventUser
		int handleEventUser( lua_State* L );
		
		//! handleEventInternal
		int handleEventInternal( lua_State* L );
		
		//! handleEventDefault
		int handleEventDefault( lua_State* L );
		
		//! Get absolute X
		int getAbsoluteX(lua_State* L);
		
		//! Get absolute Y
		int getAbsoluteY(lua_State* L);
		
		//! Get absolute Position
		int getAbsolutePosition(lua_State* L);
		
		//! Get X
		int getX(lua_State* L);
		
		//! Get Y
		int getY(lua_State* L);
		
		//! Set X
		int setX(lua_State* L);
		
		//! setXIfAuto
		int setXIfAuto(lua_State* L);
		
		//! Set Y
		int setY(lua_State* L);
		
		//! setYIfAuto
		int setYIfAuto(lua_State* L);
		
		//! request auto X
		int requestAutoX(lua_State* L);
		
		//! request auto Y
		int requestAutoY(lua_State* L);
		
		//! hasAutoX
		int hasAutoX(lua_State* L);
		
		//! hasAutoY
		int hasAutoY(lua_State* L);
		
		//! moveToIfAuto
		int moveToIfAuto(lua_State* L);
		
		//! moveTo
		int moveTo(lua_State* L);
		
		//! moveRelative
		int moveRelative(lua_State* L);
		
		//! moveRelativeIfAuto
		int moveRelativeIfAuto(lua_State* L);
		
		//! requestAutoPosition
		int requestAutoPosition(lua_State* L);
		
		//! getParent
		int getParent( lua_State* L );
		
		//! setParent
		int setParent( lua_State* L );
		
		//! getPrecedentChild
		int getPrecedentChild( lua_State* L );
		
		//! getSubcedentChild
		int getSubcedentChild( lua_State* L );
		
		//! getToppestEnhancedChild
		int getToppestEnhancedChild( lua_State* L );
		
		//! getToppestChild
		int getToppestChild( lua_State* L );
		
		//! getLowestChild
		int getLowestChild( lua_State* L );
		
		//! enhanceToParent
		int enhanceToParent( lua_State* L );
		
		//! removeChild
		int removeChild( lua_State* L );
		
		//! removeChildren
		int removeChildren( lua_State* L );
		
		//! removeEnhancedChildren
		int removeEnhancedChildren(lua_State* L);
		
		//! addChild
		int addChild( lua_State* L );
		
		//! addEnhancedChild
		int addEnhancedChild( lua_State* L );
		
		//! getDimensions
		int getDimensions( lua_State* L );
		
		//! getAbsoluteDimensions
		int getAbsoluteDimensions( lua_State* L );
		
		//! setDimensions
		int setDimensions( lua_State* L );
		
		//! setDimensionsIfAuto
		int setDimensionsIfAuto( lua_State* L );
		
		//! requestAutoDimensions
		int requestAutoDimensions( lua_State* L );
		
		//! setPadding
		int setPadding( lua_State* L );
		
		//! getPadding
		int getPadding( lua_State* L );
		
		//! isEnhanced
		int isEnhanced( lua_State* L );
		
		//! isPressed
		int isPressed( lua_State* L );
		
		//! isHidden
		int isHidden( lua_State* L );
		
		//! isResizeable
		int isResizeable( lua_State* L );
		
		//! isResizeableX
		int isResizeableX( lua_State* L );
		
		//! isResizeableY
		int isResizeableY( lua_State* L );
		
		//! isChildDragged
		int isChildDragged( lua_State* L );
		
		//! isEnabled
		int isEnabled( lua_State* L );
		
		//! isClickable
		int isClickable( lua_State* L );
		
		//! isDraggable
		int isDraggable( lua_State* L );
		
		//! hasSmallDragTrig
		int hasSmallDragTrig( lua_State* L );
		
		//! isMouseClickRepeat
		int isMouseClickRepeat( lua_State* L );
		
		//! isDoubleClickable
		int isDoubleClickable( lua_State* L );
		
		//! requestsKeyboard
		int requestsKeyboard( lua_State* L );
		
		//! getClientRect
		int getClientRect(lua_State* L);
		
		//! setSize
		int setSize(lua_State* L);
		
		//! setSizeIfAuto
		int setSizeIfAuto(lua_State* L);
		
		//! getSize
		int getSize( lua_State* L );
		
		//! getSizeRect
		int getSizeRect( lua_State* L );
		
		//! requestAutoSize
		int requestAutoSize( lua_State* L );
		
		//! getHeight
		int getHeight( lua_State* L );
		
		//! getWidth
		int getWidth( lua_State* L );
		
		//! getMinHeight
		int getMinHeight( lua_State* L );
		
		//! getMinWidth
		int getMinWidth( lua_State* L );
		
		//! requestAutoWidth
		int requestAutoWidth( lua_State* L );
		
		//! requestAutoHeight
		int requestAutoHeight( lua_State* L );
		
		//! hasAutoWidth
		int hasAutoWidth( lua_State* L );
		
		//! hasAutoHeight
		int hasAutoHeight( lua_State* L );
		
		//! setWidth
		int setWidth(lua_State* L);
		
		//! setHeight
		int setHeight(lua_State* L);
		
		//! setWidthIfAuto
		int setWidthIfAuto(lua_State* L);
		
		//! setHeightIfAuto
		int setHeightIfAuto(lua_State* L);
		
		//! setMinWidth
		int setMinWidth(lua_State* L);
		
		//! setMinHeight
		int setMinHeight(lua_State* L);
		
		//! hide
		int hide( lua_State* L );
		
		//! show
		int show( lua_State* L );
		
		//! focus
		int focus( lua_State* L );
		
		//! blur
		int blur( lua_State* L );
		
		//! focusChild
		int focusChild( lua_State* L );
		
		//! blurChild
		int blurChild( lua_State* L );
		
		//! blink
		int blink( lua_State* L );
		
		//! getType
		int getType( lua_State* L );
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_gadget>::FunctionType methods[];
		static Lunar<_lua_gadget>::PropertyType properties[];
};

#endif