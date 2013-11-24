#include "_lua/lua.gadget.slider.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*##################################
##            Lua-Slider          ##
##################################*/

_lua_slider::_lua_slider( lua_State* L )
{
	switch( lua_gettop( L ) )
	{
		case 3:
		case 4:
		case 5:
			//	_slider( _coord x , _coord y , _length sliderLength , _s32 value = 0 , _dimension dimension = _dimension::horizontal , _style&& style = _style() )
			this->setGadget( new _slider( optcheck<int>( L , 1 ) , optcheck<int>( L , 2 ) , optcheck<int>( L , 3 ) , lightcheck<int>( L , 4 , 0 ) , lightcheck<_dimension>( L , 5 , _dimension::horizontal ) , lightcheck<_style>( L , 6 ) ) );
			break;
		case 6:
			if( lua_isstring( L , 6 ) ) // Check if 6th argument is a style
			{
				this->setGadget( new _slider( optcheck<int>( L , 1 ) , optcheck<int>( L , 2 ) , optcheck<int>( L , 3 ) , lightcheck<int>( L , 4 , 0 ) , lightcheck<_dimension>( L , 5 , _dimension::horizontal ) , lightcheck<_style>( L , 6 ) ) );
				break;
			}
		case 7:
		case 8:
			//	_slider( _coord x , _coord y , _length sliderLength , _s32 value , _dimension dimension , _s32 upperBound , _s32 lowerBound = 0 , _style&& style = _style() )
			this->setGadget( new _slider( optcheck<int>( L , 1 ) , optcheck<int>( L , 2 ) , optcheck<int>( L , 3 ) , check<bool>( L , 4 ) , check<_dimension>( L , 5 ) , check<int>( L , 6 ) , lightcheck<int>( L , 7 , 0 ) , lightcheck<_style>( L , 6 ) ) );
			break;
		default:
			check<int>( L , 1 ); // Throw Reasonable error
			check<int>( L , 2 );
			check<int>( L , 3 );
			break;
	}
}

//! Lua-button
const char _lua_slider::className[] = "Slider";
Lunar<_lua_slider>::FunctionType _lua_slider::methods[] = {
	{ "increase" , wrap( _lua_slider , &_slider::increase ) },
	{ "decrease" , wrap( _lua_slider , &_slider::decrease ) },
	LUA_CLASS_FUNC_END
};

Lunar<_lua_slider>::PropertyType _lua_slider::properties[] = {
	GADGET_BASE_ATTR,
	{ "value"		, wrap( _lua_slider , &_slider::getIntValue )	, wrap( _lua_slider , &_slider::setIntValue ) },
	{ "upperBound"	, wrap( _lua_slider , &_slider::getUpperBound )	, wrap( _lua_slider , &_slider::setUpperBound ) },
	{ "lowerBound"	, wrap( _lua_slider , &_slider::getLowerBound )	, wrap( _lua_slider , &_slider::setLowerBound ) },
	{ "upperLabel"	, wrap( _lua_slider , &_slider::getUpperLabel )	, wrap( _lua_slider , &_slider::setUpperLabel ) },
	{ "lowerLabel"	, wrap( _lua_slider , &_slider::getLowerLabel )	, wrap( _lua_slider , &_slider::setLowerLabel ) },
	{ "snap"		, wrap( _lua_slider , &_slider::getSnap )		, wrap( _lua_slider , &_slider::setSnap ) },
	LUA_CLASS_ATTR_END
};