#ifndef __htoolkit_host__
#define __htoolkit_host__

#include <inttypes.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <arpa/inet.h>

class Host{
public:
	Host();
	Host( const char* _ip, uint16_t _port = 0 );
	Host( uint16_t _port, const char* _ip = NULL );
//	Host( const Host& host );
	~Host(){};
	const char* ip;
	uint16_t port;

	struct sockaddr_in getSockaddr();
	void setSockaddr( struct sockaddr_in inbound );
};

#endif
