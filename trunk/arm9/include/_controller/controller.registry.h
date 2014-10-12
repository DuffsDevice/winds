#ifndef _WIN_C_REGISTRY_
#define _WIN_C_REGISTRY_

#include <_type/type.h>
#include <_type/type.controller.h>
#include <_type/type.ini.file.h>
#include <_type/type.uniqueptr.h>
#include <_type/type.user.h>

struct lua_State;

class _registryController : public _controller
{
	private:
		
		static _uniquePtr<_iniFile>					systemRegistry;
		static _uniquePtr<_user>					userRegistry;
		static _uniquePtr<_iniFile>					fileTypeRegistry;
		static bool									correctShutDown;
		static bool									safeBootMode;
		static _unixTime							systemStartTime;
		
		// Map to cache file images
		static _map<string,_pair<string,_bitmap>>	extension2UrlAndimage;
		
		// Methods that will load the appropriate data into the registry objects
		static void prepareSystemRegistry();
		static void prepareFileTypeRegistry();
		
	public:
		
		//! Inits the debug system
		static bool		init();
		
		//! Shuts down the debug system
		static void		end();
		
		//! Returns whether a specific user is logged in
		static bool		isUserLoggedIn(){ return userRegistry != nullptr; }
		
		//! Get the Current User's Registry instance
		static _user&	getUserRegistry(){
			return *userRegistry;
		}
		
		//! Get the System's Registry
		static _ini&	getSystemRegistry(){
			return *systemRegistry;
		}
		
		//! Get the Registry that contains information about certain file-types
		static _ini&	getFileTypeRegistry(){
			return *fileTypeRegistry;
		}
		
		//! Check if the ds was safely shut down
		static bool		wasCorrectlyShutDown(){
			return correctShutDown;
		}
		
		//! Check if the ds was safely shut down
		static bool		isSafeBootMode(){
			return safeBootMode;
		}
		
		//! Indicate that the DS has been shut down correctly
		static void		indicateSafeShutdown();
		
		//! Get Execution command for the specified filetype extensions
		static const string&	getFileTypeHandler( const string& extension );
		
		/**
		 * Get File image connected to the specified filetype extensions
		 * @note the mimeType-parameter is a hint that safes comuting time
		 */
		static _bitmap	getFileTypeImage( const string& extensions , _mimeType mimeType );
		
		//! Makes the specified command the default handler to open files of the specified file type
		static void		setFileTypeHandler( const string& extension , const string& command );
		
		//! Loads a user configuration (.usr-file) from the supplied path
		static void		setUserRegistry( _uniquePtr<_user> registry );
		static void		setUserRegistry( decltype(nullptr) ){ setUserRegistry( _uniquePtr<_user>( nullptr ) ); }
		static void		setUserRegistry( string registryPath ){ setUserRegistry( new _user( registryPath ) ); }
};

#endif