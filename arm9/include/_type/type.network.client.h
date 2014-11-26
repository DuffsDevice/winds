#include <_type/type.h>

enum class _tcpType : _u8
{
	tcpNormal ,
	tcpUnblocked
};


class _networkClient
{
	private:
	
		_tcpType 	mode;
		int			socketId;
		string		host;
		bool		connected; // Whether a connection is established
		
		//! Used by 'string2IP'
		_u32 makeIP( _u8 d , _u8 c , _u8 b , _u8 a );
		
		//! Make IP from string (e.g. '192.168.1.13')
		_u32 parseIP( const string& str );
		
	public:
		
		//! Ctor
		_networkClient( _tcpType mode = _tcpType::tcpNormal ) : mode( mode ) , socketId( 0 ) , connected( false ) {}
		
		//! Dtor
		~_networkClient();
		
		//! Connects client with host
		bool connect( string host , int port );
		
		//! Dissconnect client from host
		void disconnect();
		
		//! Send a request to the server (requires the client to be already connected!)
		bool request( _literal path );
		
		//! Tries to receive something
		string receive( _u32 fragmentSize = 128 );
};