#include "socket.h"
#include <sstream>

// Constructor
Socket::Socket( int port, Direction dir, const std::string& hostname ) {
	// Initialize windows sockets, version 2.0
	WORD wVersionRequested = MAKEWORD(2, 0);
	WSADATA WSAData;
	WSAStartup( wVersionRequested, &WSAData );

	// Fill in the IP address and port
	if( hostname == "" ) {
		address_.sin_addr.s_addr = INADDR_ANY;
	} else {
		// Do not delete the hostent - winsock will do it
		hostent* dst( gethostbyname( hostname.c_str() ) );
		if( dst == NULL )
			throw std::runtime_error("Could not resolve hostname.");
		address_.sin_addr.s_addr = (*(u_long *)( dst->h_addr_list[0] ));
	}
	address_.sin_family = AF_INET;
	address_.sin_port = htons( port );
	memset( address_.sin_zero, 0, sizeof(address_.sin_zero) );

	// Create the socket
	socket_ = socket(PF_INET, SOCK_DGRAM, 0);
	if( socket_ == -1 )
		throw std::runtime_error("Could not create a socket.");

	// If necessary, bind it
	if( dir == RECEIVE || dir == BIDIR ) {
		int ret = bind( socket_, (sockaddr*)&address_, sizeof(address_) );
		if( ret == -1 )
			throw std::runtime_error("Could not bind socket.");
	}
}

// Destructor
Socket::~Socket() {
	closesocket( socket_ );
}

// Sends a raw char array
void Socket::send( const char* data, int len ) {
	int sent = sendto( socket_, data, len, 0, (struct sockaddr*)&address_, sizeof(address_) );
	if( sent != len ) {
		std::ostringstream err;
		err << "Error sending to socket: code " << WSAGetLastError() << ".\n";
		throw std::runtime_error(err.str());
	}
}

// Receives into a raw buffer
int Socket::receive( char* buffer, int len ) {
	int sizeadd = sizeof( address_ );
	return recvfrom( socket_, buffer, len, 0, (sockaddr*)&address_, &sizeadd);
}

// Cleans up
void Socket::clean() {
	WSACleanup();
}