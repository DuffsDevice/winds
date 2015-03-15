#include <_gadget/gadget.bubble.h>
#include <_controller/controller.font.h>
#include <_resource/resource.image.bubblearrow.h>
#include <_type/type.bitmap.transform.h>
#include <_type/type.windows.soundbank.h>

void _bubble::timerHandler(){
	this->shelve( false );
}

void _bubble::setupTimer(){
	this->removeInternalEventHandler( onBlur ); // Unregister _popup-Handler
	closeTimer.start();
}

_rect _bubble::getTitleDimensions() const
{
	_coord		rectOffsetY = ( getVerticalPart( this->getDirection() ) == _direction::down ? 5 : 0 );
	_coord		startX = this->icon.isValid() ? this->icon.getWidth() + 5 : 3;
	_coord		maxTitleHeight = max<_length>( this->icon.getHeight() , this->title.getTextHeight() );
	if( maxTitleHeight )
		maxTitleHeight++;
	
	return _rect( startX , rectOffsetY + 2 , this->getWidth() - startX - 1 , maxTitleHeight );
}

_rect _bubble::getContentDimensions( _rect titleDimensions ) const {
	return _rect( 3 , titleDimensions.getY2() , this->getWidth() - 4 , this->content.getTextHeight() );
}

_callbackReturn _bubble::openHandler( _event event ){
	_windowsSoundBank::play( _windowsSound::bubble );
	return handled;
}

_callbackReturn _bubble::updateHandler( _event event )
{
	_bubble* that = event.getGadget<_bubble>();
	
	// Get The Dimensions that both title and content should have
	_rect titleDims = that->getTitleDimensions();
	_rect contentDims = that->getContentDimensions( titleDims );
	
	// Update the title and the content to fit the dimensions
	that->title.update( titleDims );
	that->content.update( contentDims );
	
	// Set size depending on the required height
	that->setHeightIfAuto( max( 10 , that->content.getTextHeight() + that->title.getTextHeight() + 9 ) );
	
	return handled;
}

_callbackReturn _bubble::refreshHandler( _event event )
{
	_bubble* that = event.getGadget<_bubble>();
	
	// Get BitmapPort
	_bitmapPort	bP = that->getBitmapPort( event );
	_length		myH = bP.getHeight();
	_length		myW = bP.getWidth();
	_color		borderColor = _color::fromBW( 7 );
	
	// Fill background
	bP.fill( _color::transparent );
	
	// Determine orientation
	_direction	dir = that->getDirection();
	bool		flipH = getHorizontalPart( dir ) == _direction::right;
	bool		flipV = getVerticalPart( dir ) == _direction::down;
	_coord		rectOffsetY = ( flipV ? 5 : 0 );
	
	// Draw Left & Right Side
	bP.drawVerticalLine( 0 , rectOffsetY + 2 , myH - 9 , borderColor );
	bP.drawVerticalLine( myW - 1 , rectOffsetY + 2 , myH - 9 , borderColor );
	
	// Draw Top & Bottom Side
	bP.drawHorizontalLine( 2 , rectOffsetY , myW - 4 , borderColor );
	bP.drawHorizontalLine( 2 , rectOffsetY + myH - 6 , myW - 4 , borderColor );
	
	// Creme-colored background
	bP.drawFilledRect( 1 , rectOffsetY + 1 , myW - 2 , myH - 7 , _color::fromRGB8( 255 , 255 , 225 ) );
	
	// Draw Edges
	bP.drawPixel( 1 , rectOffsetY + 1 , borderColor );
	bP.drawPixel( myW - 2 , rectOffsetY + 1 , borderColor );
	bP.drawPixel( 1 , rectOffsetY + myH - 7 , borderColor );
	bP.drawPixel( myW - 2 , rectOffsetY + myH - 7 , borderColor );
	
	// Draw Arrow
	bP.copyTransparent( ( flipH ? 3 : myW - 9 ) , ( flipV ? 0 : myH - 6 ) , _bitmapTransform( BMP_BubbleArrow() , flipH , flipV ) );
	
	// Store text dimensions
	_rect titleDimensions = that->getTitleDimensions();
	_rect contentDimensions = that->getContentDimensions( titleDimensions );
	
	// Draw Text
	that->title.drawTo( titleDimensions , bP );
	that->content.drawTo( contentDimensions , bP );
	
	// Draw Bitmap
	if( that->icon.isValid() )
		bP.copyTransparent( 3 , 2 , that->icon );
	
	return use_default;
}

_bubble::_bubble( _length width , wstring title , wstring content , _bitmap icon , bool closeable , _style&& style ) :
	_popup( width , ignore , nullptr , (_style&&)style )
	, closeable( closeable )
	, title( move(title) , _fontController::getStandardFont() , _color::black , _fontController::getStandardFontSize() )
	, content( move(content) , _fontController::getStandardFont() , _color::fromBW( 12 ) , _fontController::getStandardFontSize() )
	, icon( move(icon) )
	, closeTimer( make_callback( this , &_bubble::timerHandler ) , 6000 , false )
{
	_popup::setOffset( { 6 , 0 } );
	this->title.setEllipsis( 0 );
	this->title.setAlign( _align::left );
	this->content.setAlign( _align::left );
	this->content.setVAlign( _valign::top );
	
	this->setInternalEventHandler( onUpdate , make_callback( &_bubble::updateHandler ) );
	
	// Update texts
	this->updateNow();
	
	// Register onDraw-Handler
	this->setInternalEventHandler( onDraw , make_callback( &_bubble::refreshHandler ) );
	this->setInternalEventHandler( onOpen , make_callback( &_bubble::openHandler ) );
}