#include <_type/type.h>

typedef enum{
	tcpNormal,
	tcpUnblocked
}_tcpType;


class _wifiSocketClient{
	
	private:
	
		_tcpType 	mode;
		int			sock; // Internal socket id
		_u16		port; // Port to use
		bool		succeeded; // Whether a connection was succesful
		
		//! Used by 'string2IP'
		_u32 makeIP( _u8 d , _u8 c , _u8 b , _u8 a );
		
		//! Make IP from string (e.g. '192.168.1.13')
		_u32 string2IP( string str );
		
		
	public:
		
		//! Ctor
		_wifiSocketClient( string host , int port , _tcpType mode = _tcpType::tcpNormal );
		
		//! Send a request to the server
		void request( string url );
		
		//! Dtor
		~_wifiSocketClient();
};