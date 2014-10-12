#ifndef _WIN_T_FONT_HANDLE_
#define _WIN_T_FONT_HANDLE_

#include <_type/type.h>
#include <_type/type.font.h>

class _fontHandle
{
	private:
		
		mutable _fontPtr ptr;
		
		//! Checks if the pointer is still valid
		_fontPtr validate() const ;
	
	public:
		
		//! Ctor
		_fontHandle( _fontPtr font = nullptr ) :
			ptr( font )
		{}
		
		//! Casts to _font*
		operator _fontPtr() const {
			return validate();
		}
		
		//! Assignment operator
		_fontHandle& operator=( _fontPtr font ){
			this->ptr = font;
			return *this;
		}
		
		//! Equality check
		bool operator==( _fontPtr font ){
			return this->ptr == font;
		}
		
		//! Get pointer to font
		_fontPtr data() const {
			return validate();
		}
		
		//! Get pointer to font without validating it and without any fallback
		_fontPtr rawData(){
			return this->ptr;
		}
		
		//! Check if the font to which the handle points still exists
		bool isValid() const {
			validate();
			return this->ptr != nullptr;
		}
		
		//! Makes the handle point to the first font in the font-manager
		_fontHandle front();
		
		//! Makes the handle point to the last font in the font-manager
		_fontHandle back();
		
		//! Modifies the handle to point to the next font in the font-manager
		_fontHandle next();
		
		//! Modifies the handle to point to the previous font in the font-manager
		_fontHandle prev();
		
		//! Dereferencing operator
		const _font& operator*() const { return *validate(); }
		
		//! Dereferencing operator
		_fontPtr operator->() const { return validate(); }
};

#endif