#ifndef _WIN_T_INI_
#define _WIN_T_INI_

#include "_type/type.h"
#include "_type/type.assocVector.h"

typedef _assocVector<string,_assocVector<string,string>> _iniStructure;

class _ini
{
	private:
		
		_iniStructure			array;
		
	public:
		
		//! Ctor
		_ini( const string& input ){
			this->read( input );
		}
		
		//! Default Ctor
		_ini(){
		}
		
		//! Returns either -1 for success or the line number of error
		_s16 read( const string& input );
		
		//! Encode the std::map structure to string
		string write();
		
		//! Get the c++ std::map representation after a previous call to ::read
		const _iniStructure& getMap() const {
			return this->array;
		}
		
		//! Get a modifyable version of the std::map representation of the .ini file
		_iniStructure& getMap(){
			return this->array;
		}
		
		//! Read an index from the registry out of the supplied section
		const string& readIndex( const string& section , const string& name ) const ;
		_int readIndexInt( const string& section , const string& name ) const ;
		
		//! Write an index to the registry, a section and a key will be generated, if they're not existent
		void writeIndex( const string& section , const string& name , const string& value ){ this->array[section][name] = value; }
		
		//! Delete a section from the ini structure
		void deleteSection( const string& section ){ this->array.erase(section); }
		
		//! Delete an index out of the supplied region
		void deleteIndex( const string& section , const string& name );
		
		//! Read whole Section
		const _assocVector<string,string>& readSection( const string& section ) const ;
		
		//! Virtual Dtor
		virtual ~_ini(){};
};

#endif