#include "udp.h"
#include "host.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>

UDP::UDP(){
	zeroize();
	open();
}
UDP::UDP( const char* ip, const uint16_t port ){
	zeroize();
	m_host = Host( ip, port );
	open();
}
UDP::UDP( const uint16_t port, const char* ip ){
	zeroize();
	m_host = Host( ip, port );
	open();
}
UDP::UDP( Host host ){
	zeroize();
	m_host = host;
	open();
}
UDP::UDP( const UDP& udp ){
	zeroize();
	m_host = udp.m_host;
	reuseSocket();
	open();
}
UDP::~UDP(){
}

int UDP::recv( void* data, size_t sz ){
	socklen_t addr_len = sizeof( struct sockaddr_in );
	return recvfrom( m_sockfd, data, sz, 0,
			(struct sockaddr*)&m_incoming, &addr_len );
}
int UDP::recv( Host* host, void* data, size_t sz ){
	int ret = recv( data, sz );
	*host = getReplyHost();
	return ret;
}
int UDP::send( const char* ip, uint16_t port, const void* data, size_t sz ){
	return send( Host(ip,port), data, sz );
}
int UDP::send( Host host, const void* data, size_t sz ){
#define LAST_OCTET_OFFSET 12
#define OCTET_LENGTH 3
	if( strncmp(	host.ip + LAST_OCTET_OFFSET, 
			"255.255.255.255", 
			OCTET_LENGTH ) == 0 ){
		setBroadcast(true);
	}
	int len = sizeof( m_outgoing );
	m_outgoing = host.getSockaddr();
	return sendto( m_sockfd, data, sz, 0, (sockaddr*)&m_outgoing, len );
#undef LAST_OCTET_OFFSET
#undef OCTET_LENGTH
}

int UDP::setBlocking( bool block ){
	if( m_blocking == block ) return 1;
	m_blocking = block;
	int old = fcntl( m_sockfd, F_GETFL );
	if( block ) old &= ~O_NONBLOCK;
	else        old |=  O_NONBLOCK;
	return fcntl( m_sockfd, F_SETFL, old );
}
int UDP::setBroadcast( bool broadcast ){
	if( m_broadcast == broadcast ) return 1;
	m_broadcast = broadcast;
	return setOption( SO_BROADCAST, broadcast );

}
Host UDP::getReplyHost(){
	Host ret;
	ret.setSockaddr( m_incoming );
	return ret;
}

int UDP::open(){
	int ret = 0;
	m_outgoing = m_host.getSockaddr();
	if( (m_sockfd = socket( AF_INET, SOCK_DGRAM, 0 )) < 0 ) ret |= 0x01;
	if( bind( m_sockfd, (const struct sockaddr*)&m_outgoing,
				sizeof(m_outgoing) ) < 0 )      ret |= 0x02;
	return ret;
}
int UDP::close(){
	return reuseSocket();
}

int UDP::setOption( int optname, bool on ){
	int i = on; 
	return setsockopt( m_sockfd, SOL_SOCKET, optname, &i, sizeof(int) );
}
int UDP::reuseSocket( bool reuse ){
	return setOption( SO_REUSEADDR, reuse );
}

void UDP::zeroize(){
	m_sockfd = -1;
	m_blocking = true;
	memset( &m_outgoing, 0, sizeof( struct sockaddr_in ) );
	memset( &m_incoming, 0, sizeof( struct sockaddr_in ) );
	m_host.ip = NULL;
	m_host.port = 0;
}
