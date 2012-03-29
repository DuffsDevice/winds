#ifndef _WIN_T_FILEATTR_
#define _WIN_T_FILEATTR_

union _fileAttributes{
	_u8 sum;	
	// Bitset
	struct{
		public:
			_u8	readonly 	:1;
			_u8 hidden 		:1;
			_u8 system		:1;
			_u8 volume		:1;
		private:
			_u8 directory	:1;
		public:
			_u8 archive		:1;
	} __attribute__(( packed )) attr;
	
	_fileAttributes( _u8 val ) : sum( val ){}
	_fileAttributes() : sum( 0 ){}
	operator _u8(){
		return this->sum;
	}
};

#endif