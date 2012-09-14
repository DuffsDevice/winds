#ifndef _WIN_G_SCROLLAREA_
#define _WIN_G_SCROLLAREA_

#include "_type/type.h"
#include "_type/type.gadget.h"
#include "_gadget/gadget.scrollBar.h"

enum class _scrollType : _u8{
	scroll = 0,
	meta = 1,
	hidden = 1,
	prevent = 2
};

class _scrollArea : public _gadget {
	
	private:
	
		_length scrollX;
		_length scrollY;
		
		_scrollType scrollTypeX;
		_scrollType scrollTypeY;
		
		_scrollBar*	scrollBarX;
		_scrollBar*	scrollBarY;
		
		// Size of scrolling Area
		_length		clipWidth;
		_length		clipHeight;
		
		_length		canvasWidth;
		_length		canvasHeight;
		
		bool		computeCanvasWidth;
		bool		computeCanvasHeight;
		
		_gadgetList	nonEnhancedChildren;
		
		static _gadgetEventReturnType refreshHandler( _gadgetEvent event );
		static _gadgetEventReturnType dragHandler( _gadgetEvent event );
		static _gadgetEventReturnType resizeHandler( _gadgetEvent e );
		
		void		computeCanvasSize();
		void		computeClipSize();
		void		refresh();
		
	public:
	
		// Method to "manually set the Area, that should be scrolled" or "to tell this scrollArea to compute the scrolling Area on its own by passing 0"
		void setCanvasWidth( _length canvasWidth = 0 ){ 
			this->computeCanvasWidth = !canvasWidth;
			if( canvasWidth )
				this->canvasWidth = canvasWidth;
			else
				this->computeCanvasSize();
			refresh();
		}
		
		void setCanvasHeight( _length canvasHeight = 0 ){
			this->computeCanvasHeight = !canvasHeight;
			if( canvasHeight )
				this->canvasHeight = canvasHeight;
			else
				this->computeCanvasSize();
			refresh();
		}
		
		// Methods to scroll to a position
		void scrollToX( _length position , bool ease = false );
		void scrollToY( _length position , bool ease = false );
		
		// Get the current scroll
		_length getScrollY();
		_length getScrollX();
		
		// Set scrollTypes
		void setScrollTypeX( _scrollType typeX );
		void setScrollTypeY( _scrollType typeY );
		
		// Replace children-functions
		void addChild( _gadget* child );
		void removeChild( _gadget* child );
		
		//! Default Constructor width optional scrolltypes
		_scrollArea( _length width , _length height , _coord x , _coord y , _scrollType scrollTypeX = _scrollType::meta, _scrollType scrollTypeY = _scrollType::meta , _gadgetStyle style = _defaultStyle_ );
		
		//! Destrucor
		~_scrollArea();
};

extern map<_string,_scrollType> string2scrollType;
extern map<_scrollType,_string> scrollType2string;
#endif