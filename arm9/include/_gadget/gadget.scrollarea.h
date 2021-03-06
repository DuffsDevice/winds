#ifndef _WIN_G_SCROLLAREA_
#define _WIN_G_SCROLLAREA_

#include <_type/type.h>
#include <_type/type.gadget.h>
#include <_gadget/gadget.scrollBar.h>

enum class _scrollType : _u8{
	scroll = 0,
	meta = 1,
	hidden = 1,
	prevent = 2
};

class _scrollArea : public _gadget {
	
	private:
		
		_scrollType				scrollTypeX;
		_scrollType				scrollTypeY;
		
		_uniquePtr<_scrollBar>	scrollBarX;
		_uniquePtr<_scrollBar>	scrollBarY;
		
		_padding				offset;
		
		// Size of scrolling Area
		_length					clipWidth;
		_length					clipHeight;
		
		_length					canvasWidth;
		_length					canvasHeight;
		bool					leaveCorner;
		
		_gadgetList				nonEnhancedChildren;
		
		static _callbackReturn refreshHandler( _event );
		static _callbackReturn childHandler( _event );
		static _callbackReturn scrollHandler( _event );
		static _callbackReturn updateHandler( _event );
		static _callbackReturn resizeHandler( _event );
		
		void	computeClipSize();
		void	updateScrollBars();
		
	public:
		
		// Methods to scroll to a position
		void scrollToX( _length position , bool ease = true ){ this->scrollBarX->setValue( position , ease ); }
		void scrollToY( _length position , bool ease = true ){ this->scrollBarY->setValue( position , ease ); }
		void scrollTo( _length posX , _length posY , bool ease = true ){ this->scrollBarX->setValue( posX , ease ); this->scrollBarY->setValue( posY , ease ); }
		
		// Requests the scrollArea to leave the right bottom edge free the whole time for things like a resizehandle
		void leaveFreeCorner( bool flag = true ){
			if( this->leaveCorner == flag )
				return;
			this->leaveCorner = flag;
			this->updateScrollBars();
		}
		
		// Checks if the right bottom corner is left free
		bool hasFreeCorner(){ return this->leaveCorner; }
		
		// Get the current scroll
		_length getScrollY(){ return this->scrollBarY->getValue(); }
		_length getScrollX(){ return this->scrollBarX->getValue(); }
		
		// Set scrollTypes
		void setScrollTypeX( _scrollType typeX );
		void setScrollTypeY( _scrollType typeY );
		
		void setPaddingOffset( _padding p ){ this->offset = p; }
		_padding getPaddingOffset(){ return this->offset; }
		
		//! Default Constructor width optional scrolltypes
		_scrollArea( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , _scrollType scrollTypeX = _scrollType::meta , _scrollType scrollTypeY = _scrollType::meta , _style&& style = _style() );
		_scrollArea( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , _style&& style ) :
			_scrollArea( move(x) , move(y) , move(width) , move(height) , _scrollType::meta , _scrollType::meta , move(style) )
		{}
		
		//! Destrucor
		virtual ~_scrollArea() = default;
};

extern _fromStr<_scrollType>	string2scrollType;
extern _toStr<_scrollType>		scrollType2string;

#endif