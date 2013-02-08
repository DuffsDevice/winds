#ifndef _WIN_F_SHORTCUT_
#define _WIN_F_SHORTCUT_

#include "_type/type.h"
#include "_type/type.direntry.h"
#include "_type/type.ini.h"
using namespace std;

/*#define GUID_1 0x01
#define GUID_2 0x14
#define GUID_3 0x02
#define GUID_4 0x00
#define GUID_5 0x00
#define GUID_6 0x00
#define GUID_7 0x00
#define GUID_8 0x00
#define GUID_9 0xc0
#define GUID_10 0x00
#define GUID_11 0x00
#define GUID_12 0x00
#define GUID_13 0x00
#define GUID_14 0x00
#define GUID_15 0x00
#define GUID_16 0x46


struct _linkTarget{
	_u16	targetSize;
		
struct _linkTargetList{
	_u16	listSize; // size in bytes of the following id-lists
	list<_linkTarget> targets;
	_u16 	endOfList; // Always Zero
}
struct _shortcutFileAttrs{
	_u8 targedReadOnly : 1;
	_u8 targetHidden : 1;
	_u8 targetSystemFile : 1;
	_u8 targetVolume : 1;
	_u8 targetDirectory : 1;
	_u8 targetArchive : 1; // Target has been modified since last backup (???)
	_u8 targetEncrypted : 1;
	_u8 targetNromal : 1; // (???)
	_u8 targetTemporary : 1;
	_u8 targetSparseFile : 1;
	_u8 targetHasReparsePointData : 1;
	_u8 targetIsCompressed : 1;
	_u8 targetOffline : 1;
}__attribute__(( packed ));

struct _shortcutFlags{
	_u8	hastargetIDList : 1; // Most important!
	_u8 pointsToFD : 1; // Points to a File or Directory
	_u8 hasDescString : 1;
	_u8 relativePath : 1;
	_u8 hasWorkingDirectory : 1;
	_u8 hasCmdArgs : 1;
	_u8 hasCustomIcon : 1;
} __attribute__(( packed ));

struct _shortcutHeader{
	_u32 	upperL; // Specifies a Link
	_u64	GUID_one; // Class Identifier
	_u64	GUID_two; // Class Identifier part two
	_shortcutFlags 		linkFlags;
	_shortcutFileAttrs 	fileAttributes;
	_u32	time1; // Creation Time (UTC)
	_u32	time2; // Modification Time (UTC)
	_u32	time3; // Last Access Time (UTC)
	_u32	fileLength;
	_u32	iconIndex; // Zero if flags_bits::hasCustomIcon is 'true'
	_u32	showWndValue; // ???
	_u8		hotKey;
	_u8		reserved1;
	_u32	reserved2; // Always Zero
	_u32	reserved3; // Always Zero
}__attribute__(( packed ));*/

class _shortcut : public _direntry {
	
	private:
		
		_direntry	destination;
		
		_bitmap		image;
		
	public:
		
		_shortcut( string fn );
		
		//! Get the Destination the shortcut pointes to
		_direntry getDestination();
		
		bool open();
		
		const string getExtension() const;
		
		_bitmap getFileImage();
};

#endif