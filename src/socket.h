#include <string>
#include <exception>
#include <winsock.h>

class Socket {
public:
	// Communication direction
	enum Direction { RECEIVE, SEND, BIDIR };

	// Constructor. Port will be htons'ed, so no need to convert it.
	Socket( int port, Direction dir, const std::string& hostname = "" );

	// Destructor
	~Socket();

	// Sends a raw char array
	void send( const char* data, int len );

	// Receives into a raw buffer. Returns number of bytes received.
	int receive( char* buffer, int len );

	// Cleans up the sockets - call at app. exit or when sockets are no longer used
	static void clean();

private:
	// Windows socket
	SOCKET socket_;

	// IP address and port
	sockaddr_in address_; 
};
