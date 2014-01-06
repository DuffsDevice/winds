// Check if already included
#ifndef _WIN_G_FILEVIEW_
#define _WIN_G_FILEVIEW_

#include "_type/type.gadget.h"
#include "_type/type.flexptr.h"
#include "_gadget/gadget.scrollArea.h"
#include "_gadget/gadget.label.h"
#include "_gadget/gadget.fileobject.h"

class _fileView : public _scrollArea {
	
	private:
	
		_direntry					directory;
		_fileViewType				viewType;
		flex_ptr<_vector<_literal>>	filemask;
		bool						singleClickToExecute;
		
		void generateChildren();
		
	public:
	
		//! Method to set the Path
		void setPath( const string& path );
		
		//! Method to get the Path
		string getPath(){ return this->directory.getFileName(); }
		
		//! Tell the _fileview to reduce 
		void setFileMask( _vector<_literal> allowedExtensions ){
			filemask = new _vector<_literal>( move(allowedExtensions) );
			this->generateChildren();
		}
		
		//! Full Ctor
		_fileView( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string path , _fileViewType viewtype , _vector<_literal> allowedExtensions = {} , _scrollType scrollTypeX = _scrollType::meta , _scrollType scrollTypeY = _scrollType::meta , bool singleClickToExecute = false , _style&& style = _style() );
		
		//! Simple Ctor
		_fileView( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string path , _vector<_literal> allowedExtensions = {} , bool singleClickToExecute = false , _style&& style = _style() ) :
			_fileView( x , y , width , height , path , _fileViewType::list , move(allowedExtensions) , _scrollType::meta , _scrollType::meta , singleClickToExecute , (_style&&)style )
		{ }
		
		//! Dtor
		~_fileView();
};

#endif