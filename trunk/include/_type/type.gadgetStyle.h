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
		
		static const _pStyleAttr<( 1 << 2 )> 		destroyable;
		static const _nStyleAttr<( 1 << 2 )> 		notDestroyable;
		
		static const _pStyleAttr<( 1 << 3 )> 		editable;
		static const _nStyleAttr<( 1 << 3 )> 		notEditable;
		
		static const _pStyleAttr<( 1 << 4 )> 		minimizeable;
		static const _nStyleAttr<( 1 << 4 )> 		notMinimizeable;
		
		static const _pStyleAttr<( 1 << 5 )> 		canTakeFocus;
		static const _nStyleAttr<( 1 << 5 )> 		canNotTakeFocus;
		
		static const _pStyleAttr<( 1 << 6 )> 		canLooseFocus;
		static const _nStyleAttr<( 1 << 6 )> 		canNotLooseFocus;
		
		static const _pStyleAttr<( 1 << 7 )> 		canReceiveFocus;
		static const _nStyleAttr<( 1 << 7 )> 		canNotReceiveFocus;
		
		static const _nStyleAttr<( 1 << 8 )> 		defaultDragTrig;
		static const _pStyleAttr<( 1 << 8 )> 		smallDragTrig;
		
		static const _nStyleAttr<( 1 << 9 )> 		mouseClickDefault;
		static const _pStyleAttr<( 1 << 9 )> 		mouseClickRepeat;
		
		static const _pStyleAttr<( 1 << 10 )> 		doubleClickable;
		static const _nStyleAttr<( 1 << 10 )> 		notDoubleClickable;
};


class _gadget;
class _program;

/**
 * Specifies the appearence of a gadget
**/
struct _style
{
	friend class _gadget;
	
	union
	{
		_u16 attrs;
		struct
		{
			bool	resizeableX : 1;	/** Is resizable? 1st bit: X-Direction; 2nd bit: Y-Direction **/
			bool	resizeableY : 1;
			bool	destroyable : 1;	/** Is destroyable? **/
			bool	editable : 1;		/** Is editable? **/
			bool	minimizeable : 1;	/** A Link is added to the taskbar */
			bool	canTakeFocus : 1;	/** Can blur the focused gadget */
			bool	canLooseFocus : 1;	/** Can be blurred */
			bool	canReceiveFocus : 1;/** Can receive focus */
			bool	smallDragTrig : 1;
			bool	mouseClickRepeat : 1;
			bool	doubleClickable : 1;
			_u8		unused1 : 4;
			_u8		unused2 : 1;
		} __attribute__(( packed )) ;
	};
	
	private:
	// Only for _gadget class
	bool pressed : 1;
	bool dragged : 1;
	bool focused : 1;
	bool enhanced : 1;
	bool visible : 1;
	bool minimized : 1;
	bool maximized : 1;
	public:
	
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
		, pressed( false )
		, dragged( false )
		, focused( false )
		, enhanced( false )
		, visible( true )
		, minimized( false )
		, data( 0 )
	{ }
	
	// Enable _styleAttr-changes also on this struct
	template<int t> constexpr _style operator |( const _pStyleAttr<t> ) const { _style s = *this; s.attrs |= t; return s; }
	template<int t> constexpr _style operator |( const _nStyleAttr<t> ) const { _style s = *this; s.attrs &= ( _u32( 1 << 16 ) - 1 ) ^ t; return s; }
	template<int t> _style& operator |=( const _pStyleAttr<t> ){ attrs |= t; return *this; }
	template<int t> _style& operator |=( const _nStyleAttr<t> ){ attrs &= ( _u32( 1 << 16 ) - 1 ) ^ t; return *this; }
	
} __attribute__(( packed )) ;

extern void applyString2style( _style& attr , string input );

#endif
