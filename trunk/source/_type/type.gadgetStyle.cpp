#include "_type/type.gadgetStyle.h"

_gadgetStyle::_gadgetStyle() : 
	resizable(false)
	, destroyable(true)
	, editable(true)
	, windowBg( RGB( 29 , 29 , 27 ) )
{ }

_gadgetStyle _defaultStyle_;