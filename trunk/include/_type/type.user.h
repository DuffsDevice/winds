#ifndef _WIN_T_USER_
#define _WIN_T_USER_

#include "_type/type.h"
#include "_type/type.registry.h"

class _user : public _registry
{
	private:
		
		string		folderName;
		_bitmap		userLogo;
	
	public:
		
		static _bitmap getUserLogoFromImage( const _bitmap& image );
		static _bitmap getUserImage( string path );
		
		// Touch Parameters
		_u32 	mDD, mCC, mDC, mDA, kRD, kRS, fOH, sOH, sFE, sBTC;
		
		_user( string username );
		
		~_user();
		
		string getUsername();
		
		const _bitmap& getLogo(){ return this->userLogo; }
		
		bool checkPassword( string pw );
		
		bool hasPassword();
		
		void setPassword( string pw );
		
		void setUsername( string un );
		
		void remove();
		
		_s32 getIntAttr( string idx );
		
		string getStrAttr( string idx );
};

#endif
	