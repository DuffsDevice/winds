// Check if already included
#ifndef _WIN_G_FILEVIEW_
#define _WIN_G_FILEVIEW_

#include "_type/type.gadget.h"
#include "_type/type.directory.h"
#include "_gadget/gadget.label.h"
#include "_gadget/gadget.fileobject.h"
#include <string>

class _fileview : public _gadget {
	
	private:
	
		_directory*		directory;
		_fileviewType	viewType;
		
	public:
	
		_fileview( _length width , _length height , _coord x , _coord y , _directory* dir , _fileviewType viewtype = _fileviewType::list , _gadgetStyle style = _defaultStyle_ );
		
};

#endif