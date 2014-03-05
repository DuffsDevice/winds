#include "_type/type.menu.rule.h"

bool _menuHandlerRule::operator()( _int listIndex , _int index ) const 
{
	switch( this->type )
	{
		case _menuHandlerRuleType::wholeMenu:
			return true;
		case _menuHandlerRuleType::listRange:
			if( this->listIndex == listIndex && this->startIndex <= index && this->endIndex >= index )
				return true;
			break;
		case _menuHandlerRuleType::listIndex:
			if( this->listIndex == listIndex && this->startIndex == index )
				return true;
			break;
		case _menuHandlerRuleType::wholeList:
			if( this->listIndex == listIndex )
				return true;
			break;
	}
	return false;
}