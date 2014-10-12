#ifndef _WIN_PROG_MAPPER_PROGOBJECT_
#define _WIN_PROG_MAPPER_PROGOBJECT_

#include <_gadget/gadget.button.h>
#include <_gadget/gadget.image.h>
#include <_gadget/gadget.window.h>
#include <_type/type.direntry.h>

class PROG_Mapper_Object : public _gadget
{
	private:
		
		_bitmap		progImage;
		string		progAuthor;
		string		progName;
		_direntry	programFile;
		
		static _callbackReturn refreshHandler( _event event );
		
	public:
		
		PROG_Mapper_Object( _length width , string progPath );
		
		//! Get Path to Program
		const string& getPath() const { return this->programFile.getFileName(); }
};

#endif