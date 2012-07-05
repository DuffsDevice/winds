#include "_type/type.event.h"

#include <stdio.h>

using namespace std;

_gadgetEvent::_gadgetEvent( _gadget* src , _gadgetEventType type , _gadgetEventArgs args ){
	this->type = type;
	this->args = args;
	this->args.src = src;
}

_gadgetEvent::_gadgetEvent( _gadgetEventType type , _gadgetEventArgs args ){
	this->type = type;
	this->args = args;
}

_gadgetEvent _gadgetEvent::refreshEvent( _gadget* src , _area damagedRects , _gadgetEventArgs args ){
	_gadgetEvent e = _gadgetEvent( src , "refresh" , args );
	e.args.damagedRects = damagedRects;
	e.args.bubble = true;
	e.args.src = src;
	return e;
}

_gadgetEvent _gadgetEvent::dialogClose( _gadget* src , _s32 intValue , string strValue , _gadgetEventArgs args ){
	_gadgetEvent e = _gadgetEvent( src , "dialogClose" , args );
	e.args.src = src;
	e.args.intValue = intValue;
	e.args.strValue = strValue;
	return e;
}

void _gadgetEvent::setType( _gadgetEventType type ){
	this->type = type;
}

_gadgetEventType _gadgetEvent::getType(){
	return this->type;
}

void _gadgetEvent::setArgs( _gadgetEventArgs args ){
	this->args = args;
}

_gadgetEventArgs& _gadgetEvent::getArgs(){
	return this->args;
}

void _gadgetEvent::setGadget( _gadget* that ){ this->that = that; }

_gadget* _gadgetEvent::getGadget(){ return this->that; }

map<string,_gadgetEventReturnType> string2eventReturnType = {
	{ "handled" , handled },
	{ "use_normal" , use_normal },
	{ "use_default" , use_default },
	{ "not_handled" , not_handled },
};

map<_gadgetEventReturnType,string> eventReturnType2string = {
	{ handled , "handled" },
	{ use_normal , "use_normal" },
	{ use_default , "use_default" },
	{ not_handled , "not_handled" },
};