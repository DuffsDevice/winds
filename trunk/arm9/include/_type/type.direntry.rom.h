#ifndef _WIN_T_DIRENTRY_NDSEXE_
#define _WIN_T_DIRENTRY_NDSEXE_

#include <_type/type.h>
#include <_type/type.direntry.h>
#include <_type/type.ini.h>
#include <_type/type.arguments.h>

class _ndsExecuteable : public _direntry {
	
	private:
		
		_bitmap	image;
		_bitmap	fileImage;
		bool	homeBrew;
		
		void readAttributes() const ;
		
	public:
		
		//! Ctor
		_ndsExecuteable( string fn );
		
		//! Returns, whether the rom is homebrew
		bool isHomeBrew(){
			return this->homeBrew;
		}
		
		//! Get the executeable image
		_bitmap getImage() const {
			return this->image;
		}
		
		//! Get the (scaled) executeable file-image
		_bitmap getFileImage() const {
			return this->fileImage;
		}
};

#endif