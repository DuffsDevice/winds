#ifndef _WIN_T_style_
#define _WIN_T_style_

#include "_type/type.h"


template<int t> class _nStyleAttr{
};

template<int t> class _pStyleAttr {
};

class _styleAttr
{
	private:
		
		_u16 sum;
		
		friend class _style;
		
	public:
		
		// Casts
		operator _u16(){ return sum; }
		
		// This all works completely at compile-time!
		template<int t> constexpr _styleAttr operator |( const _pStyleAttr<t> ) const { _u16 s = sum; s |= t; return _styleAttr( s ); }
		template<int t> constexpr _styleAttr operator |( const _nStyleAttr<t> ) const { _u16 s = sum; s &= ( _u32( 1 << 16 ) - 1 ) ^ t; return _styleAttr( s ); }
		
		// This all works completely at compile-time!
		template<int t> _styleAttr& operator |=( const _pStyleAttr<t> ){ sum |= t; return *this; }
		template<int t> _styleAttr& operator |=( const _nStyleAttr<t> ){ sum &= ( _u32( 1 << 16 ) - 1 ) ^ t; return *this; }
		
		// Default ctor
		_styleAttr();
		
		// Ctor with sum
		_styleAttr( _u16 val ){ sum = val; }
		
		static const _pStyleAttr<1> 				resizeableX;
		static const _nStyleAttr<1> 				notResizeableX;
		
		static const _pStyleAttr<2> 				resizeableY;
		static const _nStyleAttr<2>			 		notResizeableY;
		
		static const _pStyleAttr<3>					resizeable;
		static const _nStyleAttr<3>					notResizeable;
		
		static const _pStyleAttr<( 1 << 2 )> 		enabled;
		static const _nStyleAttr<( 1 << 2 )> 		disabled;
		
		static const _pStyleAttr<( 1 << 3 )> 		clickable;
		static const _nStyleAttr<( 1 << 3 )> 		notClickable;
		
		static const _pStyleAttr<( 1 << 4 )> 		canTakeFocus;
		static const _nStyleAttr<( 1 << 4 )> 		canNotTakeFocus;
		
		static const _pStyleAttr<( 1 << 5 )> 		canLooseFocus;
		static const _nStyleAttr<( 1 << 5 )> 		canNotLooseFocus;
		
		static const _pStyleAttr<( 1 << 6 )> 		canReceiveFocus;
		static const _nStyleAttr<( 1 << 6 )> 		canNotReceiveFocus;
		
		static const _nStyleAttr<( 1 << 7 )> 		defaultDragTrig;
		static const _pStyleAttr<( 1 << 7 )> 		smallDragTrig;
		
		static const _nStyleAttr<( 1 << 8 )> 		mouseClickDefault;
		static const _pStyleAttr<( 1 << 8 )> 		mouseClickRepeat;
		
		static const _pStyleAttr<( 1 << 9 )> 		doubleClickable;
		static const _nStyleAttr<( 1 << 9 )> 		notDoubleClickable;
		
		static const _pStyleAttr<( 1 << 10 )> 		keyboardRequest;
		static const _nStyleAttr<( 1 << 10 )> 		noKeyboardRequest;
		
		static const _pStyleAttr<( 1 << 11 )> 		draggable;
		static const _nStyleAttr<( 1 << 11 )> 		notDraggable;
		
		static const _pStyleAttr<( 1 << 12 )> 		focusBringsFront;
		static const _nStyleAttr<( 1 << 12 )> 		focusNoAction;
};

//! Predefine class
class _program;

/**
 * Specifies the appearence of a gadget
 */
struct _style
{
	union
	{
		_u16 attrs;
		struct
		{
			bool	resizeableX : 1;		/** Is resizable? 1st bit: X-Direction; 2nd bit: Y-Direction  */
			bool	resizeableY : 1;
			bool	enabled : 1;			/** Is editable */
			bool	clickable : 1;			/** Is clickable?  */
			bool	canTakeFocus : 1;		/** Can blur the focused gadget */
			bool	canLooseFocus : 1;		/** Can be blurred */
			bool	canReceiveFocus : 1;	/** Can receive focus */
			bool	smallDragTrig : 1; 		/** Wants a small distance after which the gadget will receive a 'dragStart-Event' */
			bool	mouseClickRepeat : 1;	/** Indexes, whether or not keeping the stylus touched down should be interpreted as many clicks */
			bool	doubleClickable : 1; 	/** Tells, whether the gadget wants to handle doubleclicks */
			bool	keyboardRequest : 1; 	/** If this is 'true', the keyboard will pop up and will throw key-events onto this gadget */
			bool	draggable : 1; 			/** Is this gadget draggable? */
			bool	focusBringsFront : 1;	/** Whether a focus leads to bringing the gadget to the front of adjacent children **/
		} __attribute__(( packed )) ;
	};
	
	union
	{
		_int 		val;
		void* 		data;
		_program* 	host;
	};
	
	//! Pass a host
	static _style storeHost( _program* owner , _styleAttr attr = _styleAttr() );
	
	//! Store an integer in it
	static _style storeInt( _int data , _styleAttr attr = _styleAttr() );
	
	//! Store any type of data in it
	static _style storeHandle( void* data , _styleAttr attr = _styleAttr() );
	
	//! Ctor with attributes
	_style( _styleAttr sA = _styleAttr() ) :
		attrs( sA )
		, data( 0 )
	{ }
	
	// Enable _styleAttr-changes also on this struct
	template<int t> constexpr _style operator |( const _pStyleAttr<t> ) const { _style s = *this; s.attrs |= t; return s; }
	template<int t> constexpr _style operator |( const _nStyleAttr<t> ) const { _style s = *this; s.attrs &= ( _u32( 1 << 16 ) - 1 ) ^ t; return s; }
	template<int t> _style& operator |=( const _pStyleAttr<t> ){ attrs |= t; return *this; }
	template<int t> _style& operator |=( const _nStyleAttr<t> ){ attrs &= ( _u32( 1 << 16 ) - 1 ) ^ t; return *this; }
	
};

extern void applyString2style( _style& attr , string input );

#endif
