#ifndef __htoolkit_udp__
#define __htoolkit_udp__

#include "host.h"
#include <stdio.h>
#include <inttypes.h>
#include <netinet/ip.h>

class UDP{
public: //Constructors / Destructor
	UDP();
	UDP( const char* ip, const uint16_t port = 0    );
	UDP( const uint16_t port, const char* ip = NULL );
	UDP( Host host );
	UDP( const UDP& udp );
	~UDP();
public: 
	int recv( void* data, size_t sz );
	int recv( Host* host, void* data, size_t sz );
	int send( const char* ip, uint16_t port, const void* data, size_t sz );
	int send( Host host, const void* data, size_t sz );

	int setBlocking( bool block = true );
	int setBroadcast( bool broadcast = true );
	Host getReplyHost();
protected:
	int open();
	int close();
	int setOption(int optname, bool on = true);
	int reuseSocket( bool reuse = true );

	void zeroize();
private:
	int  m_sockfd;
	bool m_broadcast;
	bool m_blocking;

	struct sockaddr_in m_outgoing;
	struct sockaddr_in m_incoming;

	Host m_host;
	
};
#endif

