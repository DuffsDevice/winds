#ifndef _WIN_T_TOKENIZER_
#define _WIN_T_TOKENIZER_

#include "_type/type.h"

class _tokenizer
{
	private:
		
		const string&		source;
		string&				destination;
		const char* const	delimiters;
		_u32				start;
		_u32				end;
		bool				noEmptyValues;
	
	public:
		
		_tokenizer( const string& src , string& dest , const char* const delim , bool noEmptyValues = false ) :
			source( src ),
			destination( dest ),
			delimiters( delim ),
			end( 0 ),
			noEmptyValues( noEmptyValues )
		{}
		
		bool next()
		{
			do
			{
				start = end;
				if( start == string::npos )
					return false;
				
				// Find Delimiter
				end = source.find_first_of( delimiters , start );
				
				if( end != string::npos ) // Check if new delimiter found
				{
					destination = source.substr( start , end - start ); // Get text from Start (start) to End (end)
					end++; // Move beyond delimiter
				}
				else
					destination = source.substr( start );
				
			}while( noEmptyValues && destination.empty() );
			
			return true;
		}
};

#endif