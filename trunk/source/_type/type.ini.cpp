#include "_type/type.ini.h"
#include "_type/type.system.h"

void _ini::trim( string& str ){
	size_t endpos = str.find_last_not_of(" \n\r\t");
	if( string::npos != endpos )
	{
		str = str.substr( 0, endpos+1 );
	}
	size_t startpos = str.find_first_not_of(" \n\r\t");
	if( string::npos != startpos )
	{
		str = str.substr( startpos );
	}
}

/* See documentation in header file. */
bool _ini::parse()
{
    /* Uses a fair bit of stack (use heap instead if you need to) */
    string line, name , value , section;

    _u16 lineNo = 0;
    bool error = 0;

    /* Scan through file line by line */
    while ( getline( this->str , line ) )
	{
		lineNo++;
		
        if( line[0] == ';' || line[0] == '#') {
            /* Per Python ConfigParser, allow '#' comments at start of line */
        }
        else if ( line[0] == '[' ) {
            /* A "[section]" line */
			_s16 end = line.find_first_of("]");
            if ( end != string::npos )
			{
				section = line.substr( 1 , end - 1 );
				//printf("Section changed: %s\n",section.c_str() );
            }
            else{
				error = true;
				/* No ']' found on section line */
				stringstream out;
				out << "Error Parsing ini-string in Line " << lineNo << ": No ending ']' found!";
				_system_->debug( out.str() );
            }
        }
        else{
			/* Must be a name:= value pair! */
			_s16 delim = line.find_first_of(":=");
			if( delim == string::npos ){
				error = true;
				stringstream out;
				out << "Error Parsing ini-string in Line " << lineNo << ": Neither ':' nor '=' found!";
				_system_->debug( out.str() );
				break;
			}
			name = line.substr( 0 , delim );
			value = line.substr( delim + 1 , line.find_first_of(";#") );
			trim( name );
			trim( value );
			this->array[section + ":" + name] = value;
			//printf("%s=%s\n" , (section + ":" + name).c_str() , value.c_str() );
        }
    }
	
	return !error;
}

map<string,string>& _ini::getMap(){
	return this->array;
}
