#ifndef _WIN_T_GADGETSTYLE_
#define _WIN_T_GADGETSTYLE_


#include "_type/type.h"

/**
 * Specifies the appearence of a gadget
**/
struct _gadgetStyle{
	void* own;
	_u32 data;
	_u8 resizable : 2;		/** Is resizable? 1st bit: X-Direction; 2nd bit: Y-Direction **/
	bool destroyable : 1;	/** Is destroyable? **/
	bool editable : 1;		/** Is editable? **/
	bool showsUpInTaskBar : 1; /** A Link is added to the taskbar */
	
	_gadgetStyle();
	
	static _gadgetStyle owner( void* owner );
	static _gadgetStyle storeData( _u32 data );
	
} __attribute__(( packed )) ;

extern _gadgetStyle _defaultStyle_;

#endif