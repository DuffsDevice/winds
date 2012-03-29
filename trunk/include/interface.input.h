#ifndef _WIN_INTERFACE_INPUT_
#define _WIN_INTERFACE_INPUT_

#include "_type/type.h"
#include <string.h>

class _interface_input
{
	private:
		
		_align 	align;
		_valign	vAlign;
		
		string 	title;
		
		_s32 	intValue;
		string 	strValue;
	
	public:
		
		virtual void setAlign( _align align ){ this->align = align; }
		
		virtual _align getAlign(){ return this->align; }
		
		virtual void setVAlign( _valign vAlign ){ this->vAlign = vAlign; }
		
		virtual _valign getVAlign(){ return this->vAlign; }
		
		virtual void setIntValue( _s32 val ){ this->intValue = val; }
		
		virtual void setStrValue( string val ){ this->strValue = val; }
		
		virtual void setTitle( string val ){ this->title = val; }
		
		virtual string getStrValue(){ return strValue; }
		
		virtual _s32 getIntValue(){ return intValue; }
		
		virtual string getTitle(){ return title; }
		
		_interface_input( string val ){ this->setTitle(val); }
};

#endif