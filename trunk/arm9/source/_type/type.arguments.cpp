#include <_type/type.arguments.h>

_args::_args( _literal cmd )
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

_pair<string,string> _args::splitCommand( const string& cmd )
{
	if( cmd.empty() )
		return _pair<string,string>();
	
	size_t fileNameEnd = string::npos;
	
	int escaped = cmd[0] == '"' ? 1 : 0;
	
	if( escaped ) // If it starts with an apostroph
		fileNameEnd = cmd.find( '"' , 1 );
	else
		fileNameEnd = cmd.find( ' ' );
	
	_literal parameterStr =
		fileNameEnd != string::npos
			? cmd.c_str() + fileNameEnd + escaped
			: ""
	;
	
	return make_pair( cmd.substr( escaped , fileNameEnd - escaped ) , parameterStr );
}