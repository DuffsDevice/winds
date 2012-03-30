#include "_type/type.eventArgs.h"

_gadgetEventArgs::_gadgetEventArgs( void* dest ){
	this->src = nullptr;
	this->dest = dest;
	this->posX = 0;
	this->posY = 0;
	this->keyCode = 0;
	this->currentKeyCodes = 0;
	this->heldTime = 0;
	this->bubbleRefresh = false;
	this->damagedRects = _area();
}

void _gadgetEventArgs::reset(){
	this->src = nullptr;
	this->dest = nullptr;
	this->posX = 0;
	this->posY = 0;
	this->keyCode = 0;
	this->currentKeyCodes = 0;
	this->heldTime = 0;
	this->bubbleRefresh = false;
	this->damagedRects = _area();
}

void* _gadgetEventArgs::getDestination(){ return this->dest; }

void* _gadgetEventArgs::getSource(){ return this->src; }

void _gadgetEventArgs::setDestination( void* newVal ){ this->dest = newVal; }

void _gadgetEventArgs::setSource( void* newVal ){ this->src = newVal; }

_coord _gadgetEventArgs::getPosX(){ return this->posX; }

_coord _gadgetEventArgs::getPosY(){ return this->posY; }

void _gadgetEventArgs::setPosX( _coord pos ){ this->posX = pos; }

void _gadgetEventArgs::setPosY( _coord pos ){ this->posY = pos; }

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

bool _gadgetEventArgs::isBubbleRefresh(){ return this->bubbleRefresh; }

void _gadgetEventArgs::setBubbleRefresh( bool bR ){ this->bubbleRefresh = bR; }


void _gadgetEventArgs::dump(){
	printf("d_eargs: iBR:%d,X:%d,Y:%d\n",this->bubbleRefresh,this->posX,this->posY);
}