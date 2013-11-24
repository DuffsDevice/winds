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
		
		void 			generateChildren( bool singleClickToExecute );
		
	public:
	
		//! Method to set the Path
		void 			setPath( const string& path );
		
		//! Method to get the Path
		string			getPath(){ return this->directory.getFileName(); }
		
		//! Full Ctor
		_fileview( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string path , _fileviewType viewtype , _scrollType scrollTypeX = _scrollType::meta , _scrollType scrollTypeY = _scrollType::meta , bool singleClickToExecute = false , _style&& style = _style() );
		
		//! Simple Ctor
		_fileview( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string path , bool singleClickToExecute = false , _style&& style = _style() ) :
			_fileview( x , y , width , height , path , _fileviewType::list , _scrollType::meta , _scrollType::meta , singleClickToExecute , (_style&&)style )
		{ }
		
		//! Dtor
		~_fileview();
};

#endif