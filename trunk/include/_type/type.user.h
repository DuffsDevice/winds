#ifndef _WIN_T_USER_
#define _WIN_T_USER_

#include "_type/type.h"
#include "_type/type.registry.h"

class _user : public _registry
{
	private:
		
		string userName;
	
	public:
		
		// Touch Parameters
		_u32 	mDD;
		_u32 	mCC;
		_u32 	mDC;
		_u32 	mDA;
		_u32 	kRD;
		_u32 	kRS;
		_u32 	fOH;
		_u32 	sOH;
		
		_bitmap* userLogo;
		_bitmap* userImage;
		
		_user( string username );
		
		~_user();
		
		string getUsername();
		
		bool checkPassword( string pw );
		
		bool hasPassword();
		
		void setPassword( string pw );
		
		void setUsername( string un );
		
		void remove();
		
		_s32 getIntAttr( string idx );
		
		string getStrAttr( string idx );
};

#endif
	