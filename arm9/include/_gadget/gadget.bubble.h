#ifndef _WIN_G_BUBBLE_
#define _WIN_G_BUBBLE_

#include "_type/type.gadget.h"
#include "_type/type.timer.h"
#include "_type/type.guistring.singleline.h"
#include "_type/type.guistring.multiline.h"
#include "_gadget/gadget.popup.h"

class _bubble : public _popup
{
	private:
		
		bool					closeable;
		_singleLineGuiString	title;
		_multiLineGuiString		content;
		_bitmap					icon;
		_timer					closeTimer;
		
		// Some event handlers
		static _callbackReturn updateHandler( _event );
		static _callbackReturn refreshHandler( _event );
		static _callbackReturn openHandler( _event );
		
		_rect	getTitleDimensions() const ;
		_rect	getContentDimensions( _rect titleDimensions ) const ;
		void	setupTimer();
		void	timerHandler();
	
	public:
		
		//! Ctor
		_bubble( _length width , string title , string content , _bitmap icon = _bitmap() , bool closeable = true , _style&& style = _style() );
		
		//! Other, still possible ways to open the bubble
		using _popup::show;
		
		//! Make the bubble point to a specific gadget
		void show( _gadget* gadget , bool rightDownOnly = true ){
			setupTimer();
			_popup::show( gadget->getAbsoluteDimensions() , rightDownOnly );
		}
		void toggle( _gadget* gadget , bool rightDownOnly = true ){
			setupTimer();
			_popup::toggle( gadget->getAbsoluteDimensions() , rightDownOnly );
		}
};

#endif