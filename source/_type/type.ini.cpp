#include "_type/type.ini.h"
#include "_type/type.system.h"

_s16 _ini::read()
{
    /* Uses a fair bit of stack (use heap instead if you need to) */
    string 	line, name , value , section = "_global_";
	_u16 	lineNo = 0;
	_u16 	errorNo = -1; // Line with error
	
	_u32 end = 0, start;

    /* Scan through file line by line */
    while ( true )
	{
		start = end;
		
		if( start == string::npos )
			break;
		
		// Find Delimiter (end of line)
		end = this->input.find_first_of( "\n\r" , start );
		
		if( end != string::npos )
		{
			// Get text from Start (start) to End (end)
			line = this->input.substr( start , end - start );
			
			// Move beyond delimiter
			end++;
		}
		else
			line = this->input.substr( start );
		
		
		/**
		 * Choose right Action
		**/
        if( !line.size() || line[0] == ';' || line[0] == '#' )
		{
            /* Per Python ConfigParser, allow '#' and ';' comments at start of line */
        }
		else if( line[0] == '\r' || line[0] == '\n' )
		{
			end++;
		}
        else if( line[0] == '[' )
		{
            /* A "[section]" line */
			size_t end = line.find_first_of("]");
			
            if ( end != string::npos )
				section = line.substr( 1 , end - 1 );
			else
			{
				errorNo = lineNo;
				continue;
			}
			
			this->array[section];
        }
        else
		{
			/* Must be a name:= value pair! */
			size_t delim = line.find_first_of(":=");
			
			if( delim == string::npos )
			{
				errorNo = lineNo;
				continue;
			}
			
			name = line.substr( 0 , delim );
			value = line.substr( delim + 1 , line.find_first_of(";#") );
			
			trim( name );
			trim( value );
			
			this->array[section][name] = value;
        }
    }
	
	return errorNo;
}

void _ini::write()
{
	this->output.clear();
	for( pair<const string,map<string,string>>& section : this->array )
	{
		if( section.first != "_global_" )
		{
			this->output += "[";
			this->output += section.first;
			this->output += "]\r\n";
		}
		for( pair<const string,string>& nvp : section.second )
		{
			if( !nvp.first.length() )
				continue;
			this->output += nvp.first;
			this->output += " = ";
			this->output += nvp.second;
			this->output += "\r\n";
		}
		this->output += "\r\n";
	}
}

_iniStructure& _ini::getMap(){
	return this->array;
}

string& _ini::getString(){
	return this->output;
}
