#include "_lua/lua.gadget.slider.h"
#include "_lua/lua.funcs.h"
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
			this->setGadget( new _slider( check<int>( L , 1 ) , check<int>( L , 2 ) , check<int>( L , 3 ) , lightcheck<int>( L , 4 , 0 ) , lightcheck<_dimension>( L , 5 , _dimension::horizontal ) , lightcheck<_style>( L , 6 , _style() ) ) );
			break;
		case 6:
			if( lua_isstring( L , 6 ) ) // Check if 6th argument is a style
			{
				this->setGadget( new _slider( check<int>( L , 1 ) , check<int>( L , 2 ) , check<int>( L , 3 ) , lightcheck<int>( L , 4 , 0 ) , lightcheck<_dimension>( L , 5 , _dimension::horizontal ) , lightcheck<_style>( L , 6 , _style() ) ) );
				break;
			}
		case 7:
		case 8:
			//	_slider( _coord x , _coord y , _length sliderLength , _s32 value , _dimension dimension , _s32 upperBound , _s32 lowerBound = 0 , _style&& style = _style() )
			this->setGadget( new _slider( check<int>( L , 1 ) , check<int>( L , 2 ) , check<int>( L , 3 ) , check<bool>( L , 4 ) , check<_dimension>( L , 5 ) , check<int>( L , 6 ) , lightcheck<int>( L , 7 , 0 ) , lightcheck<_style>( L , 6 , _style() ) ) );
			break;
		default:
			check<int>( L , 1 ); // Throw Reasonable error
			check<int>( L , 2 );
			check<int>( L , 3 );
			break;
	}
}

//! setUpperBound
int _lua_slider::setUpperBound( lua_State* L ){ getDerived()->setUpperBound( check<int>( L , 1 ) ); return 0; }

//! setLowerBound
int _lua_slider::setLowerBound( lua_State* L ){ getDerived()->setLowerBound( check<int>( L , 1 ) ); return 0; }

//! getLowerBound
int _lua_slider::getLowerBound( lua_State* L ){ lua_pushnumber( L , getDerived()->getUpperBound() ); return 1; }

//! getUpperBound
int _lua_slider::getUpperBound( lua_State* L ){ lua_pushnumber( L , getDerived()->getLowerBound() ); return 1; }

//! setLowerLabel
int _lua_slider::setLowerLabel( lua_State* L ){ getDerived()->setLowerLabel( check<string>( L , 1 ) ); return 0; }

//! setUpperLabel
int _lua_slider::setUpperLabel( lua_State* L ){ getDerived()->setUpperLabel( check<string>( L , 1 ) ); return 0; }

//! getLowerLabel
int _lua_slider::getLowerLabel( lua_State* L ){ push( L , getDerived()->getLowerLabel() ); return 1; }

//! getUpperLabel
int _lua_slider::getUpperLabel( lua_State* L ){ push( L , getDerived()->getUpperLabel() ); return 1; }

//! setSnap
int _lua_slider::setSnap( lua_State* L ){ getDerived()->setSnap( lightcheck<int>( L , 1 , 0 ) ); return 0; }

//! getSnap
int _lua_slider::getSnap( lua_State* L ){ push( L , getDerived()->getSnap() ); return 1; }

//! increase
int _lua_slider::increase( lua_State* L ){ getDerived()->increase(); return 0; }

//! decrease
int _lua_slider::decrease( lua_State* L ){ getDerived()->decrease(); return 0; }

//! Lua-button
const char _lua_slider::className[] = "Slider";
Lunar<_lua_slider>::FunctionType _lua_slider::methods[] = {
	LUA_CLASS_FUNC(_lua_slider,increase),
	LUA_CLASS_FUNC(_lua_slider,decrease),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_slider>::PropertyType _lua_slider::properties[] = {
	GADGET_BASE_ATTR,
	{ "value" , &_lua_slider::getIntValue , &_lua_slider::setIntValue },
	{ "upperBound" , &_lua_slider::getUpperBound , &_lua_slider::setUpperBound },
	{ "lowerBound" , &_lua_slider::getLowerBound , &_lua_slider::setLowerBound },
	{ "upperLabel" , &_lua_slider::getUpperLabel , &_lua_slider::setUpperBound },
	{ "lowerLabel" , &_lua_slider::getLowerBound , &_lua_slider::setLowerBound },
	{ "snap" , &_lua_slider::getSnap , &_lua_slider::setSnap },
	LUA_CLASS_ATTR_END
};