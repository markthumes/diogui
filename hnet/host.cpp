#include "host.h"

Host::Host(){
	ip = NULL;
	port = 0;
}
Host::Host( const char* _ip, uint16_t _port ){
	ip = _ip;
	port = _port;
}
Host::Host( uint16_t _port, const char* _ip ){
	ip = _ip;
	port = _port;
}
/*
Host::Host( const Host& host ){
	ip = host.ip;
	port = host.port;
}
*/

struct sockaddr_in Host::getSockaddr(){
	struct sockaddr_in ret;
	ret.sin_family = AF_INET;
	ret.sin_port   = htons(port);
	if( ip == NULL )
		ret.sin_addr.s_addr = INADDR_ANY;
	else
		ret.sin_addr.s_addr = inet_addr( ip );
	return ret;
}

void Host::setSockaddr( struct sockaddr_in inbound ){
	port = ntohs( inbound.sin_port );
	ip   = inet_ntoa( inbound.sin_addr );
}
