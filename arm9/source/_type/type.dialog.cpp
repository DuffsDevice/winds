#include <_type/type.dialog.h>

void _dialog::execute()
{
	if( this->runs )
		return;
	this->executeInternal();
	this->runs = true;
}

void _dialog::terminate()
{
	if( !this->runs )
		return;
	this->callCallback( _dialogResult::cancel );
	this->cleanup();
}

void _dialog::deleteCallback()
{
	if( this->callback )
		delete this->callback;
	this->callback = 0;
}

_toStr<_dialogResult> dialogResult2string = {
	{ _dialogResult::undefined , "undefined" },
	{ _dialogResult::cancel , "cancel" },
	{ _dialogResult::yes , "yes" },
	{ _dialogResult::no , "no" },
	{ _dialogResult::apply , "apply" }
};