#include "_type/type.event.h"

#include <stdio.h>

using namespace std;

_gadgetEvent::_gadgetEvent( _gadget* src , _gadgetEventType type , _gadgetEventArgs args ){
	this->type = type;
	this->args = args;
	this->args.setSource( src );
}

_gadgetEvent::_gadgetEvent( _gadgetEventType type , _gadgetEventArgs args ){
	this->type = type;
	this->args = args;
}

_gadgetEvent::_gadgetEvent( _gadget* src , _gadgetEventType type , _rect rect , _area damagedRects , _gadgetEventArgs args ){
	this->type = type;
	this->args = args;
	this->args.setRefreshRects( rect );
	this->args.setDamagedRects( damagedRects );
	this->args.setBubbleRefresh( true );
	this->args.setSource( src );
}

_gadgetEvent::_gadgetEvent( _gadget* src , _gadgetEventType type , _gadget* dest , _gadgetEventArgs args ){
	this->type = type;
	this->args = args;
	this->args.setDestination( dest );
	this->args.setSource( src );
}

void _gadgetEvent::setType( _gadgetEventType type ){
	this->type = type;
}

_gadgetEventType _gadgetEvent::getType(){
	return this->type;
}

const string _gadgetEvent::getTypeString(){
	return eventType2string[ this->type ];
}

void _gadgetEvent::setArgs( _gadgetEventArgs args ){
	this->args = args;
}

_gadgetEventArgs& _gadgetEvent::getArgs(){
	return this->args;
}

void _gadgetEvent::dump(){
	printf("d_event:%s",this->getTypeString().c_str());
	this->args.dump();
}

void _gadgetEvent::setGadget( _gadget* that ){ this->that = that; }

_gadget* _gadgetEvent::getGadget(){ return this->that; }

map<string,_gadgetEventType> string2eventType = {
	{ "none" , none },
	{ "mouseClick" , mouseClick },
	{ "mouseDown" , mouseDown },
	{ "mouseUp" , mouseUp },
	{ "mouseDoubleClick" , mouseDoubleClick },
	{ "keyDown" , keyDown },
	{ "keyUp" , keyUp },
	{ "keyClick" , keyClick },
	{ "dragging" , dragging },
	{ "dragStart" , dragStart },
	{ "dragStop" , dragStop },
	{ "refresh" , refresh },
	{ "individual" , individual },
	{ "change" , change },
	{ "all" , all }
};

map<string,_gadgetEventReturnType> string2eventReturnType = {
	{ "handled" , handled },
	{ "handled_no_delete" , handled_no_delete },
	{ "use_default" , use_default },
	{ "not_handled" , not_handled },
	{ "not_handled_no_delete" , not_handled_no_delete }
};

map<_gadgetEventType,string> eventType2string = {
	{ none , "none" },
	{ mouseClick , "mouseClick" },
	{ mouseDown , "mouseDown" },
	{ mouseUp , "mouseUp" },
	{ mouseDoubleClick , "mouseDoubleClick" },
	{ keyDown , "keyDown" },
	{ keyUp , "keyUp" },
	{ keyClick , "keyClick" },
	{ dragging , "dragging" },
	{ dragStart , "dragStart" },
	{ dragStop , "dragStop" },
	{ refresh , "refresh" },
	{ individual , "individual" },
	{ change , "change" },
	{ all , "all" }
};

map<_gadgetEventReturnType,string> eventReturnType2string = {
	{ handled , "handled" },
	{ handled_no_delete , "handled_no_delete" },
	{ use_default , "use_default" },
	{ not_handled , "not_handled" },
	{ not_handled_no_delete , "not_handled_no_delete" }
};