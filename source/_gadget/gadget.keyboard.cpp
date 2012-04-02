#include "_gadget/gadget.keyboard.h"
#include "_gadget/gadget.windows.h"
#include "_type/type.animation.h"
#include "_type/type.system.h"

#include "nds/arm9/console.h"

#include "_graphic/FONT_CourierNew10.h"
#include "_graphic/BMP_Background.h"

_bitmap* _keyboard::bg = new BMP_Background();

_rect _keyboard::buttonDimensions[]{
	_rect( 2 , 2 , 22 , 18 ), // 1
	_rect( 26 , 2 , 22 , 18 ), // 2
	_rect( 50 , 2 , 22 , 18 ), // 3
	_rect( 74 , 2 , 22 , 18 ), // 4
	_rect( 98 , 2 , 22 , 18 ), // 5
	_rect( 122 , 2 , 22 , 18 ), // 6
	_rect( 146 , 2 , 22 , 18 ), // 7
	_rect( 170 , 2 , 22 , 18 ), // 8
	_rect( 194 , 2 , 22 , 18 ), // 9
	_rect( 218 , 2 , 22 , 18 ), // 0
	_rect( 9 , 22 , 22 , 18 ), // Q
	_rect( 33 , 22 , 22 , 18 ), // W
	_rect( 57 , 22 , 22 , 18 ), // E
	_rect( 81 , 22 , 22 , 18 ), // R
	_rect( 105 , 22 , 22 , 18 ), // T
	_rect( 129 , 22 , 22 , 18 ), // Y/Z
	_rect( 153 , 22 , 22 , 18 ), // U
	_rect( 177 , 22 , 22 , 18 ), // I
	_rect( 201 , 22 , 22 , 18 ), // O
	_rect( 225 , 22 , 22 , 18 ), // P
	_rect( 2 , 42 , 22 , 18 ), // A
	_rect( 26 , 42 , 22 , 18 ), // S
	_rect( 50 , 42 , 22 , 18 ), // D
	_rect( 74 , 42 , 22 , 18 ), // F
	_rect( 98 , 42 , 22 , 18 ), // G
	_rect( 122 , 42 , 22 , 18 ), // H
	_rect( 146 , 42 , 22 , 18 ), // J
	_rect( 170 , 42 , 22 , 18 ), // K
	_rect( 194 , 42 , 22 , 18 ), // L
	_rect( 218 , 42 , 36 , 18 ), // Backspace
	_rect( 2 , 62 , 28 , 18 ), // Caps-Lock
	_rect( 32 , 62 , 22 , 18 ), // Z/Y
	_rect( 56 , 62 , 22 , 18 ), // X
	_rect( 80 , 62 , 22 , 18 ), // C
	_rect( 104 , 62 , 22 , 18 ), // V
	_rect( 128 , 62 , 22 , 18 ), // B
	_rect( 152 , 62 , 22 , 18 ), // N
	_rect( 176 , 62 , 22 , 18 ), // M
	_rect( 200 , 62 , 22 , 18 ), // .
	_rect( 224 , 62 , 30 , 18 ), // Return
	_rect( 2 , 82 , 33 , 18 ), // Shift
	_rect( 37 , 82 , 22 , 18 ), // Windows
	_rect( 61 , 82 , 110 , 18 ), // Space
	_rect( 173 , 82 , 22 , 18 ), // ,
	_rect( 197 , 82 , 22 , 18 ), // ?
	_rect( 221 , 82 , 33 , 18 ) // Shift
};

_gadgetEventReturnType _keyboard::refreshHandler( _gadgetEvent event )
{
	// Receive Gadget
	_keyboard* that = (_keyboard*)event.getGadget();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.getArgs().isBubbleRefresh() )
		bP.addClippingRects( event.getArgs().getDamagedRects().toRelative( that->getAbsoluteDimensions() ) );
	else
		bP.resetClippingRects();
	
	//_length myW = bP.getWidth();
	
	//bP.fill( that->style.windowBg );
	
	//bP.drawVerticalGradient( 0 , 0 , myW , 5 , RGB( 17 , 17 , 17 ) , that->style.windowBg );
	
	bP.copy( 0 , 0 , that->bg );
	
	return use_default;
}

_gadgetEventReturnType _keyboard::keyHandler( _gadgetEvent event )
{	
	// Receive Gadget
	_keyboard* that = (_keyboard*)event.getGadget();
	
	if( event.getArgs().getKeyCode() == DSWindows::KEY_SHIFT )
	{
		if( that->shift == 2 )
			return handled;
		
		that->shift = !that->shift;
		
		for( _u8 i = 0 ; i < 46 ; i++ )
		{
			that->buttons[i]->setTitle( _system_->_runtimeAttributes_->keyboardText[ bool( that->shift ) ][i] );
			that->buttons[i]->setKey( _system_->_runtimeAttributes_->keyboardChar[ bool( that->shift ) ][i] );
		}
	}
	else if( that->destination != nullptr )
		that->destination->handleEvent( event );
	
	return handled;
}

_keyboard::_keyboard( _coord x , _coord y , _gadget* dest , _gadgetStyle style ) :
	_gadget( _gadgetType::keyboard , SCREEN_WIDTH , 102 , x , y , style )
	, font( new FONT_CourierNew10() )
	, shift( false )
	, destination( dest )
{
	//! Create the buttons
	for( _u8 i = 0 ; i < 46 ; i++ )
	{
		this->buttons[i] = new _keyboardButton( _system_->_runtimeAttributes_->keyboardChar[0][i] , this->buttonDimensions[i].width , this->buttonDimensions[i].height , this->buttonDimensions[i].x , this->buttonDimensions[i].y , _system_->_runtimeAttributes_->keyboardText[0][i] );
		this->buttons[i]->setFont( this->font );
		this->addChild( this->buttons[i] );
	}
	
	this->buttons[45]->setAutoSelect( true );
	this->buttons[40]->setAutoSelect( true );
	
	
	//! Register my handler as the default Refresh-Handler
	this->registerEventHandler( refresh , &_keyboard::refreshHandler );
	this->registerEventHandler( keyDown , &_keyboard::keyHandler );
	this->registerEventHandler( keyUp , &_keyboard::keyHandler );
	this->registerEventHandler( keyClick , &_keyboard::keyHandler );
	
	// Refresh Me
	this->refreshBitmap();
}

_keyboard::~_keyboard(){
	delete this->font;
	//this->dtor();
}


_gadgetEventReturnType _keyboardButton::mouseHandler( _gadgetEvent event )
{
	// Receive Gadget
	_keyboardButton* that = (_keyboardButton*)event.getGadget();
	
	_gadgetEvent ev = _gadgetEvent( keyClick );
	
	// Set Key-code
	ev.getArgs().setKeyCode( that->key );
	
	if( that->getWindows() != nullptr )
		ev.getArgs().setCurrentKeyCodes( _system_->getCurrentKeys() );
	
	if( that->parent != nullptr )
		that->parent->handleEvent( ev );
	
	_button::mouseHandler( event );
	
	return handled;
}

void _keyboardButton::init()
{
	this->unregisterEventHandler( mouseDoubleClick );
	this->registerEventHandler( mouseClick , &_keyboardButton::mouseHandler );
}