#ifndef _WIN_T_style_
#define _WIN_T_style_

#include "_type/type.h"

//! Predefine class
class _program;

/**
 * Specifies the appearence of a gadget
 */
class _style
{
	private:
	
		// Internal Helper Class
		template<_u16 bitmask, bool setbits>
		struct _styleAttr{
			template<_u16 t, bool set>
			constexpr _style operator|( _styleAttr<t,set> param ){ return _style() | *this | param; }
		};
		
		//! Cast to _u16
		operator _u16() const { return attrs; }
	
	public:
	
		union
		{
			_u16 attrs;
			struct
			{
				bool	isResizeableX : 1;			/** Is resizable? 1st bit: X-Direction; 2nd bit: Y-Direction  */
				bool	isResizeableY : 1;
				bool	isEnabled : 1;				/** Is enabled? 0 = disabled; 1 = enabled if all parents are also */
				bool	isRightClickable : 1;		/** Provides it a reaction to onMouseRightClick?  */
				bool	takesFocus : 1;				/** Can it blur the focused gadget */
				bool	isBlurrable : 1;			/** Can it be blurred */
				bool	isFocusable : 1;			/** Can it receive focus */
				bool	wantsSmallDragThld : 1; 	/** Wants a small distance after which the gadget will receive a 'dragStart-Event' */
				bool	wantsClickRepeat : 1;		/** Indexes, whether or not keeping the stylus touched down should be interpreted as many clicks */
				bool	isDoubleClickable : 1; 		/** Tells, whether the gadget wants to handle doubleclicks */
				bool	requestsKeyboard : 1; 		/** If this is 'true', the keyboard will pop up and will throw key-events onto this gadget */
				bool	isDraggable : 1; 			/** Is this gadget draggable? */
				bool	doesFocusMoveFront : 1;		/** Whether a focus leads to moving the gadget to the front of adjacent children **/
				bool	isClickable : 1;			/** Whether the user can interact with the gadget by the stylus */
				bool	isDrawnGreyIfDisabled : 1;	/** If set to true, the gadget class will automatically paint the gadget greyscale if it is uneditable */
			}PACKED;
		};
		
		union{
			_int 		val;
			void* 		data;
			_program* 	host;
		};
		
		//! Pass a host
		static _style storeHost( _program* owner , _style attr = _style() );
		
		//! Store an integer in it
		static _style storeInt( _int data , _style attr = _style() );
		
		//! Store any type of data in it
		static _style storeHandle( void* data , _style attr = _style() );
		
		//! Ctors
		_style() :
			attrs( _style(0)
				| resizeable			| enabled
				| notRightClickable		| canTakeFocus
				| blurrable				| focusable
				| defaultDragThld		| noClickRepeat
				| doubleClickable		| noKeyboardRequest
				| notDraggable			| focusNoAction
				| clickable				| drawGreyIfDisabled
			)
			, data( 0 )
		{}
		
		//! Ctor from single _styleAttr
		template<_u16 t, bool set>
		_style( _styleAttr<t,set> param ) :
			attrs( _style() | param )
			, data( 0 )
		{}
		
		//! Ctor by sum
		_style( _u16 sum ) :
			attrs( sum )
			, data( 0 )
		{}
		
		
		//! To concatenate style attributes
		template<_u16 t> constexpr _style operator |( const _styleAttr<t,true> ) const { return _style( attrs | t ); }
		template<_u16 t> constexpr _style operator |( const _styleAttr<t,false> ) const { return _style( attrs & ( _u16(~0) ^ t ) ); }
		template<_u16 t> _style& operator |=( const _styleAttr<t,true> ){ this->attrs |= t; return *this; }
		template<_u16 t> _style& operator |=( const _styleAttr<t,false> ){ this->attrs &= _u16(~0) ^ t; return *this; }
		
		static const _styleAttr<1,true>				resizeableX;
		static const _styleAttr<1,false>			notResizeableX;
		
		static const _styleAttr<2,true>				resizeableY;
		static const _styleAttr<2,false>			notResizeableY;
		
		static const _styleAttr<3,true>				resizeable;
		static const _styleAttr<3,false>			notResizeable;
		
		static const _styleAttr<(1 << 2),true>		enabled;
		static const _styleAttr<(1 << 2),false>		disabled;
		
		static const _styleAttr<(1 << 3),true>		rightClickable;
		static const _styleAttr<(1 << 3),false>		notRightClickable;
		
		static const _styleAttr<(1 << 4),true>		canTakeFocus;
		static const _styleAttr<(1 << 4),false>		canNotTakeFocus;
		
		static const _styleAttr<(1 << 5),true>		blurrable;
		static const _styleAttr<(1 << 5),false>		notBlurrable;
		
		static const _styleAttr<(1 << 6),true>		focusable;
		static const _styleAttr<(1 << 6),false>		notFocusable;
		
		static const _styleAttr<(1 << 7),true>		smallDragThld;
		static const _styleAttr<(1 << 7),false>		defaultDragThld;
		
		static const _styleAttr<(1 << 8),true>		clickRepeat;
		static const _styleAttr<(1 << 8),false> 	noClickRepeat;
		
		static const _styleAttr<(1 << 9),true>		doubleClickable;
		static const _styleAttr<(1 << 9),false>		notDoubleClickable;
		
		static const _styleAttr<(1 << 10),true> 	keyboardRequest;
		static const _styleAttr<(1 << 10),false> 	noKeyboardRequest;

		static const _styleAttr<(1 << 11),true> 	draggable;
		static const _styleAttr<(1 << 11),false>	notDraggable;
		
		static const _styleAttr<(1 << 12),true>		focusMovesFront;
		static const _styleAttr<(1 << 12),false>	focusNoAction;
		
		static const _styleAttr<(1 << 13),true>		clickable;
		static const _styleAttr<(1 << 13),false>	notClickable;
		
		static const _styleAttr<(1 << 14),true>		drawGreyIfDisabled;
		static const _styleAttr<(1 << 14),false>	customDrawIfDisabled;
};

extern void applyString2style( _style& attr , string input );

#endif
