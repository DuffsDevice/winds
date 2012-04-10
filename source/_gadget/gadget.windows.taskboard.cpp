#include "_gadget/gadget.windows.taskBoard.h"
#include "_type/type.system.h"
#include "_graphic/BMP_StartButton.h"
#include "_graphic/BMP_WindowHeader.h"

void _windowsTaskBoard::openDialog( _gadget* dest , _gadget* visibleArea )
{
	this->keyboard->setDestination( dest );
	
	//! If not already visible
	if( !this->mode )
	{
		this->anim->setFromValue( this->getY() );
		this->anim->setToValue( SCREEN_HEIGHT - 112 );
		this->anim->start( _system_->getNow() );
		_system_->addAnimation( this->anim );
		this->mode = true;
	}
}

_gadgetEventReturnType _windowsTaskBoard::refreshHandler( _gadgetEvent event )
{	
	// Receive Gadget
	_windowsTaskBoard* that = (_windowsTaskBoard*)event.getGadget();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.getArgs().isBubbleRefresh() )
	{
		bP.addClippingRects( event.getArgs().getDamagedRects().toRelative( that->getAbsoluteDimensions() ) );
	}
	else
		bP.resetClippingRects();
	
	bP.copyHorizontalStretch( 33 , 0 , SCREEN_WIDTH - 33 , _system_->_runtimeAttributes_->windowBar );
	bP.copy( 0 , 0 , that->startButton );
	bP.drawString( 12 , 2 , _system_->_runtimeAttributes_->defaultFont , _system_->_runtimeAttributes_->startButtonText , RGB( 30 , 30 , 30 ) );
	
	return use_default;
}

_gadgetEventReturnType _windowsTaskBoard::dragHandler( _gadgetEvent event )
{
	// Receive Gadget
	_windowsTaskBoard* that = (_windowsTaskBoard*)event.getGadget();
	
	static int deltaY = 0;
	
	if( event.getType() == dragStart )
	{
		// If y pos is not on the windowbar, let my children gagdet be the object of Dragment :-)
		if( event.getArgs().getPosY() > 11 )
		{
			that->dragMe = false;			
			// Check children
			return use_default;
		}
		
		that->dragMe = true;
		
		deltaY = event.getArgs().getPosY();
		
		// If y is on the windowbar, drag Me!
		return handled;
	}
	else if( event.getType() == dragging )
	{
		// Check if there is a gadget who receives drag-events,
		// If not, it has to be me who's dragged
		if( !that->dragMe )
			return use_default;
		
		/**
		 * Handling of my 'dragment' !
		**/
		
		// Has the Gadget to move?
		if( event.getArgs().getDeltaY() == 0 )
			return handled;
		
		// Move it relatively
		that->moveTo( 0 , max( SCREEN_HEIGHT - 112 , 0 - deltaY + event.getArgs().getPosY() ) );
		
		// Return
		return handled;
	}
	else if( event.getType() == dragStop )
	{
		// Check if there is a gadget who receives drag-events,
		// If not, it has to be me who's dragged
		if( !that->dragMe )
			return use_default;
			
		that->anim->setFromValue( that->getY() );
		
		_coord toValue = SCREEN_HEIGHT - 112;
		if( ( that->mode == true && event.getArgs().getPosY() + that->getAbsoluteY() > SCREEN_HEIGHT - 80 ) || ( that->mode == false && event.getArgs().getPosY() + that->getAbsoluteY() > SCREEN_HEIGHT - 30 ) )
			toValue = SCREEN_HEIGHT - 10;
		
		that->mode = (SCREEN_HEIGHT - 112 == toValue ) ? true : false ;
		if( !that->mode )
			that->keyboard->setDestination( nullptr );
		
		that->anim->setToValue( toValue );
		that->anim->start( _system_->getNow() );
		_system_->addAnimation( that->anim );
		
		// Return
		return handled;
	}
	
	// Default return
	return not_handled;
}

_bitmap* _windowsTaskBoard::startButton = new BMP_StartButton();

_windowsTaskBoard::_windowsTaskBoard() :
	_gadget( _gadgetType::taskboard , SCREEN_WIDTH , 112 , 0 , SCREEN_HEIGHT - 10 )
{
	this->anim = new _animation<int>( 0 , 0 , 500 );
	this->anim->setEasing( _animation<int>::_bounce::easeOut );
	this->anim->setter( [&]( int y ){ this->setY( y ); } );
	
	this->mode = false; // Means "Hidden"
	
	//! Add the Keyboard
	this->keyboard = new _keyboard( 0 , 10 , this );
	this->addChild( this->keyboard );
	
	this->registerEventHandler( refresh , &_windowsTaskBoard::refreshHandler );
	this->registerEventHandler( dragStart , &_windowsTaskBoard::dragHandler );
	this->registerEventHandler( dragStop , &_windowsTaskBoard::dragHandler );
	this->registerEventHandler( dragging , &_windowsTaskBoard::dragHandler );
	
	//! Refresh Me!
	this->refreshBitmap();
}

_windowsTaskBoard::~_windowsTaskBoard(){
	delete this->anim;
	delete this->keyboard;
}