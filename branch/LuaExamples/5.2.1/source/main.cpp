#include "lua.hpp"
#include <malloc.h>
#include "lualib.h"
#include "lunar.h"

#include "nds.h"

class object{
	private:
		int t;
	public:
		
		bool isPrecious;
		bool isExisting;
		
		object( lua_State* L , bool prec ) : isPrecious( prec ){
			t = 5;
			printf("Creating: %d\n",prec);
		}
		
		int setT( lua_State* s )
		{
			t = luaL_checkint( s , 1 );
			return 0;
		}
		
		int getNeu( lua_State* s ){
			Luna<object>::createNew( s );
		}
		
		static Luna<object>::FunctionType Functions[];
		static Luna<object>::PropertyType Properties[];
		static char* className;
};

Luna<object>::FunctionType object::Functions[] = {
	{ "setT" , &object::setT }
	, { "getNeu" , &object::getNeu }
	, { NULL , NULL }
};
Luna<object>::PropertyType object::Properties[] = {
	{ NULL , NULL , NULL }
};
char* object::className = "object";

static void *l_alloc (void *ud, void *ptr, size_t osize,size_t nsize) {
	if (nsize == 0) {
		free(ptr);
		return NULL;
	}
	else
		return realloc(ptr, nsize);
}

int main( int argc , char* argv[] )
{
	consoleDemoInit();
	
	lua_State* L = luaL_newstate();
	luaL_openlibs( L );
	Luna<object>::Register( L , "" );
	
	luaL_dostring( L , "l = object();l.setT(5);t = l.getNeu()");
	
	while(true)
		swiWaitForVBlank();
	
	return 0;
}
