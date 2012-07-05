#include "_type/type.eventArgs.h"

_gadgetEventArgs::_gadgetEventArgs( void* dest ){
	this->reset( dest );
}

void _gadgetEventArgs::reset( void* dest ){
	this->src = nullptr;
	this->dest = dest;
	this->posX = 0;
	this->posY = 0;
	this->keyCode = 0;
	this->currentKeyCodes = 0;
	this->heldTime = 0;
	this->bubble = false;
	this->damagedRects = _area();
}

void* _gadgetEventArgs::getDestination(){ return this->dest; }

void* _gadgetEventArgs::getSource(){ return this->src; }

void _gadgetEventArgs::setDestination( void* newVal ){ this->dest = newVal; }

void _gadgetEventArgs::setSource( void* newVal ){ this->src = newVal; }

_coord _gadgetEventArgs::getPosX( bool effectivePos ){ if( effectivePos ) return this->effectiveX; return this->posX; }

_coord _gadgetEventArgs::getPosY( bool effectivePos ){ if( effectivePos ) return this->effectiveY; return this->posY; }

void _gadgetEventArgs::setPosX( _coord pos ){ this->posX = pos; }

void _gadgetEventArgs::setPosY( _coord pos ){ this->posY = pos; }

void _gadgetEventArgs::setEffectivePosX( _coord pos ){ this->effectiveX = pos; }

void _gadgetEventArgs::setEffectivePosY( _coord pos ){ this->effectiveY = pos; }

_coord _gadgetEventArgs::getDeltaX(){ return this->deltaX; }

_coord _gadgetEventArgs::getDeltaY(){ return this->deltaY; }

void _gadgetEventArgs::setDeltaX( _coord pos ){ this->deltaX = pos; }

void _gadgetEventArgs::setDeltaY( _coord pos ){ this->deltaY = pos; }

_key _gadgetEventArgs::getKeyCode(){ return this->keyCode; }

void _gadgetEventArgs::setKeyCode( _key code ){ this->keyCode = code; }

_u32 _gadgetEventArgs::getHeldTime(){ return this->heldTime; }

void _gadgetEventArgs::setHeldTime( _u32 heldTime ){ this->heldTime = heldTime; }

_key _gadgetEventArgs::getCurrentKeyCodes(){ return this->currentKeyCodes; }

void _gadgetEventArgs::setCurrentKeyCodes( _key code ){ this->currentKeyCodes = code; }

_area _gadgetEventArgs::getDamagedRects(){ return this->damagedRects; };

void _gadgetEventArgs::setDamagedRects( _area rects ){ this->damagedRects = rects; };

bool _gadgetEventArgs::isBubblePrevented(){ return this->bubble; }

void _gadgetEventArgs::preventBubble( bool bR ){ this->bubble = bR; }

bool _gadgetEventArgs::hasClippingRects(){ return !this->damagedRects.empty(); }

_s32 _gadgetEventArgs::getIntValue(){ return this->intValue; }

string _gadgetEventArgs::getStrValue(){ return this->strValue; }