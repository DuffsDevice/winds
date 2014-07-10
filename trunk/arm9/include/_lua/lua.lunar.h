// Check if already included
#ifndef _WIN_L_LUNAR_
#define _WIN_L_LUNAR_

#include "_lua/lua.h"
#include <type_traits>
#include <tuple>
#include <map>
#include <string>

namespace LunarHelper
{
	template <typename T>
	class HasStaticMethods{
		template<class C> static std::true_type test( decltype(&C::staticmethods) );
		template<class C> static std::false_type test(...);
	public:
		enum{ value = std::is_same<std::true_type,decltype(test<T>(nullptr))>::value };
	};
	
	template <typename T>
	class HasConstructor : public std::is_constructible<T,lua_State*>{};
	
	template <typename T>
	class HasBaseClasses{
		template<class C> static std::true_type test( typename C::baseclasses* );
		template<class C> static std::false_type test(...);
	public:
		enum{ value = std::is_same<std::true_type,decltype(test<T>(nullptr))>::value };
	};
	
	constexpr bool isFunc( int index ){ return index & ( 1 << 8 ); }
	constexpr int index2func( int index ){ return index | ( 1 << 8 ); }
	constexpr int func2index( int index ){ return index & (~( 1 << 8 )); }
	
	extern std::map<std::string,bool(*)(const char*)> className2checkIfBaseFunction;
	
	template<class Class>
	using MemFuncPtr	= int (Class::*)( lua_State* );
	using FuncPtr		= int (*)( lua_State* );
	
	template<class Class>
	struct PropertyType{
		const char*	name;
		MemFuncPtr<Class>	getter;
		MemFuncPtr<Class>	setter;
	};
	
	template<class Class>
	struct FunctionType{
		const char*			name;
		MemFuncPtr<Class>	func;
	};
	
	struct StaticType{
		const char*			name;
		FuncPtr				func;
	};
	
	static unused void registerStaticClassHelper( lua_State* state , const char* className , StaticType* staticmethods )
	{
		// Create table that will hold all static methods
		lua_newtable( state );
		int tableIndex = lua_gettop( state );
		
		for( int i = 0 ; staticmethods[i].name ; i++ )
		{
			lua_pushstring( state , staticmethods[i].name ); // Push Key
			lua_pushcfunction( state , staticmethods[i].func ); // Push Value
			lua_rawset( state , tableIndex ); // Push into table
		}
		
		// Set metatable of table and pop it from stack
		lua_setmetatable( state , tableIndex );
		
		// Adds the currently created table to the global namespace and pops it from stack
		lua_setglobal( state , className );
	}
	
	static unused void registerClassNameHelper( lua_State* state , const char* className , StaticType* staticmethods , lua_CFunction constructor )
	{
		// Create table that will hold all static methods
		lua_newtable( state );
		int tableIndex = lua_gettop( state );
		
		for( int i = 0 ; staticmethods[i].name ; i++ )
		{
			lua_pushstring( state , staticmethods[i].name ); // Push Key
			lua_pushcfunction( state , staticmethods[i].func ); // Push Value
			lua_rawset( state , tableIndex ); // Push into table
		}
		
		// Create table that will become our metatable
			lua_newtable( state );
			int metatableIndex = lua_gettop( state );
			
			// Push constructor
			lua_pushliteral( state , "__call" );
			lua_pushcfunction( state , constructor );
			lua_settable( state , metatableIndex );
		//
		
		// Set metatable of table and pop it from stack
		lua_setmetatable( state , tableIndex );
		
		// Adds the currently created table to the global namespace and pops it from stack
		lua_setglobal( state , className );
	}
}

template<class Class>
class Lunar
{
	template<typename> friend class Lunar;
	
	private:
		
		// Short typdefs
		template<typename C,typename R = int> using requireBaseClasses = typename std::enable_if<LunarHelper::HasBaseClasses<C>::value,R>::type;
		template<typename C,typename R = int> using requireNoBaseClasses = typename std::enable_if<!LunarHelper::HasBaseClasses<C>::value,R>::type;
		template<typename C,typename R = void> using requireStatics = typename std::enable_if<LunarHelper::HasStaticMethods<C>::value && LunarHelper::HasConstructor<C>::value,R>::type;
		template<typename C,typename R = void> using requirePureStatics = typename std::enable_if<LunarHelper::HasStaticMethods<C>::value && !LunarHelper::HasConstructor<C>::value,R>::type;
		template<typename C,typename R = void> using requireNoStatics = typename std::enable_if<!LunarHelper::HasStaticMethods<C>::value,R>::type;
		
		//
		// PROPERTY - GETTERS
		//
		template<int index, typename bases>
		struct propertyGetterBaseImpl{
			static int dispatcher( lua_State* state )
			{
				int result = Lunar::propertyGetterBaseImpl<index-1,bases>::dispatcher( state );	// Recursion
				if( result != -1 )																// Break up if we found the attribute
					return result;
				using baseType = typename std::tuple_element<index,bases>::type;				// Store current base class Type
				return Lunar<baseType>::propertyGetterImpl( state );						// Call property-getter
			}
		};
		
		template<typename bases>
		struct propertyGetterBaseImpl<0,bases>{
			static int dispatcher( lua_State* state ){
				using baseType = typename std::tuple_element<0,bases>::type;	// Store current base class Type
				return Lunar<baseType>::propertyGetterImpl( state );		// Call property-getter
			}
		};
		
		template<typename bases>
		struct propertyGetterBaseImpl<-1,bases>{
			static inline int dispatcher( lua_State* state ){ return -1; } // For empty tuples
		};
		
		// FRONT END FOR BACK END //
		// ~~~~~~~~~~~~~~~~~~~~~~ //
		template<typename TempClass>
		static inline requireBaseClasses<TempClass> propertyGetterBase( lua_State* state ){
			return propertyGetterBaseImpl<std::tuple_size<typename TempClass::baseclasses>::value - 1,typename TempClass::baseclasses>::dispatcher( state );
		}
		
		template<typename TempClass>
		static inline requireNoBaseClasses<TempClass> propertyGetterBase( lua_State* ){
			return -1;
		}
		
		static int propertyGetterImpl( lua_State* state )
		{
			// 1 -> Object
			// 2 -> Name of the attribute
			
			luaL_getmetatable( state , Class::className );	// Get metatable
			
			if( !lua_istable( state , -1 ) ){
				lua_pop( state , 1 );
				return -1;
			}
			
			lua_pushvalue( state , 2 );						// Push the name of the requested attribute
			lua_rawget( state , -2 );						// Get its index
			
			if( lua_isnumber( state , -1 ) )				// Check if we got a valid index
			{
				int index = lua_tonumber( state , -1 );		// Store index
				
				Class** instancePtr = static_cast<Class**>( lua_touserdata( state , 1 ) );	// Fetch instance
				Class* instance = *instancePtr;												// Unreference pointer to pointer
				
				// Check if the instance is valid
				if( !instancePtr || !instance ){
					luaL_error( state , "Internal error, no object given!" );
					return 0;
				}
				
				if( LunarHelper::isFunc(index) ) // A func
				{
					lua_pushnumber( state , LunarHelper::func2index(index) ); 			// Push the right function index as well as..
					lua_pushlightuserdata( state , instance ); 							// ..the corresponding object onto the stack
					lua_pushcclosure( state , &Lunar<Class>::functionDispatch , 2 );	// Push a C Closure having 2 upvalues (index and instance)
					return 1;															// Return a 'function'
				}
				else if( Class::properties[index].getter != nullptr ) // A normal attribute!
				{
					// Clean up the stack before we call the getter
					lua_pop( state , 2 );		// Pop metatable and index
					lua_remove( state , 1 );	// Remove userdata
					lua_remove( state , 1 );	// Remove [key]
					
					return (instance->*(Class::properties[index].getter))( state );
				}
			}
			else // Try base classes
			{
				lua_pop( state , 2 ); // Pop index and metatable
				return propertyGetterBase<Class>( state );
			}
			
			return -1; // Indicate not only we pushed nothing on the stack but also we have not found the attribute
		}
	
	private:
		
		//
		// PROPERTY - SETTERS
		//
		template<int index, typename bases>
		struct propertySetterBaseImpl{
			static int dispatcher( lua_State* state )
			{
				int result = Lunar::propertyGetterBaseImpl<index-1,bases>::dispatcher( state );	// Recursion
				if( result != -1 )																// Break up if we found the attribute
					return result;
				using baseType = typename std::tuple_element<index,bases>::type;				// Store current base class Type
				return Lunar<baseType>::propertySetterImpl( state );						// Call property-setter
			}
		};
		
		template<typename bases>
		struct propertySetterBaseImpl<0,bases>{
			static int dispatcher( lua_State* state ){
				using baseType = typename std::tuple_element<0,bases>::type;	// Store current base class Type
				return Lunar<baseType>::propertySetterImpl( state );		// Call property-setter
			}
		};
		
		template<typename bases>
		struct propertySetterBaseImpl<-1,bases>{
			static inline int dispatcher( lua_State* state ){ return -1; } // For empty tuples
		};
		
		static int propertySetterImpl( lua_State* state )
		{
			// 1 -> Object
			// 2 -> Name of the attribute
			// 2 -> Value to apply
			
			luaL_getmetatable( state , Class::className );	// Get metatable
			lua_pushvalue( state , 2 );						// Push the name of the requested attribute
			lua_rawget( state , -2 );						// Get its index
			
			if( lua_isnumber( state , -1 ) )				// Check if we got a valid index
			{
				int index = lua_tonumber( state , -1 );		// Store index
				
				// Fetch instance
				Class** instancePtr = static_cast<Class**>( lua_touserdata( state , 1 ) );
				Class* instance = *instancePtr;				// Unreference pointer to pointer
				
				// Check if the instance is valid
				if( !instance || !instancePtr ){
					luaL_error( state , "Internal error, no object given!" );
					return 0;
				}
				
				if( LunarHelper::isFunc(index) ){			// You cannot 'set' a member method!
					luaL_error( state , "Trying to set the method [%s] of class [%s]", Class::methods[ LunarHelper::func2index(index) ].name , Class::className );
					return 1;
				}
				else if( Class::properties[index].setter != nullptr ) // A normal attribute!
				{
					// Clean up the stack before we call the setter
					lua_pop( state , 2 );		// Pop metatable and index
					lua_remove( state , 1 );	// Remove userdata
					lua_remove( state , 1 );	// Remove [key]
					
					return (instance->*(Class::properties[index].setter))( state );
				}
			}
			else // Try base classes
			{
				lua_pop( state , 2 ); // Pop index and metatable
				return propertySetterBase<Class>( state );
			}
			
			return -1; // Indicate not only we pushed nothing on the stack but also we have not found the attribute
		}
		
		// FRONT END FOR BACK END //
		// ~~~~~~~~~~~~~~~~~~~~~~ //
		template<typename TempClass>
		static inline requireBaseClasses<TempClass> propertySetterBase( lua_State* state ){
			return propertySetterBaseImpl<std::tuple_size<typename TempClass::baseclasses>::value - 1,typename TempClass::baseclasses>::dispatcher( state );
		}
		
		template<typename TempClass>
		static inline requireNoBaseClasses<TempClass> propertySetterBase( lua_State* ){
			return -1;
		}
	
	private:
	
		//
		// CHECK FOR OBJECTS ON THE LUA STACK
		//
		template<int index, typename bases>
		struct recursiveCheckIfBaseImpl{
			static bool dispatcher( const char* possibleBaseClass )
			{
				bool result = Lunar::recursiveCheckIfBaseImpl<index-1,bases>::dispatcher( possibleBaseClass );	// Recursion
				if( result )																					// Break up if we found the attribute
					return result;
				using baseType = typename std::tuple_element<index,bases>::type;	// Store current base class Type
				return Lunar<baseType>::recursiveCheck( possibleBaseClass );		// Call property-getter
			}
		};
		
		template<typename bases>
		struct recursiveCheckIfBaseImpl<0,bases>{
			static bool dispatcher( const char* possibleBaseClass )
			{
				using baseType = typename std::tuple_element<0,bases>::type;	// Store current base class Type
				return Lunar<baseType>::recursiveCheck( possibleBaseClass );	// Check if 'baseType' is the base class we want to receive from lua
			}
		};
		
		template<typename bases>
		struct recursiveCheckIfBaseImpl<-1,bases>{
			static inline bool dispatcher( const char* ){ return nullptr; } // For empty tuples
		};
		
		// FRONT END FOR BACK END //
		// ~~~~~~~~~~~~~~~~~~~~~~ //
		template<typename TempClass>
		static inline requireBaseClasses<TempClass,bool> recursiveCheckIfBase( const char* possibleBaseClass ){
			return recursiveCheckIfBaseImpl<std::tuple_size<typename TempClass::baseclasses>::value - 1,typename TempClass::baseclasses>::dispatcher( possibleBaseClass );
		}
		
		template<typename TempClass>
		static inline requireNoBaseClasses<TempClass,bool> recursiveCheckIfBase( const char* ){
			return false;
		}
		
		// FRONT END FOR BACK END //
		// ~~~~~~~~~~~~~~~~~~~~~~ //
		static bool inline recursiveCheck( const char* possibleBaseClass )
		{
			// Check whether the base class we need equals this class
			if( strcmp( possibleBaseClass , Class::className ) == 0 )
				return true;
			// Check whether one of our base classes is the class we need
			return recursiveCheckIfBase<Class>( possibleBaseClass );
		}
	
	private:
		
		////////////////
		// MIDDLE END //
		////////////////
		
		static int propertyGetter( lua_State* state )
		{
			const char* attrName = lua_tostring( state , 2 );
			
			int result = propertyGetterImpl( state );
			if( result != -1 )
				return result;
			luaL_error( state , "Object has no attribute named '%s'!" , attrName ); // Throw error
			return 0;
		}
		
		static int propertySetter( lua_State* state )
		{
			const char* attrName = lua_tostring( state , 2 );
			
			int result = propertySetterImpl( state );
			if( result != -1 )
				return result;
			luaL_error( state , "Object has no attribute named '%s'!" , attrName ); // Throw error
			return 0;
		}
		
		static int functionDispatch( lua_State* state )
		{
			int index = (int)lua_tonumber( state , lua_upvalueindex(1) ); // Fetch function index
			
			// Fetch instance
			Class* instance = static_cast<Class*>( lua_touserdata( state , lua_upvalueindex(2) ) );
			
			// Check if the instance is valid
			if( !instance ){
				luaL_error( state , "Internal error in function dispatch: No object given!" );
				return 0;
			}
			
			return (instance->*(Class::methods[index].func))( state );
		}
		
		static int garbageCollector( lua_State* state )
		{
			// Fetch instance
			Class** instancePtr = static_cast<Class**>( lua_touserdata( state , -1 ) );
			Class* instance = *instancePtr;	// Unreference pointer to pointer
			
			// Check if the instance is valid
			if( !instance || !instancePtr ){
				luaL_error( state , "Internal error in garbage collector: No object given!" );
				return 0;
			}
			
			// Delete the instance
			delete instance;
			
			return 0;
		}
		
		static int toString( lua_State* state )
		{
			void** instancePtr = static_cast<void**>( lua_touserdata( state , -1 ) );
			void* instance = *instancePtr;
			
			if( instancePtr && instance )
				lua_pushfstring( state , "%s (%p)" , Class::className , (void*)instance );
			else
				luaL_error( state , "Internal error in string conversion: No object given!" );
			
			return 1;
		}
		
		static int dump( lua_State* state )
		{
			std::string output = "Dump of ";
			output += Class::className;
			
			if( Class::methods[0].name )
			{
				output += ":\nMethods:";
				for (int i = 0; Class::methods[i].name; i++)
				{
					output += "\n - ";
					output += Class::methods[i].name;
				}
			}
			
			if( Class::properties[0].name )
			{
				output += "\nProperties:";
				for (int i = 0; Class::properties[i].name; i++){
					output += "\n - ";
					output += Class::properties[i].name;
				}
			}
			
			lua_pushstring( state , output.c_str() );
			
			return 1;
		}
		
		static int constructorDeleteSelfReference( lua_State* state )
		{
			lua_remove( state , 1 );
			return constructor(state);
		}
		
		static int constructor( lua_State* state )
		{
			Class* instance = new Class(state); // Create new 'Class'
			Class** instancePtr = static_cast<Class**>( lua_newuserdata( state , sizeof(Class*) ) ); // Create pointer to pointer
			*instancePtr = instance; // Set that pointer to the newly allocated object
			
			// Set metatable
			luaL_getmetatable( state , Class::className ); 		// Fetch global metatable Class::classname
			lua_setmetatable( state , -2 );
			return 1;
		}
		
		template<typename TempClass>
		static inline requirePureStatics<TempClass> registerClassName( lua_State* state ){
			// This call was outsourced
			LunarHelper::registerStaticClassHelper(
				state
				, Class::className
				, Class::staticmethods
			);
		}
		
		template<typename TempClass>
		static inline requireStatics<TempClass> registerClassName( lua_State* state ){
			// This call was outsourced
			LunarHelper::registerClassNameHelper(
				state
				, Class::className
				, Class::staticmethods
				, &Lunar<Class>::constructorDeleteSelfReference
			);
		}
		
		template<typename TempClass>
		static inline requireNoStatics<TempClass> registerClassName( lua_State* state )
		{
			lua_pushcfunction( state , &Lunar<Class>::constructor );
			lua_setglobal( state , Class::className );
		}
	
	public:
		
		///////////////
		// FRONT END //
		///////////////
		
		template<typename...Bases> using			BaseClassType
			= std::tuple<Bases...>;
		typedef LunarHelper::PropertyType<Class>	PropertyType;
		typedef LunarHelper::FunctionType<Class>	FunctionType;
		typedef typename LunarHelper::StaticType	StaticType;
		
		// Push an object on the lua stack using a pointer to it
		static void push( lua_State* state , Class* instance )
		{
			if( instance ) // Make sure the pointer passed is not null
			{
				Class **instancePtr = static_cast<Class**>( lua_newuserdata( state , sizeof(Class*) ) ); // Create userdata
				*instancePtr = instance; // Write pointer
				
				luaL_getmetatable( state , Class::className );	// Get metatable
				lua_setmetatable( state , -2 );					// Apply to object
			}
			else
				lua_pushnil( state );
		}
		
		// Push an object on the lua stack
		template<typename... Arguments>
		static void emplace( lua_State* state , Arguments... parameters ){
			Lunar<Class>::push( state , new Class( std::forward<Arguments>(parameters)... ) );
		}
		
		// Check for objects of 'Class' on the Lua-Stack at position 'narg'
		static Class* check( lua_State* state , int narg )
		{
			Class* instance = lightcheck( state , narg );
			if( !instance ){
				lua_typeerror( state , narg , Class::className );
				return nullptr;
			}
			return instance;
		}
		
		static bool test( lua_State* state , int narg )
		{
			// We need the name of the object that is on the stack
			if( !lua_getmetatable( state , narg ) )
				return false;
			
			lua_pushliteral( state , "__name" );
			lua_rawget( state , -2 );
			
			if( !lua_isstring( state , -1 ) ){
				lua_pop( state , 2 );
				return false;
			}
			
			const char* name = lua_tostring( state , -1 );
			lua_pop( state , 2 ); // Pop the metatable and the classname from the stack
			
			// We want to check whether this class is a base class of the gadget on the stack
			auto func = LunarHelper::className2checkIfBaseFunction[name];
			
			return func && (*func)( Class::className );
		}
		
		static Class* lightcheck( lua_State* state , int narg )
		{
			if( test( state , narg ) )
			{
				// Fetch instance
				Class** instancePtr = static_cast<Class**>( lua_touserdata( state , narg ) );
				
				// Check if the instance is valid
				if( instancePtr )
					return *instancePtr;
			}
			return nullptr;
		}
		
		static void install( lua_State* state )
		{	
			// Register the constructor
			Lunar<Class>::registerClassName<Class>( state );
			
			// Create a metatable
			luaL_newmetatable( state , Class::className );
			
			// Store its index
			int metatable = lua_gettop( state );
			
			// Push Garbage-Collector handler
			lua_pushliteral( state , "__gc");
			lua_pushcfunction( state , &Lunar<Class>::garbageCollector );
			lua_settable( state , metatable );
			
			// Tell Lua how this class gets converted to a string
			lua_pushliteral( state , "__tostring");
			lua_pushcfunction( state , &Lunar<Class>::toString );
			lua_settable( state , metatable );
			
			// Register handler for property and function access
			lua_pushliteral( state , "__index");
			lua_pushcfunction( state , &Lunar<Class>::propertyGetter );
			lua_settable( state , metatable );
			
			// Register a handler for setting values of properties
			lua_pushliteral( state , "__newindex");
			lua_pushcfunction( state , &Lunar<Class>::propertySetter );
			lua_settable( state , metatable );
			
			// Pass the name of the class as an attribute in the metatable
			lua_pushliteral( state , "__name" );
			lua_pushstring( state , Class::className );
			lua_settable( state , metatable );
			
			lua_pushliteral( state , "dump" );
			lua_pushcfunction( state, &Lunar<Class>::dump );
			lua_settable( state , metatable );
			
			// Register Properties
			for( int i = 0 ; Class::properties[i].name ; i++ )
			{
				// Push the unique name of the property for identification
				lua_pushstring( state , Class::properties[i].name );
				
				// And a number indexing which property it is
				lua_pushnumber( state , i );
				lua_rawset( state , metatable );
			}
			
			// Register Functions
			for( int i = 0 ; Class::methods[i].name ; i++ )
			{
				// Push the unique name of the function for identification
				lua_pushstring( state , Class::methods[i].name );
				
				// Add a number indexing which func it is
				lua_pushnumber( state , LunarHelper::index2func(i) );
				lua_rawset( state , metatable );
			}
			
			LunarHelper::className2checkIfBaseFunction[Class::className] = static_cast<bool(*)(const char*)>( &Lunar<Class>::recursiveCheck );
		}
};

#define LUA_CLASS_FUNC( class , name ) { #name , &class::name }
#define LUA_CLASS_FUNC_END { 0 , 0 }
#define LUA_CLASS_ATTR_END { 0 , 0 , 0 }

#endif