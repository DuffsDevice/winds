#include "_lua/lua.func.h"
#include "_lua/lua.lunar.h"
#include "_lua/lua.class.event.h"
#include "_lua/lua.class.rect.h"
#include "_lua/lua.class.area.h"
#include "_lua/lua.class.font.h"
#include "_lua/lua.class.border.h"
#include "_lua/lua.class.bitmap.h"
#include "_lua/lua.class.bitmap.port.h"
#include "_lua/lua.class.hardwarekeypattern.h"
#include "_lua/lua.class.time.h"
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
#include "_lua/lua.gadget.colorpicker.h"
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

// For error-dialogs
#include "_type/type.system.h"
#include "_dialog/dialog.runtimeerror.h"

namespace _luafunc
{
	_map<lua_State*,_runtimeErrorDialog*> errorDialogs;
	
	void errorHandler( lua_State* L , const char* message , ... )
	{
		auto& dialogRef = errorDialogs[L];
		
		// If there is already a dialog: return, we only allow one dialog per state
		if( dialogRef )
			return;
		
		static char output[521];
		
		// Format right
		va_list args;
		va_start(args, message);
		vsprintf(output, message , args);
		va_end(args);
		
		// Create error-dialog that will destroy itself once it finished
		dialogRef = new _runtimeErrorDialog( _system::getLocalizedString( "lbl_lua_parser_error" ) , output );
		dialogRef->setCallback( make_inline_callback<void(_dialogResult)>( [=]( _dialogResult )->void{ delete dialogRef; } ) );
		dialogRef->execute();
	}
	
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
	
	void pushFont( lua_State* L , const _font* arg ){
		Lunar<_lua_font>::push( L , new _lua_font(arg) );
	}
	
	void pushTime( lua_State* L , _time&& arg ){
		Lunar<_lua_time>::push( L , new _lua_time( move(arg) ) );
	}
	
	void pushHWKP( lua_State* L , _hardwareKeyPattern&& arg ){
		Lunar<_lua_hardwarekeypattern>::push( L , new _lua_hardwarekeypattern( move(arg) ) );
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
			case _gadgetType::colorpicker:
				Lunar<_lua_colorpicker>::push( L , new _lua_colorpicker( (_colorPicker*)gadget ) );
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
	
	namespace detail
	{
		_gadget* check( lua_State* L , int index , _gadget** dummy )
		{
			_lua_gadget* gadget = Lunar<_lua_gadget>::lightcheck( L , index );
			return gadget ? gadget->getGadget() : nullptr;
		}
		
		_bitmap check( lua_State* L , int index , _bitmap* dummy ){
			_lua_bitmap* bmp = Lunar<_lua_bitmap>::lightcheck( L , index );
			return bmp ? (_bitmap&)*bmp : _bitmap();
		}
		
		_border check( lua_State* L , int index , _border* dummy ){
			_lua_border* b = Lunar<_lua_border>::lightcheck( L , index );
			return b ? *b : _border();
		}
		
		_time check( lua_State* L , int index , _time* dummy ){
			_lua_time* b = Lunar<_lua_time>::lightcheck( L , index );
			return b ? *b : _time();
		}
		
		_bitmapPort check( lua_State* L , int index , _bitmapPort* dummy )
		{
			static _bitmap bitmapFallback;
			
			_lua_bitmapport* bmp = Lunar<_lua_bitmapport>::lightcheck( L , index );
			if( bmp )
				return *bmp;
			
			return _bitmapPort(bitmapFallback);
		}
		
		_hardwareKeyPattern check( lua_State* L , int index , _hardwareKeyPattern* dummy ){
			_lua_hardwarekeypattern* hwkp = Lunar<_lua_hardwarekeypattern>::lightcheck( L , index );
			return hwkp ? *hwkp : _hardwareKeyPattern();
		}
		
		_rect check( lua_State* L , int index , _rect* dummy ){
			_lua_rect* rc = Lunar<_lua_rect>::lightcheck( L , index );
			return rc ? *rc : _rect();
		}
		
		_area check( lua_State* L , int index , _area* dummy ){
			_lua_area* ar = Lunar<_lua_area>::lightcheck( L , index );
			return ar ? *ar : _area();
		}
		
		_event check( lua_State* L , int index , _event* dummy ){
			_lua_event* ev = Lunar<_lua_event>::check( L , 1 );
			return ev ? *ev : _event();
		}
		
		const _font* check( lua_State* L , int index , const _font** dummy ){
			_lua_font* ft = Lunar<_lua_font>::lightcheck( L , index );
			if( ft )
				return (const _font*)*ft; // You CAN cast _lua_font to _font
			return nullptr;
		}
	}
}