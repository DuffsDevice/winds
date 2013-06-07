#include "_type/type.connection.h"
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>

#define IS_DIGIT_OR_DOT(c) (isdigit(c)||((c)=='.'))
#define IS_INETADDR(s) (IS_DIGIT_OR_DOT(s[0])&&IS_DIGIT_OR_DOT(s[1])&&IS_DIGIT_OR_DOT(s[2])&&IS_DIGIT_OR_DOT(s[3])&&IS_DIGIT_OR_DOT(s[4])&&IS_DIGIT_OR_DOT(s[5])&&IS_DIGIT_OR_DOT(s[6]))

_u32 _wifiSocketClient::makeIP( _u8 d , _u8 c , _u8 b , _u8 a )
{
	return ( a << 24 ) | ( b << 16 ) | ( c << 8 ) | d;
}


_u32 _wifiSocketClient::string2IP( string str ) {
	
	int i, j;
	_u8 data[4];
	int debut = 0;

	for (j = 0; j < 4; j++) {
		char buffer[256];
		
		for (i = debut; str[i] != '.' && str[i] != '\0'; i++) {
			buffer[i-debut] = str[i];
		}
		
		buffer[i-debut] = '\0';
		debut = i + 1;
		data[j] = atoi(buffer);
	}

	return this->makeIP( data[0] , data[1] , data[2] , data[3] );
}

_wifiSocketClient::_wifiSocketClient( string host , int port , _tcpType mode ) :
	succeeded( false )
{
	unsigned long 		ip;
	struct sockaddr_in 	servaddr;
	
	//! Create Socket
	this->sock = socket(AF_INET, SOCK_STREAM, 0);

	if ( IS_INETADDR( host ) )
		ip = this->string2IP(host); // An IP entered!
	else
		ip = *(unsigned long *)gethostbyname( host.c_str() )->h_addr_list[0]; // A Web address entered!

	servaddr.sin_family = AF_INET; // Standard
	servaddr.sin_port = htons( port ); // Write Port No.
	servaddr.sin_addr.s_addr = ip;

	int connectResult = connect( this->sock , (sockaddr*)&servaddr , sizeof(servaddr) );
	
	if( connectResult == 0 )
	{
		this->succeeded = true;
		if( mode == _tcpType::tcpUnblocked )
		{
			int i = 1;
			ioctl(this->sock, FIONBIO, &i);
		}
	}
	printf("->%d\n",this->succeeded);
}

_wifiSocketClient::~_wifiSocketClient()
{

}

void _wifiSocketClient::request( string url ){
	printf("Sent Request\n");
	send( this->sock, url.c_str() , url.length() , 2 );
}
