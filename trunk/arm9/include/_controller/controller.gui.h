#ifndef _WIN_C_SCENARIO_
#define _WIN_C_SCENARIO_

#include <_type/type.h>
#include <_type/type.bitmap.h>
#include <_type/type.gui.h>
#include <_type/type.controller.h>
#include <_type/type.screen.gadget.h>
#include <_type/type.color.palette.h>
#include <_type/type.menu.h>
#include <_screen/screen.keyboard.h>
#include <_controller/controller.video.h>

enum class _guiState : _u8{
	empty,
	login,
	bootup,
	setup,
	desktop
};

extern _fromStr<_guiState>	string2guiState;
extern _toStr<_guiState>	guiState2string;

class _guiController : public _controller
{
	private:
		
		static _guiState					state;
		static bool							dirty;
		
		//! The GuiScenario-Object that is having controll over the system right now
		static _uniquePtr<_gui>				activeGui;
		
		
		//! Specific instances of screens
		static _uniquePtr<_gadgetScreen>	host;
		static _uniquePtr<_keyboardScreen>	keyboard;
		static _uniquePtr<_screen>			top;
		
		//! Currently focused gadget
		static _gadget*						currentFocus;
		static _gadget*						lastClickedGadget;
		
		//////////////////////////
		// Graphical Attributes //
		//////////////////////////
		
		static _bitmap						windowBar;
		static _bitmap						windowBarBlurred;
		static _colorPalette				disabledPalette;
		static _color						itemFg;
		static _color						itemBg;
		static _color						focusFg;
		static _color						focusBg;
		static _color						selectFg;
		static _color						selectBg;
		static _color						controlBg;
		static _color						controlFg;
		static bool							magnifyKeyboardFocus;
		
		////////////////////////////////////
		// Attributes about certain sizes //
		////////////////////////////////////
		
		static _u16 						minDragDistance;		// Minimum Distance after which dragging starts (in contrast to a click)
		static _u16							maxClickPeriod;			// The maximum number of frames after which a mouseDown and a mouseUp generate a 'mouseClick'-event
		static _u16							keyRepetitionDelay;		// Number of Frames until repetition
		static _u16							keyRepetitionPause;		// Number of Frames between each click
		static _u8							maxDoubleClickPeriod;	// Maximum number of frames between the two clicks that generates a double click
		static _u8							maxDoubleClickArea;		// The maximum area in which two clicks have to happen to generate a double click
		static _u16							rightClickDelay;		// The Number of frames that the stylus has to be down to generate a right click
		
		static _u8							fileObjectHeight;
		static _u8							listItemHeight;
		static _length						counterHeight;
		static _length						selectHeight;
		
		static _color						startButtonTextColor;
		static bool							fileExtensionVisible;
		
		//! Internal routine that creates the palette with which disabled gadgets are drawn disabled
		static void createDisabledPalette();
	
	public:
		
		//! Initialization routine for the scenario controller
		static bool				init();
		
		//! This method will change the current system scenario if needed
		static void				frame();
		
		//! Shuts down the Graphical User-Interface
		static void				end();
		
		
		/////////////////////////
		// DOM Tree Management //
		/////////////////////////
		
		//! The current state - getter and setter
		static void				changeState( _guiState state ){ dirty = _guiController::state != state; _guiController::state = state; }
		static _guiState		getState(){ return state; }
		
		//! Get the currently active Host-instance
		static _gadgetScreen*	getHost(){ return _guiController::host; }
		
		//! Get the top screen instance
		static _screen*			getTopScreen(){ return _guiController::top; }
		
		//! Creates a new host that replaces the old
		template<class HostClass>
		static void				setHost( _style&& style = _style() ){
			_guiController::host = new HostClass( _videoController::getBgIdHost() , (_style&&)style );
		}
		
		//! Deletes the current Gadget-Host
		static void				deleteHost(){ _guiController::host = nullptr; }
		
		//! Enables the Keyboard
		static void				enableKeyboard( _optValue<_coord> handlePosition = ignore );
		
		//! Disables the Keyboard
		static void				disableKeyboard();
		
		//! Get the currently active Keyboard-instance
		static _keyboardScreen*	getKeyboard(){ return _guiController::keyboard; }
		
		//! Get the currently focused gadget
		static _gadget*			getCurrentFocus(){ return _guiController::currentFocus; }
		static void				setCurrentFocus( _gadget* gadget ){ _guiController::currentFocus = gadget; }
		
		//! Get the _gadget that was touched by the mouse last
		static _gadget*			getLastClickedGadget(){ return _guiController::lastClickedGadget; }
		static void				setLastClickedGadget( _gadget* gadget ){ _guiController::lastClickedGadget = gadget; }
		
		//! Returns true, if the _textbox, _textarea or whatever shall be magnified when clicked
		static bool				isKeyboardMagnifEnabled(){ return _guiController::magnifyKeyboardFocus; }
		
		//! Enables and Disables the magnification of e.e. a _textbox when it gets focus
		static void				enableKeyboardMagnif(){ _guiController::magnifyKeyboardFocus = true; }
		static void				disableKeyboardMagnif(){ _guiController::magnifyKeyboardFocus = false; }
		
		//////////////////////
		// Graphical Things //
		//////////////////////
		
		//! Gets the Foreground color of an item in WinDS
		static _color			getItemFg( bool focused = false , bool selected = false ){
			if( focused )
				return _guiController::focusFg;
			return selected ? _guiController::selectFg : _guiController::itemFg;
		}
		
		//! Same Here for the Background
		static _color			getItemBg( bool focused = false , bool selected = false ){
			if( focused )
				return _guiController::focusBg;
			return selected ? _guiController::selectBg : _guiController::itemBg;
		}
		
		//! Gets the Foreground color for a native gui element
		static _color			getControlFg(){ return _guiController::controlFg; }
		
		//! Same Here for the Background
		static _color			getControlBg(){ return _guiController::controlBg; }
		
		//! Get the bitmap that makes up the upper bar of a window
		static _constBitmap&	getWindowsDesign( bool active = true ){ return active ? _guiController::windowBar : _guiController::windowBarBlurred; }
		
		//! Get the palette that helps to draw disabled gadgets
		static const _colorPalette&		getDisabledPalette(){ return _guiController::disabledPalette; }
		
		
		////////////////////////////////////
		// Getters for certain attributes //
		////////////////////////////////////
		
		static _u16		getMinDragDistance(){ return minDragDistance; }
		static _u16		getMaxClickPeriod(){ return maxClickPeriod; }
		static _u16		getKeyRepetitionDelay(){ return keyRepetitionDelay; }
		static _u16		getKeyRepetitionPause(){ return keyRepetitionPause; }
		static _u8		getMaxDoubleClickPeriod(){ return maxDoubleClickPeriod; }
		static _u8		getMaxDoubleClickArea(){ return maxDoubleClickArea; }
		static _u16		getRightClickDelay(){ return rightClickDelay; }
		
		static _u8		getFileObjectHeight(){ return fileObjectHeight; }
		static _u8		getListItemHeight(){ return listItemHeight; }
		static _length	getCounterHeight(){ return counterHeight; }
		static _length	getSelectHeight(){ return selectHeight; }
		
		static _color	getStartButtonTextColor(){ return startButtonTextColor; }
		static bool		showFileExtension(){ return fileExtensionVisible; }
};

#endif