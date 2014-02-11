#ifndef _WIN_T_REGISTRY_
#define _WIN_T_REGISTRY_

#include "_type/type.h"
#include "_type/type.ini.file.h"
#include "_type/type.language.h"

class _registry : public _iniFile
{
	private:
	
		_language	language;
		_unixTime	systemStartTime;
	
	public:
		
		//! Get Language
		_language getLanguage() const { return this->language; }
		
		//! Set Language
		void setLanguage( _language lang ){ this->language = lang; }
		
		//! Get Execution command for a specific filetype
		const string& getFileTypeHandler( const string& extension ) const ;
		
		//! Get File image connected to a specific file extension
		_bitmap getFileTypeImage( const string& extensions , _mimeType mimeType ) const ;
		
		//! Indicate that the DS Was shut down correctly
		void indicateSafeShutdown();
		
		//! Check if WinDS was shut down correctly the last time it was turned off
		bool wasSafelyShutDown();
		
		//! Dtor
		~_registry();
		
		//! Ctor
		_registry( string path );
};

#endif