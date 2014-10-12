#include <_lua/lua.lunar.h>

namespace LunarHelper{
	std::map<std::string,bool(*)(const char*)> className2checkIfBaseFunction;
}