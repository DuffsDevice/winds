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
		
		_scrollType scrollTypeX;
		_scrollType scrollTypeY;
		
		_scrollBar*	scrollBarX;
		_scrollBar*	scrollBarY;
		
		// Size of scrolling Area
		_length		clipWidth;
		_length		clipHeight;
		
		_length		canvasWidth;
		_length		canvasHeight;
		
		_u8			computeCanvasWidth;
		_u8			computeCanvasHeight;
		
		_gadgetList	nonEnhancedChildren;
		
		static _callbackReturn refreshHandler( _event event );
		static _callbackReturn resizeHandler( _event event );
		static _callbackReturn scrollHandler( _event event );
		
		void		computeCanvasSize();
		void		computeClipSize();
		void		updateScrollBars();
		
	public:
	
		// Method to "manually set the Area, that should be scrolled" or "to tell this scrollArea to compute the scrolling Area on its own by passing 0"
		void setCanvasWidth( _length canvasWidth = 0 ){ 
			if( canvasWidth )
			{
				this->canvasWidth = canvasWidth;
				computeCanvasWidth = 0;
			}
			else
				computeCanvasWidth = 2;
			this->computeCanvasSize();
			updateScrollBars();
		}
		
		void setCanvasHeight( _length canvasHeight = 0 ){
			if( canvasHeight )
			{
				this->canvasHeight = canvasHeight;
				computeCanvasHeight = 0;
			}
			else
				computeCanvasHeight = 2;
			this->computeCanvasSize();
			updateScrollBars();
		}
		
		// Methods to scroll to a position
		void scrollToX( _length position , bool ease = true ){ this->scrollBarX->setValue( position , ease ); }
		void scrollToY( _length position , bool ease = true ){ this->scrollBarY->setValue( position , ease ); }
		void scrollTo( _length posX , _length posY , bool ease = true ){ this->scrollBarX->setValue( posX , ease ); this->scrollBarY->setValue( posY , ease ); }
		
		// Get the current scroll
		_length getScrollY(){ return this->scrollBarY->getValue(); }
		_length getScrollX(){ return this->scrollBarX->getValue(); }
		
		// Set scrollTypes
		void setScrollTypeX( _scrollType typeX );
		void setScrollTypeY( _scrollType typeY );
		
		// Replace children-functions
		void addChild( _gadget* child );
		void removeChild( _gadget* child );
		
		//! Default Constructor width optional scrolltypes
		_scrollArea( _length width , _length height , _coord x , _coord y , _scrollType scrollTypeX = _scrollType::meta, _scrollType scrollTypeY = _scrollType::meta , _style style = _style() );
		
		//! Destrucor
		~_scrollArea();
};

extern map<_string,_scrollType> string2scrollType;
extern map<_scrollType,_string> scrollType2string;
#endif