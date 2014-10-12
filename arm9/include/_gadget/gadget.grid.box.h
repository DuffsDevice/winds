#ifndef _WIN_T_GRID_BOX_
#define _WIN_T_GRID_BOX_

#include <_gadget/gadget.grid.h>

enum class _gridBoxType : _u8
{
	str,
	str_fmt,
	gadget
};
	
class _gridBox : public _gadget
{
	private:
		
		_gridBoxType type;
		
		virtual _callbackReturn drawHandler( _event ) = 0;
		
	public:
		
		_gridBox( _gridBoxType type ) :
			type( type )
		{}
		
		_gridBoxType getType(){ return this->type; }
};

class _gridBoxStr : _gridBox
{
	private:
		
		string content;
		
	public:
	
		string getContent() const { return this->content; }
		
		void setContent( string str ){ this->content = str; }
};

class _gridBoxStrFmt : _gridBox
{
	private:
		
		_literal	format;
		_int		content;
		
	public:
	
		string getContent() const { return this->content; }
		
		void setContent( string str ){ this->content = str; }
};