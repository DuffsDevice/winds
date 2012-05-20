#ifndef _WIN_G_SCROLLAREA_
#define _WIN_G_SCROLLAREA_

#include "_type/type.h"
#include "_type/type.gadget.h"
#include "_gadget/gadget.scrollArea.buttons.h"

typedef enum{
	scroll,
	hidden,
	prevent
}_scrollType;

class _scrollArea : public _gadget {
	
	private:
	
		_length		scrollX;
		_length		scrollY;
		
		// Size of scrolling Area
		_length		innerWidth;
		_length		innerHeight;
		
		_u8			computeInnerWidth;
		_u8			computeInnerHeight;
		
		_gadgetList	nonEnhancedChildren;
		
		_scrollType scrollTypeX;
		_scrollType scrollTypeY;
		
		_length		visibleWidth;
		_length		visibleHeight;
		
		_scrollButton*	buttonTop;
		_scrollButton*	buttonBottom;
		_scrollButton*	buttonRight;
		_scrollButton*	buttonLeft;
		_scrollButton*	buttonHandleX;
		_scrollButton*	buttonHandleY;
		
		_u32			_ratioWidth_;
		_u32			_ratioHeight_;
		
		static _gadgetEventReturnType refreshHandler( _gadgetEvent event );
		static _gadgetEventReturnType dragHandler( _gadgetEvent event );
		
		void 		onResize();
		
		void		computeInnerSize();
		
		void		computeRatio();
		
		_u32 		getWidthRatio( _u32 of ){ return this->_ratioWidth_ * of >> 16; }
		
		_u32		getHeightRatio( _u32 of ){ return this->_ratioHeight_ * of >> 16; }
		
		void		hideOrShowScrollButtons();
		
		void		moveScrollHandleX();
		void		moveScrollHandleY();
		
	public:
	
		// Methods to tell this scrollArea to compute its size
		void setInnerWidth(){ this->computeInnerWidth = true; this->computeInnerSize(); }
		void setInnerHeight(){ this->computeInnerHeight = true; this->computeInnerSize(); }
		
		// Method to manually set the Area that should be scrolled
		void setInnerWidth( _length innerWidth ){ this->computeInnerWidth = false; this->innerWidth = innerWidth; }
		void setInnerHeight( _length innerHeight ){ this->computeInnerHeight = false; this->innerHeight = innerHeight; }
		
		void scrollToX( _length position , bool ease = false );
		void scrollToY( _length position , bool ease = false );
		
		// Methdos to scroll to a position
		_length getScrollY();
		_length getScrollX();
		
		void setScrollTypeX( _scrollType typeX );
		void setScrollTypeY( _scrollType typeY );
		
		// Replace children-functions
		void addChild( _gadget* child );
		void removeChild( _gadget* child );
		
		//! Default Constructor width optional scrolltypes
		_scrollArea( _length width , _length height , _coord x , _coord y , _scrollType scrollTypeX = _scrollType::scroll, _scrollType scrollTypeY = _scrollType::scroll , _gadgetStyle style = _defaultStyle_ );
		
		//! Destrucor
		~_scrollArea();
};
#endif