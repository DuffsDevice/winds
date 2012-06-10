#include "_freetype/freetypefacemanager.h"
#include "_type/type.bitmap.h"

// FreeType assumes that structures allocated by the user and passed as
// arguments are zeroed out except for the actual data. In other words, 
// it is recommended to use ‘calloc’ (or variants of it) instead of ‘malloc’
// for allocation.

FreeTypeFaceManager::~FreeTypeFaceManager() {
	while( root )
	{
		FreeTypeFaceManagerItem* temp = root->next;
		delete root->faceID;
		delete root;
		root = temp;
	};
	root = nullptr;
}

const FaceID* FreeTypeFaceManager::addFace( string path, const _u8 index) {

	FaceID* faceID = new FaceID;

	// Copy path into faceID struct
	faceID->path = path;

	faceID->index = index;
	faceID->charMapIndex = 0; // This wil get set later (on demand by the face requester).   

	FreeTypeFaceManagerItem* newItem = new FreeTypeFaceManagerItem;
	newItem->faceID = faceID;
	newItem->next = root;
	root = newItem;
	
	return faceID;
}
