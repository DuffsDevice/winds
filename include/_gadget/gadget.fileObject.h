// Check if already included
#ifndef _WIN_G_FILEOBJECT_
#define _WIN_G_FILEOBJECT_

#include "_type/type.gadget.h"
#include "_type/type.file.h"
#include "_type/type.runtimeAttributes.h"
#include "_gadget/gadget.label.h"
#include "_gadget/gadget.imagegadget.h"
#include <string>

typedef enum{
	detail = 0,
	list = 1,
	symbol_small = 2,
	symbol_big = 3
}_fileviewType;

class _fileobject : public _gadget {
	
	private:
	
		// My Data...
		_file*			file;
		_fileviewType	viewType;
		
		_label*			label;
		_imagegadget*	icon;
		
		static _gadgetEventReturnType refreshHandler( _gadgetEvent event );
		static _gadgetEventReturnType dragHandler( _gadgetEvent event );
		static _gadgetEventReturnType mouseHandler( _gadgetEvent event );
		
	public:
	
		_fileobject( _coord x , _coord y , _file* dir , _fileviewType viewtype = _fileviewType::list , _gadgetStyle style = _defaultStyle_ );
		
};

#endif