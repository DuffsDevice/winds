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
	windowbar,
	windowmenu,
	windowmenuentry,
	screen,
	popup,
	contextmenu,
	contextmenuentry,
	resizehandle,
	grid,
	griddescriber,
	gridbox,
	none // No type set (is probably not used)
};

extern _toStr<_gadgetType>		gadgetType2string;
extern _fromStr<_gadgetType>	string2gadgetType;

#endif