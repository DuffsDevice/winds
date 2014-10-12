#include <_lua/lua.func.h>
#include <_lua/lua.lunar.h>
#include <_lua/lua.class.event.h>
#include <_lua/lua.class.rect.h>
#include <_lua/lua.class.area.h>
#include <_lua/lua.class.font.h>
#include <_lua/lua.class.color.h>
#include <_lua/lua.class.border.h>
#include <_lua/lua.class.bitmap.h>
#include <_lua/lua.class.bitmap.port.h>
#include <_lua/lua.class.hardwarekeypattern.h>
#include <_lua/lua.class.time.h>
#include <_lua/lua.class.mime.h>
#include <_lua/lua.class.menu.h>
#include <_lua/lua.gadget.window.h>
#include <_lua/lua.gadget.window.mainframe.h>
#include <_lua/lua.gadget.window.bar.h>
#include <_lua/lua.gadget.window.menu.h>
#include <_lua/lua.gadget.button.h>
#include <_lua/lua.gadget.calendar.h>
#include <_lua/lua.gadget.label.h>
#include <_lua/lua.gadget.contextmenu.h>
#include <_lua/lua.gadget.popup.h>
#include <_lua/lua.gadget.progressbar.h>
#include <_lua/lua.gadget.resizehandle.h>
#include <_lua/lua.gadget.counter.h>
#include <_lua/lua.gadget.colorpicker.gradient.h>
#include <_lua/lua.gadget.colorpicker.hexagon.h>
#include <_lua/lua.gadget.checkbox.h>
#include <_lua/lua.gadget.clock.h>
#include <_lua/lua.gadget.button.image.h>
#include <_lua/lua.gadget.image.h>
#include <_lua/lua.gadget.scrollArea.h>
#include <_lua/lua.gadget.scrollBar.h>
#include <_lua/lua.gadget.button.sticky.h>
#include <_lua/lua.gadget.taskinfo.h>
#include <_lua/lua.gadget.textbox.h>
#include <_lua/lua.gadget.textarea.h>
#include <_lua/lua.gadget.select.h>
#include <_lua/lua.gadget.radio.h>
#include <_lua/lua.gadget.slider.h>

namespace _luafunc
{
	void pushEvent( lua_State* L , _event&& arg ){
		Lunar<_lua_event>::emplace( L , move(arg) );
	}
	
	void pushRect( lua_State* L , _rect&& arg ){
		Lunar<_lua_rect>::emplace( L , move(arg) );
	}
	
	void pushArea( lua_State* L , _area&& arg ){
		Lunar<_lua_area>::emplace( L , move(arg) );
	}
	
	void pushBitmapRef( lua_State* L , _bitmap& arg ){
		Lunar<_lua_bitmap>::push( L , new _lua_bitmap(arg) );
	}
	
	void pushBitmap( lua_State* L , _bitmap&& arg ){
		Lunar<_lua_bitmap>::push( L , new _lua_bitmap( move(arg) ) );
	}
	
	void pushBitmapPort( lua_State* L , _bitmapPort&& arg ){
		Lunar<_lua_bitmapport>::emplace( L , move(arg) );
	}
	
	void pushBorder( lua_State* L , _border&& arg ){
		Lunar<_lua_border>::emplace( L , move(arg) );
	}
	
	void pushFont( lua_State* L , _fontHandle arg ){
		Lunar<_lua_font>::emplace( L , arg );
	}
	
	void pushTime( lua_State* L , _time&& arg ){
		Lunar<_lua_time>::emplace( L , move(arg) );
	}
	
	void pushColor( lua_State* L , _color&& arg ){
		Lunar<_lua_color>::emplace( L , move(arg) );
	}
	
	void pushHWKP( lua_State* L , _hardwareKeyPattern&& arg ){
		Lunar<_lua_hardwarekeypattern>::emplace( L , move(arg) );
	}
	
	void pushMimeType( lua_State* L , _mimeType&& arg ){
		Lunar<_lua_mimetype>::emplace( L , move(arg) );
	}
	
	void pushMenu( lua_State* L , _menu&& arg ){
		Lunar<_lua_menu>::push( L , new _lua_menu( move(arg) ) );
	}
	
	void pushMenuRef( lua_State* L , _menu& arg ){
		Lunar<_lua_menu>::push( L , new _lua_menu( arg ) );
	}
	
	void pushGadget( lua_State* L , _gadget* gadget )
	{
		if( !gadget )
		{
			lua_pushnil( L );
			return;
		}
		
		switch( gadget->getType() )
		{
			case _gadgetType::window:				Lunar<_lua_window>::emplace( L , (_window*)gadget );							break;
			case _gadgetType::button:				Lunar<_lua_button>::emplace( L , (_button*)gadget );							break;
			case _gadgetType::stickybutton:			Lunar<_lua_stickybutton>::emplace( L , (_stickyButton*)gadget );				break;
			case _gadgetType::label:				Lunar<_lua_label>::emplace( L , (_label*)gadget );								break;
			case _gadgetType::clockgadget:			Lunar<_lua_clockgadget>::emplace( L , (_clockGadget*)gadget );					break;
			case _gadgetType::checkbox:				Lunar<_lua_checkbox>::emplace( L , (_checkbox*)gadget );						break;
			case _gadgetType::selectbox:			Lunar<_lua_select>::emplace( L , (_select*)gadget );							break;
			case _gadgetType::textbox:				Lunar<_lua_textbox>::emplace( L , (_textBox*)gadget );							break;
			case _gadgetType::textarea:				Lunar<_lua_textarea>::emplace( L , (_textArea*)gadget );						break;
			case _gadgetType::counter:				Lunar<_lua_counter>::emplace( L , (_counter*)gadget );							break;
			case _gadgetType::progressbar:			Lunar<_lua_progressbar>::emplace( L , (_progressBar*)gadget );					break;
			case _gadgetType::radiobox:				Lunar<_lua_radio>::emplace( L , (_radio*)gadget );								break;
			case _gadgetType::calendar:				Lunar<_lua_calendar>::emplace( L , (_calendar*)gadget );						break;
			case _gadgetType::imagebutton:			Lunar<_lua_imagebutton>::emplace( L , (_imageButton*)gadget );					break;
			case _gadgetType::imagegadget:			Lunar<_lua_imagegadget>::emplace( L , (_imageGadget*)gadget );					break;
			case _gadgetType::scrollarea:			Lunar<_lua_scrollarea>::emplace( L , (_scrollArea*)gadget );					break;
			case _gadgetType::scrollbar:			Lunar<_lua_scrollbar>::emplace( L , (_scrollBar*)gadget );						break;
			case _gadgetType::gradientcolorpicker:	Lunar<_lua_gradientcolorpicker>::emplace( L , (_gradientColorPicker*)gadget );	break;
			case _gadgetType::hexagoncolorpicker:	Lunar<_lua_hexagoncolorpicker>::emplace( L , (_hexagonColorPicker*)gadget );	break;
			case _gadgetType::resizehandle:			Lunar<_lua_resizehandle>::emplace( L , (_resizeHandle*)gadget );				break;
			case _gadgetType::windowbar:			Lunar<_lua_windowbar>::emplace( L , (_windowBar*)gadget );						break;
			case _gadgetType::windowmenu:			Lunar<_lua_windowmenu>::emplace( L , (_windowMenu*)gadget );					break;
			case _gadgetType::slider:				Lunar<_lua_slider>::emplace( L , (_slider*)gadget );							break;
			case _gadgetType::contextmenu:			Lunar<_lua_contextmenu>::emplace( L , (_contextMenu*)gadget );					break;
			case _gadgetType::popup:				Lunar<_lua_popup>::emplace( L , (_contextMenu*)gadget );						break;
			case _gadgetType::mainframe:			Lunar<_lua_mainframe>::emplace( L , (_mainFrame*)gadget );						break;
			case _gadgetType::taskinfo:				Lunar<_lua_taskinfo>::emplace( L , (_taskInfo*)gadget );						break;
			default:								Lunar<_lua_gadget>::emplace( L , gadget , false );
		}
	}
}