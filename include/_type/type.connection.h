#include "_type/type.h"

typedef enum{
	tcpNormal,
	tcpUnblocked
}_tcpType;

class _wifiSocketClient{
	
	private:
	
		_tcpType 	mode;
		int			sock;
		_u16		port;
		bool		succeeded;
		
		_u32 makeIP( _u8 d , _u8 c , _u8 b , _u8 a );
		
		_u32 string2IP( string str );
		
		
	public:
		
		_wifiSocketClient( string host , int port , _tcpType mode );
		
		void request( string url );
	
};