#include <_type/type.service.h>

// Convert a _serviceState to a string
_toStr<_serviceState> serviceState2string = {
	{ _serviceState::init		, "init" },
	{ _serviceState::main		, "main" },
	{ _serviceState::success	, "success" },
	{ _serviceState::terminate	, "terminate" },
	{ _serviceState::error		, "error" },
	{ _serviceState::undefined	, "undefined" }
};

// And back
_fromStr<_serviceState> string2serviceState = {
	{ "init"		, _serviceState::init },
	{ "main"		, _serviceState::main },
	{ "success"		, _serviceState::success },
	{ "error"		, _serviceState::error },
	{ "terminate"	, _serviceState::terminate },
	{ "undefined"	, _serviceState::undefined }
};