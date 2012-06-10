#ifndef _FREETYPE_FACE_MANAGER_H_
#define _FREETYPE_FACE_MANAGER_H_

#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_CACHE_H 
#include FT_BITMAP_H

#include "_type/type.h"

/** 
 * FaceID is a Woopsi structure identifying a face (typically a pointer to
 * a font path and an index (fonts may contain multiple faces)).
 * @param path Pointer to the path of the font.
 * @param index Index of the face in the font (0 is the default....beware,
 * if you ask for an index that is not in the font it will mess things up
 * (chars might not display, I experienced this)).
 * @param charMapIndex caches the charMapIndex value (we are using Unicode)
 * so it doesn't have to be computed each time.
 */
struct FaceID {
	_string path;
	_u8 index;
	_u32 charMapIndex;
};

/** 
 * FaceManager is a basic linked list containing the installed fonts.  You
 * are supposed to add all the fonts that you are going to use.  You are not
 * supposed to remove fonts once you have added them.  FreeType expects this
 * data to be persistent and not to change (if you really need to change it,
 * it's possible but you have to tell FreeType that you did).
 */
struct FreeTypeFaceManagerItem {
	FaceID* faceID;
	FreeTypeFaceManagerItem *next;
};

class FreeTypeFaceManager {
	public: 
		/**
		 * Constructor.
		 */
		 FreeTypeFaceManager() :
			root( nullptr )
		{ }

		/**
		 * Destructor.
		 */
		~FreeTypeFaceManager();

		/**
		 * Stores a face inside the dynamic array.
		 * @param path Pointer to the path of the font.
		 * @param index Index of the face in the fonts as it may have many
		 * (default is 0).
		 * @return The index of the face in the dynamic array.
		 */           
		const FaceID* addFace( string path, const _u8 index = 0 );

	private:
		FreeTypeFaceManagerItem* root;		/**< Internal stack of data items. */
};

#endif
