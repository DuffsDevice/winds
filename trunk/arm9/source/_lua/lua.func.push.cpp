#include "_lua/lua.func.h"
#include "_lua/lua.lunar.h"
#include "_lua/lua.class.event.h"
#include "_lua/lua.class.rect.h"
#include "_lua/lua.class.area.h"
#include "_lua/lua.class.font.h"
#include "_lua/lua.class.color.h"
#include "_lua/lua.class.border.h"
#include "_lua/lua.class.bitmap.h"
#include "_lua/lua.class.bitmap.port.h"
#include "_lua/lua.class.hardwarekeypattern.h"
#include "_lua/lua.class.time.h"
#include "_lua/lua.class.mime.h"
#include "_lua/lua.gadget.window.h"
#include "_lua/lua.gadget.window.bar.h"
#include "_lua/lua.gadget.window.menu.h"
#include "_lua/lua.gadget.button.h"
#include "_lua/lua.gadget.calendar.h"
#include "_lua/lua.gadget.label.h"
#include "_lua/lua.gadget.contextmenu.h"
#include "_lua/lua.gadget.progressbar.h"
#include "_lua/lua.gadget.resizehandle.h"
#include "_lua/lua.gadget.counter.h"
#include "_lua/lua.gadget.colorpicker.gradient.h"
#include "_lua/lua.gadget.colorpicker.hexagon.h"
#include "_lua/lua.gadget.checkbox.h"
#include "_lua/lua.gadget.clock.h"
#include "_lua/lua.gadget.button.image.h"
#include "_lua/lua.gadget.image.h"
#include "_lua/lua.gadget.scrollArea.h"
#include "_lua/lua.gadget.scrollBar.h"
#include "_lua/lua.gadget.button.sticky.h"
#include "_lua/lua.gadget.textbox.h"
#include "_lua/lua.gadget.textarea.h"
#include "_lua/lua.gadget.select.h"
#include "_lua/lua.gadget.radio.h"
#include "_lua/lua.gadget.slider.h"

namespace _luafunc
{
	void pushEvent( lua_State* L , _event&& arg ){
		Lunar<_lua_event>::push( L , new _lua_event( move(arg) ) );
	}
	
	void pushRect( lua_State* L , _rect&& arg ){
		Lunar<_lua_rect>::push( L , new _lua_rect( move(arg) ) );
	}
	
	void pushArea( lua_State* L , _area&& arg ){
		Lunar<_lua_area>::push( L , new _lua_area( move(arg) ) );
	}
	
	void pushBitmapRef( lua_State* L , _bitmap& arg ){
		Lunar<_lua_bitmap>::push( L , new _lua_bitmap(&arg) );
	}
	
	void pushBorder( lua_State* L , _border&& arg ){
		Lunar<_lua_border>::push( L , new _lua_border(arg) );
	}
	
	void pushBitmap( lua_State* L , _bitmap&& arg ){
		Lunar<_lua_bitmap>::push( L , new _lua_bitmap( move(arg) ) );
	}
	
	void pushBitmapPort( lua_State* L , _bitmapPort&& arg ){
		Lunar<_lua_bitmapport>::push( L , new _lua_bitmapport( move(arg) ) );
	}
	
	void pushFont( lua_State* L , _fontPtr arg ){
		Lunar<_lua_font>::push( L , new _lua_font(arg) );
	}
	
	void pushTime( lua_State* L , _time&& arg ){
		Lunar<_lua_time>::push( L , new _lua_time( move(arg) ) );
	}
	
	void pushColor( lua_State* L , _color&& arg ){
		Lunar<_lua_color>::push( L , new _lua_color( move(arg) ) );
	}
	
	void pushHWKP( lua_State* L , _hardwareKeyPattern&& arg ){
		Lunar<_lua_hardwarekeypattern>::push( L , new _lua_hardwarekeypattern( move(arg) ) );
	}
	
	void pushMimeType( lua_State* L , _mimeType&& arg ){
		Lunar<_lua_mimetype>::push( L , new _lua_mimetype( move(arg) ) );
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
			case _gadgetType::window:
				Lunar<_lua_window>::push( L , new _lua_window( (_window*)gadget ) );
				break;
			case _gadgetType::button:
				Lunar<_lua_button>::push( L , new _lua_button( (_button*)gadget ) );
				break;
			case _gadgetType::stickybutton:
				Lunar<_lua_stickybutton>::push( L , new _lua_stickybutton( (_stickyButton*)gadget ) );
				break;
			case _gadgetType::label:
				Lunar<_lua_label>::push( L , new _lua_label( (_label*)gadget ) );
				break;
			case _gadgetType::clockgadget:
				Lunar<_lua_clockgadget>::push( L , new _lua_clockgadget( (_clockGadget*)gadget ) );
				break;
			case _gadgetType::checkbox:
				Lunar<_lua_checkbox>::push( L , new _lua_checkbox( (_checkbox*)gadget ) );
				break;
			case _gadgetType::selectbox:
				Lunar<_lua_select>::push( L , new _lua_select( (_select*)gadget ) );
				break;
			case _gadgetType::textbox:
				Lunar<_lua_textbox>::push( L , new _lua_textbox( (_textBox*)gadget ) );
				break;
			case _gadgetType::textarea:
				Lunar<_lua_textarea>::push( L , new _lua_textarea( (_textArea*)gadget ) );
				break;
			case _gadgetType::counter:
				Lunar<_lua_counter>::push( L , new _lua_counter( (_counter*)gadget ) );
				break;
			case _gadgetType::progressbar:
				Lunar<_lua_progressbar>::push( L , new _lua_progressbar( (_progressBar*)gadget ) );
				break;
			case _gadgetType::radiobox:
				Lunar<_lua_radio>::push( L , new _lua_radio( (_radio*)gadget ) );
				break;
			case _gadgetType::calendar:
				Lunar<_lua_calendar>::push( L , new _lua_calendar( (_calendar*)gadget ) );
				break;
			case _gadgetType::imagebutton:
				Lunar<_lua_imagebutton>::push( L , new _lua_imagebutton( (_imageButton*)gadget ) );
				break;
			case _gadgetType::imagegadget:
				Lunar<_lua_imagegadget>::push( L , new _lua_imagegadget( (_imageGadget*)gadget ) );
				break;
			case _gadgetType::scrollarea:
				Lunar<_lua_scrollarea>::push( L , new _lua_scrollarea( (_scrollArea*)gadget ) );
				break;
			case _gadgetType::scrollbar:
				Lunar<_lua_scrollbar>::push( L , new _lua_scrollbar( (_scrollBar*)gadget ) );
				break;
			case _gadgetType::gradientcolorpicker:
				Lunar<_lua_gradientcolorpicker>::push( L , new _lua_gradientcolorpicker( (_gradientColorPicker*)gadget ) );
				break;
			case _gadgetType::hexagoncolorpicker:
				Lunar<_lua_hexagoncolorpicker>::push( L , new _lua_hexagoncolorpicker( (_hexagonColorPicker*)gadget ) );
				break;
			case _gadgetType::resizehandle:
				Lunar<_lua_resizehandle>::push( L , new _lua_resizehandle( (_resizeHandle*)gadget ) );
				break;
			case _gadgetType::windowbar:
				Lunar<_lua_windowbar>::push( L , new _lua_windowbar( (_windowBar*)gadget ) );
				break;
			case _gadgetType::windowmenu:
				Lunar<_lua_windowmenu>::push( L , new _lua_windowmenu( (_windowMenu*)gadget ) );
				break;
			case _gadgetType::slider:
				Lunar<_lua_slider>::push( L , new _lua_slider( (_slider*)gadget ) );
				break;
			case _gadgetType::contextmenu:
				Lunar<_lua_contextmenu>::push( L , new _lua_contextmenu( (_contextMenu*)gadget ) );
				break;
			default:
				Lunar<_lua_gadget>::push( L , new _lua_gadget( gadget , false ) );
		}
	}
}