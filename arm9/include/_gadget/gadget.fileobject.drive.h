// Check if already included
#ifndef _WIN_G_FILEOBJECT_DRIVE_
#define _WIN_G_FILEOBJECT_DRIVE_

#include <_type/type.gadget.h>
#include <_type/type.direntry.h>
#include <_gadget/gadget.label.h>
#include <_gadget/gadget.fileobject.h>

class _driveFileObject : public _fileObject
{
	private:
		
		// Handler for specific events
		static _callbackReturn refreshHandler( _event );
		static _callbackReturn updateHandler( _event );
		
		// Default Menu to Open on right Click
		static _uniquePtr<_menu>	defaultMenu;
		static void		defaultMenuHandler( _u16 listIndex , _u16 entryIndex );
		void			initializeMenu();
		
	public:
		
		//! Ctor
		_driveFileObject( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string file , _fileViewType viewtype = _fileViewType::list , _style&& style = _style() );
};

#endif