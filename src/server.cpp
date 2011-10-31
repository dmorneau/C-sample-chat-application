#include <iostream>
#include <cassert>
#include "socket.h"
using namespace std;

const unsigned int BUFFERSIZE = 200;
const string ack = "ACK";

// Ref http://www.comp.sd.keio.jp
// Demo server
int main(int argc, char *argv[]){
	if( argc == 2 ) {
		cout << "Only one argument received - assuming client is localhost." << endl;
	}
	if( argc < 2 ) {
		cout << "Usage: " << argv[0] << " server-port client-hostname" << endl;
		cout << "Client hostname is localhost by default." << endl;
		return 1;
	}

	try {
		Socket in( atoi(argv[1]), Socket::RECEIVE );
		Socket out( atoi(argv[1]) + 1, Socket::SEND, 
			argc > 2 ? argv[2] : "localhost" );

		int packet = 0;
		while( true ) {
			// Get a buffer from the socket
			char buffer[BUFFERSIZE];
			int rcvd = in.receive( buffer, BUFFERSIZE );
			buffer[rcvd + 1] = '\0';

			// Send ack to client
			out.send( ack.c_str(), ack.length() + 1 );

			// Print the result
			if( rcvd == -1 )
				cout << "Packet #" << packet++ << " too long for buffer - ignored.";
			else
				cout << "Packet #" << packet++ << ": " << buffer << endl;
			if( buffer == string(":q") )
				break;

		}
	} catch (runtime_error& e) {
		cout << e.what();
	}

	Socket::clean();
	return 0;
}
