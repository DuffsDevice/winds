// Check if already included
#ifndef _WIN_G_FILEVIEW_
#define _WIN_G_FILEVIEW_

#include "_type/type.gadget.h"
#include "_gadget/gadget.scrollArea.h"
#include "_gadget/gadget.label.h"
#include "_gadget/gadget.fileobject.h"

class _fileview : public _scrollArea {
	
	private:
	
		_direntry		directory;
		_fileviewType	viewType;
		
		_gadgetList		files;
		
		void 			generateChildren();
		
	public:
	
		// Method to set the Path
		void 			setPath( const string& path );
		
		string			getPath(){ return this->directory.getFileName(); }
		
		_fileview( _length width , _length height , _coord x , _coord y , string path , _fileviewType viewtype = _fileviewType::liste , _scrollType scrollTypeX = _scrollType::prevent , _scrollType scrollTypeY = _scrollType::scroll , _gadgetStyle style = _defaultStyle_ );
		
		~_fileview();
		
};

#endif