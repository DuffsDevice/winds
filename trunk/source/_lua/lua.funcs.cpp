#include "_lua/lua.funcs.h"
#include "_lua/lunar.h"
#include "_lua/lua.class.event.h"
#include "_lua/lua.gadget.window.h"
#include "_lua/lua.gadget.button.h"
#include "_lua/lua.gadget.calendar.h"
#include "_lua/lua.gadget.label.h"
#include "_lua/lua.gadget.contextmenu.h"
#include "_lua/lua.gadget.progressbar.h"
#include "_lua/lua.gadget.resizehandle.h"
#include "_lua/lua.gadget.counter.h"
#include "_lua/lua.gadget.colorpicker.h"
#include "_lua/lua.gadget.checkbox.h"
#include "_lua/lua.gadget.imagegadget.h"
#include "_lua/lua.gadget.scrollArea.h"
#include "_lua/lua.gadget.scrollBar.h"
#include "_lua/lua.gadget.stickybutton.h"
#include "_lua/lua.gadget.textbox.h"
#include "_lua/lua.gadget.textarea.h"
#include "_lua/lua.gadget.select.h"
#include "_lua/lua.gadget.radio.h"
#include "_lua/lua.gadget.slider.h"

// For error-dialogs
#include "_dialog/dialog.runtimeerror.h"

namespace _luafunc
{
	_map<lua_State*,_runtimeErrorDialog*> errorDialogs;
	
	void errorHandler( lua_State* L , string message )
	{
		auto& dialogRef = errorDialogs[L];
		
		// If there is already a dialog: return, we only allow one dialog per state
		if( dialogRef )
			return;
		
		// Create error-dialog that will destroy itself once it finished
		dialogRef = new _runtimeErrorDialog( _system::getLocalizedString( "lbl_lua_parser_error" ) , move(message) );
		dialogRef->setCallback( make_inline_callback<void(_dialogResult)>( [=]( _dialogResult )->void{ delete dialogRef; } ) );
		dialogRef->execute();
	}
	
	void pushEvent( lua_State* L , _event&& arg ){
		Lunar<_lua_event>::push( L , new _lua_event( move(arg) ) );
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
				Lunar<_lua_stickybutton>::push( L , new _lua_stickybutton( (_stickybutton*)gadget ) );
				break;
			case _gadgetType::label:
				Lunar<_lua_label>::push( L , new _lua_label( (_label*)gadget ) );
				break;
			case _gadgetType::checkbox:
				Lunar<_lua_checkbox>::push( L , new _lua_checkbox( (_checkbox*)gadget ) );
				break;
			case _gadgetType::selectbox:
				Lunar<_lua_select>::push( L , new _lua_select( (_select*)gadget ) );
				break;
			case _gadgetType::textbox:
				Lunar<_lua_textbox>::push( L , new _lua_textbox( (_textbox*)gadget ) );
				break;
			case _gadgetType::textarea:
				Lunar<_lua_textarea>::push( L , new _lua_textarea( (_textarea*)gadget ) );
				break;
			case _gadgetType::counter:
				Lunar<_lua_counter>::push( L , new _lua_counter( (_counter*)gadget ) );
				break;
			case _gadgetType::progressbar:
				Lunar<_lua_progressbar>::push( L , new _lua_progressbar( (_progressbar*)gadget ) );
				break;
			case _gadgetType::radiobox:
				Lunar<_lua_radio>::push( L , new _lua_radio( (_radio*)gadget ) );
				break;
			case _gadgetType::calendar:
				Lunar<_lua_calendar>::push( L , new _lua_calendar( (_calendar*)gadget ) );
				break;
			case _gadgetType::imagegadget:
				Lunar<_lua_imagegadget>::push( L , new _lua_imagegadget( (_imagegadget*)gadget ) );
				break;
			case _gadgetType::scrollarea:
				Lunar<_lua_scrollarea>::push( L , new _lua_scrollarea( (_scrollArea*)gadget ) );
				break;
			case _gadgetType::scrollbar:
				Lunar<_lua_scrollbar>::push( L , new _lua_scrollbar( (_scrollBar*)gadget ) );
				break;
			case _gadgetType::colorpicker:
				Lunar<_lua_colorpicker>::push( L , new _lua_colorpicker( (_colorpicker*)gadget ) );
				break;
			case _gadgetType::resizehandle:
				Lunar<_lua_resizehandle>::push( L , new _lua_resizehandle( (_resizeHandle*)gadget ) );
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
	
	template<> _gadget* lightcheck<_gadget>( lua_State* L , int index , _gadget* fallback )
	{
		if( !lua_isuserdata( L , index ) )
			return fallback;
		
		_lua_gadget* tmp;
		
		if( ( tmp = Lunar< _lua_window >::lightcheck( L , index ) ) != nullptr )
			return tmp->getGadget();
		if( ( tmp = Lunar< _lua_button >::lightcheck( L , index ) ) != nullptr )
			return tmp->getGadget();
		if( ( tmp = Lunar< _lua_stickybutton >::lightcheck( L , index ) ) != nullptr )
			return tmp->getGadget();
		if( ( tmp = Lunar< _lua_label >::lightcheck( L , index ) ) != nullptr )
			return tmp->getGadget();
		if( ( tmp = Lunar< _lua_checkbox >::lightcheck( L , index ) ) != nullptr )
			return tmp->getGadget();
		if( ( tmp = Lunar< _lua_select >::lightcheck( L , index ) ) != nullptr )
			return tmp->getGadget();
		if( ( tmp = Lunar< _lua_textbox >::lightcheck( L , index ) ) != nullptr )
			return tmp->getGadget();
		if( ( tmp = Lunar< _lua_textarea >::lightcheck( L , index ) ) != nullptr )
			return tmp->getGadget();
		if( ( tmp = Lunar< _lua_counter >::lightcheck( L , index ) ) != nullptr )
			return tmp->getGadget();
		if( ( tmp = Lunar< _lua_progressbar >::lightcheck( L , index ) ) != nullptr )
			return tmp->getGadget();
		if( ( tmp = Lunar< _lua_radio >::lightcheck( L , index ) ) != nullptr )
			return tmp->getGadget();
		if( ( tmp = Lunar< _lua_calendar >::lightcheck( L , index ) ) != nullptr )
			return tmp->getGadget();
		if( ( tmp = Lunar< _lua_imagegadget >::lightcheck( L , index ) ) != nullptr )
			return tmp->getGadget();
		if( ( tmp = Lunar< _lua_scrollarea >::lightcheck( L , index ) ) != nullptr )
			return tmp->getGadget();
		if( ( tmp = Lunar< _lua_scrollbar >::lightcheck( L , index ) ) != nullptr )
			return tmp->getGadget();
		if( ( tmp = Lunar< _lua_colorpicker >::lightcheck( L , index ) ) != nullptr )
			return tmp->getGadget();
		if( ( tmp = Lunar< _lua_resizehandle >::lightcheck( L , index ) ) != nullptr )
			return tmp->getGadget();
		if( ( tmp = Lunar< _lua_slider >::lightcheck( L , index ) ) != nullptr )
			return tmp->getGadget();
		if( ( tmp = Lunar< _lua_contextmenu >::lightcheck( L , index ) ) != nullptr )
			return tmp->getGadget();
		
		// Fallback No. 1
		if( ( tmp = Lunar<_lua_gadget>::lightcheck( L , index ) ) != nullptr )
			return tmp->getGadget();
		
		// Fallback No. 2
		return fallback;
	}
	
	template<> _bitmap check<_bitmap>( lua_State* L , int index )
	{
		_lua_bitmap* bmp = Lunar<_lua_bitmap>::check( L , index );
		if( bmp && bmp->bm )
			return *bmp->bm;
		return _bitmap();
	}
	
	template<> _bitmap lightcheck<_bitmap>( lua_State* L , int index , _bitmap&& fallback )
	{
		_lua_bitmap* bmp = Lunar<_lua_bitmap>::lightcheck( L , index );
		if( bmp && bmp->bm )
			return *bmp->bm;
		return fallback;
	}
}