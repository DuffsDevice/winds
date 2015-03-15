#ifndef _WIN_T_INI_
#define _WIN_T_INI_

#include <_type/type.h>
#include <_type/type.assocVector.h>

typedef _assocVector<wstring,_assocVector<wstring,wstring>> _iniStructure;

class _ini
{
	private:
		
		_iniStructure			array;
		
	public:
		
		//! Ctor
		_ini( const wstring& input ){
			this->read( input );
		}
		
		//! Default Ctor
		_ini(){}
		
		//! Returns either -1 for success or the line number of error
		_s16 read( const wstring& input );
		
		//! Encode the std::map structure to string
		wstring write();
		
		//! Get the c++ std::map representation after a previous call to ::read
		const _iniStructure& getMap() const {
			return this->array;
		}
		
		//! Get a modifyable version of the std::map representation of the .ini file
		_iniStructure& getMap(){
			return this->array;
		}
		
		//! Override the std::map representation
		void setMap( _iniStructure structure ){
			this->array = move(structure);
		}
		
		//! Read an index from the registry out of the supplied section
		//! If 'exists' is not null, it will be set to true, if the key exists, false otherwise
		const wstring& readIndex( const wstring& section , const wstring& name , bool* exists = nullptr ) const ;
		_int readIndexInt( const wstring& section , const wstring& name , bool* exists = nullptr ) const ;
		
		//! Write an index to the registry, a section and a key will be generated, if they're not existent
		void writeIndex( const wstring& section , const wstring& name , const wstring& value ){ this->array[section][name] = value; }
		
		//! Delete a section from the ini structure
		void deleteSection( const wstring& section ){ this->array.erase(section); }
		
		//! Delete an index out of the supplied region
		void deleteIndex( const wstring& section , const wstring& name );
		
		//! Read whole Section
		//! If 'exists' is not null, it will be set to true, if the key exists, false otherwise
		const _assocVector<wstring,wstring>& readSection( const wstring& section , bool* exists = nullptr ) const ;
		
		//! Virtual Dtor
		virtual ~_ini(){};
};

#endif