#include "_type/type.ini.h"
#include "_type/type.system.h"

bool _ini::read()
{
    /* Uses a fair bit of stack (use heap instead if you need to) */
    string line, name , value , section = "_global_";

    /* Scan through file line by line */
    while ( getline( this->input , line ) )
	{
        if( line[0] == ';' || line[0] == '#') {
            /* Per Python ConfigParser, allow '#' comments at start of line */
        }
        else if ( line[0] == '[' )
		{
            /* A "[section]" line */
			_s16 end = line.find_first_of("]");
			
            if ( end != string::npos )
				section = line.substr( 1 , end - 1 );
			else
				return false;
			
			this->array[section][""] = "";
        }
        else
		{
			/* Must be a name:= value pair! */
			_s16 delim = line.find_first_of(":=");
			if( delim == string::npos )
				return false;
			name = line.substr( 0 , delim );
			value = line.substr( delim + 1 , line.find_first_of(";#") );
			
			trim( name );
			trim( value );
			
			this->array[section][name] = value;
        }
    }
	
	return true;
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
