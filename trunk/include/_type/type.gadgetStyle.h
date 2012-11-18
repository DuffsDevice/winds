#ifndef _WIN_T_style_
#define _WIN_T_style_


#include "_type/type.h"

class _gadget;

/**
 * Specifies the appearence of a gadget
**/
struct _style{
	friend class _gadget;
	void* own;
	_int data;
	_u8 resizable : 2;		/** Is resizable? 1st bit: X-Direction; 2nd bit: Y-Direction **/
	bool destroyable : 1;	/** Is destroyable? **/
	bool focused : 1;
	bool pressed : 1;
	private:
	bool enhanced : 1;
	public:
	bool dragged : 1;
	bool draggable : 1;
	bool editable : 1;		/** Is editable? **/
	bool showsUpInTaskBar : 1; /** A Link is added to the taskbar */
	bool canTakeFocus : 1;  /** Can blur the focused gadget */
	bool canLooseFocus : 1; /** Can be blurred */
	bool canReceiveFocus : 1; /** Can receive focus */
	bool smallDragTrig : 1;
	
	//! 8bytes and 15 bits!
	
	static _style owner( void* owner );
	static _style storeData( _int data );
	
	_style();
	
} __attribute__(( packed )) ;

#endif
