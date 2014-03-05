#include "_type/type.program.args.h"

_programArgs::_programArgs( _literal cmd )
{
	if( !cmd )
		return;

	// Variables for string iteration
	_literal start = cmd;
	_literal cur = start;
	_literal startPhrase = start;
	
	// Flag
	bool stringOpened = false;
	
	do{
		switch( *cur )
		{
			case 0:
			case '-':
			case ' ':
				if( !stringOpened || !*cur ) // If not inside quotation marks
				{
					if( cur - startPhrase - 1 > 0 ) // Check length of the token
						this->push_back( string( startPhrase , cur - startPhrase ) );
					startPhrase = cur + 1; // Set begin of new phrase behind the current character
				}
				break;
			case '"':
				if( cur - startPhrase - 1 > 0 ) // Check length of the token
				{
					if( stringOpened )
						this->push_back( string( startPhrase , cur - startPhrase ) );
					else
						startPhrase = cur + 1; // Set begin of new phrase behind the "
				}
				stringOpened = !stringOpened; // Switch Mode
				startPhrase = cur + 1; // Set begin of new phrase behind the "
				break;
			default:
				break;
		}
	}while( *cur++ );
}