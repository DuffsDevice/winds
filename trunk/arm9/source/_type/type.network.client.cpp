#include <_type/type.network.client.h>

// Linux Headers
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

// C-Headers
#include <cstdlib>
#include <cstdio>

#define IS_DIGIT_OR_DOT(c) (isdigit(c)||((c)=='.'))
#define IS_INETADDR(s) (IS_DIGIT_OR_DOT(s[0])&&IS_DIGIT_OR_DOT(s[1])&&IS_DIGIT_OR_DOT(s[2])&&IS_DIGIT_OR_DOT(s[3])&&IS_DIGIT_OR_DOT(s[4])&&IS_DIGIT_OR_DOT(s[5])&&IS_DIGIT_OR_DOT(s[6]))

_u32 _networkClient::makeIP( _u8 d , _u8 c , _u8 b , _u8 a )
{
	return ( a << 24 ) | ( b << 16 ) | ( c << 8 ) | d;
}


_u32 _networkClient::parseIP( const string& str )
{
	_u32 i = 0;
	_u32 j = 0;
	_u32 debut = 0;
	_u8 data[4];

	for( j = 0 ; j < 4 && str[i] != '\0' && i - debut < 64 ; j++ )
	{
		_char buffer[64];
		
		for( i = debut ; str[i] != '.' && str[i] != '\0' && i - debut < 64 ; i++ )
			buffer[ i - debut ] = str[i];
		
		buffer[ i - debut ] = '\0';
		debut = i + 1;
		data[j] = atoi( buffer );
	}
	
	return this->makeIP( data[0] , data[1] , data[2] , data[3] );
}

bool _networkClient::connect( string host , int port )
{
	// Disconnect with old host if there is one
	this->disconnect();
	
	_u32			ip;
	sockaddr_in 	servaddr;
	
	//! Create Socket
	this->socketId = socket( AF_INET , SOCK_STREAM , 0 );

	if( IS_INETADDR( host ) ){
		ip = this->parseIP( host ); // An IP entered
		this->host.clear();
	}
	else{
		ip = *(_u32*)gethostbyname( host.c_str() )->h_addr_list[0]; // A Web address entered
		this->host = move(host);
	}

	servaddr.sin_family = AF_INET; // Standard
	servaddr.sin_port = htons( port ); // Write Port Number
	servaddr.sin_addr.s_addr = ip;

	int connectResult = ::connect( this->socketId , (sockaddr*)&servaddr , sizeof(servaddr) );
	
	if( connectResult == 0 ) // Connection established
	{
		this->connected = true;
		if( mode == _tcpType::tcpUnblocked )
		{
			int i = 1;
			ioctl( this->socketId , FIONBIO , &i );
		}
	}
	
	return this->connected;
}

_networkClient::~_networkClient()
{
	this->disconnect();
	
	closesocket( this->socketId );
}

bool _networkClient::request( _literal path )
{
	if( !this->connected )
		return false;
	
	string request = "GET ";
	request += path;
	request += " HTTP/1.1";
	if( !this->host.empty() ){
		request += "\r\nHost: ";
		request += this->host;
	}
	request += "\r\nUser-Agent: Nintendo DS\r\n\r\n";
	
	// Send the request
	return send( this->socketId , request.c_str() , request.length() , 2 ) > 0;
}

void _networkClient::disconnect()
{
	if( this->connected ){
		shutdown( this->socketId , 0 );
		this->connected = false;
		this->host.clear();
	}
}

string _networkClient::receive( _u32 fragmentSize , bool wholeResult )
{
	string	buffer;
	int		bytesReceived;
    _char*	tempBuffer = new _char[fragmentSize];

    while( ( bytesReceived = recv( this->socketId, tempBuffer , fragmentSize - 1 , 0 ) ) != 0 ) // if recv returns 0, the socket has been closed.
	{
        if( bytesReceived > 0 )
		{
            tempBuffer[bytesReceived] = 0; // null-terminate
			buffer += tempBuffer;
		}
		
		if( !wholeResult )
			return buffer;
	}
	
	return buffer;
}