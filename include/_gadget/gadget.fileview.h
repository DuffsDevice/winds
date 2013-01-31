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
		
		// Full Ctor
		_fileview( _length width , _length height , _coord x , _coord y , string path , _fileviewType viewtype , _scrollType scrollTypeX = _scrollType::meta , _scrollType scrollTypeY = _scrollType::meta , _style style = _style() );
		
		// Simple Ctor
		_fileview( _length width , _length height , _coord x , _coord y , string path , _style style = _style() ) :
			_fileview( width , height , x , y , path , _fileviewType::liste , _scrollType::meta , _scrollType::meta , style )
		{ }
		
		~_fileview();
		
};

#endif