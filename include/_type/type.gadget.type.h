#ifndef _WIN_T_GADGETTYPE_
#define _WIN_T_GADGETTYPE_

#include "_type/type.h"

/**
 * Specifies the Type of an API-Element (aka _gadget)
 */
enum class _gadgetType : _u8
{
	button,
	stickybutton,
	expandbutton,
	label,
	checkbox,
	radiobox,
	textbox,
	textarea,
	counter,
	clockimage,
	slider,
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
	popup,
	contextmenu,
	contextmenuentry,
	resizehandle,
	none // No type set (is probably not used)
};

extern _map<_gadgetType,string> gadgetType2string;
extern _map<string,_gadgetType> string2gadgetType;

#endif