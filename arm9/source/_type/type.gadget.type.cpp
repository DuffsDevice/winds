#include <_type/type.gadget.type.h>

// Convert a gadgetType to a string
_toStr<_gadgetType> gadgetType2string = {
	{ _gadgetType::button				, "button" },
	{ _gadgetType::stickybutton			, "stickybutton" },
	{ _gadgetType::expandbutton			, "expandbutton" },
	{ _gadgetType::imagebutton			, "imagebutton" },
	{ _gadgetType::label				, "label" },
	{ _gadgetType::checkbox				, "checkbox" },
	{ _gadgetType::calendar				, "calendar" },
	{ _gadgetType::radiobox				, "radiobox" },
	{ _gadgetType::textbox				, "textbox" },
	{ _gadgetType::textarea				, "textarea" },
	{ _gadgetType::selectbox			, "selectbox" },
	{ _gadgetType::selectitem			, "selectitem" },
	{ _gadgetType::progressbar			, "progressbar" },
	{ _gadgetType::keyboard				, "keyboard" },
	{ _gadgetType::desktop				, "desktop" },
	{ _gadgetType::fileview				, "fileview" },
	{ _gadgetType::filetree				, "filetree" },
	{ _gadgetType::fileobject			, "fileobject" },
	{ _gadgetType::imagegadget			, "imagegadget" },
	{ _gadgetType::scrollarea			, "scrollarea" },
	{ _gadgetType::scrollbutton			, "scrollbutton" },
	{ _gadgetType::scrollbar			, "scrollbar" },
	{ _gadgetType::window				, "window" },
	{ _gadgetType::windowbar			, "windowbar" },
	{ _gadgetType::windowmenu			, "windowmenu" },
	{ _gadgetType::windowmenuentry		, "windowmenuentry" },
	{ _gadgetType::mainframe			, "mainframe" },
	{ _gadgetType::codepointmatrix		, "codepointmatrix" },
	{ _gadgetType::screen				, "screen" },
	{ _gadgetType::popup				, "popup" },
	{ _gadgetType::slider				, "slider" },
	{ _gadgetType::counter				, "counter" },
	{ _gadgetType::hexagoncolorpicker	, "hexagoncolorpicker" },
	{ _gadgetType::gradientcolorpicker	, "gradientcolorpicker" },
	{ _gadgetType::clockgadget			, "clockgadget" },
	{ _gadgetType::contextmenu			, "contextmenu" },
	{ _gadgetType::contextmenuentry		, "contextmenuentry" },
	{ _gadgetType::resizehandle			, "resizehandle" },
	{ _gadgetType::grid					, "grid" },
	{ _gadgetType::griddescriber		, "griddescriber" },
	{ _gadgetType::gridbox				, "gridbox" },
	{ _gadgetType::treeview				, "treeview" },
	{ _gadgetType::divider				, "divider" },
	{ _gadgetType::none					, "none" }
};

_fromStr<_gadgetType> string2gadgetType = {
	{ "button"				, _gadgetType::button },
	{ "stickybutton"		, _gadgetType::stickybutton },
	{ "expandbutton"		, _gadgetType::expandbutton },
	{ "imagebutton"			, _gadgetType::imagebutton },
	{ "label"				, _gadgetType::label },
	{ "checkbox"			, _gadgetType::checkbox },
	{ "calendar"			, _gadgetType::calendar },
	{ "radiobox"			, _gadgetType::radiobox },
	{ "textbox"				, _gadgetType::textbox },
	{ "textarea"			, _gadgetType::textarea },
	{ "selectbox"			, _gadgetType::selectbox },
	{ "selectitem"			, _gadgetType::selectitem },
	{ "progressbar"			, _gadgetType::progressbar },
	{ "keyboard"			, _gadgetType::keyboard },
	{ "desktop"				, _gadgetType::desktop },
	{ "fileview"			, _gadgetType::fileview },
	{ "filetree"			, _gadgetType::filetree },
	{ "fileobject"			, _gadgetType::fileobject },
	{ "imagegadget"			, _gadgetType::imagegadget },
	{ "scrollarea"			, _gadgetType::scrollarea },
	{ "scrollbutton"		, _gadgetType::scrollbutton },
	{ "scrollbar"			, _gadgetType::scrollbar },
	{ "window"				, _gadgetType::window },
	{ "windowbar"			, _gadgetType::windowbar },
	{ "windowmenu"			, _gadgetType::windowmenu },
	{ "windowmenuentry"		, _gadgetType::windowmenuentry },
	{ "mainframe"			, _gadgetType::mainframe },
	{ "codepointmatrix"		, _gadgetType::codepointmatrix },
	{ "screen"				, _gadgetType::screen },
	{ "popup"				, _gadgetType::popup },
	{ "counter"				, _gadgetType::counter },
	{ "hexagoncolorpicker"	, _gadgetType::hexagoncolorpicker },
	{ "gradientcolorpicker"	, _gadgetType::gradientcolorpicker },
	{ "clockgadget"			, _gadgetType::clockgadget },
	{ "slider"				, _gadgetType::slider },
	{ "contextmenu"			, _gadgetType::contextmenu },
	{ "contextmenuentry"	, _gadgetType::contextmenuentry },
	{ "resizehandle"		, _gadgetType::resizehandle },
	{ "grid"				, _gadgetType::grid },
	{ "griddescriber"		, _gadgetType::griddescriber },
	{ "gridbox"				, _gadgetType::gridbox },
	{ "treeview"			, _gadgetType::treeview },
	{ "divider"				, _gadgetType::divider },
	{ "none"				, _gadgetType::none }
};