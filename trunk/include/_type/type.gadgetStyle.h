#ifndef _WIN_T_GSTYLE_
#define _WIN_T_GSTYLE_


#include "_type/type.h"

/**
 * Specifies the appearence of a gadget
**/
struct _gadgetStyle{
	bool resizable;		/** Is resizable? **/
	bool destroyable;	/** Is destroyable? **/
	bool editable;		/** Is editable? **/
	bool showsUpInTaskBar; /** A Link is added to the taskbar */
	_gadgetStyle();
};

extern _gadgetStyle _defaultStyle_;

#endif