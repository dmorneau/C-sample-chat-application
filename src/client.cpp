#include <iostream>
#include "socket.h"
#include <cassert>
using namespace std;

const int BUFFSIZE = 10;

enum Protocol { SimplexStopWait, SimplexUnrestricted };
const Protocol PROTOCOL = SimplexStopWait;

// Ref http://www.comp.sd.keio.jp
// Demo client
int main( int argc, char *argv[] ){
	if( argc < 3 ) {
		cout << "Usage: " << argv[0] << " port hostname" << endl;
		return -1;
	}
	cout << "Use :q to quit." << endl;

	try {
		Socket out( atoi(argv[2]), Socket::SEND, argv[1] );
		Socket in( atoi(argv[2]) + 1, Socket::RECEIVE );

		string text;
		char buff[BUFFSIZE];
		do {
			// Send a line
			getline( cin, text );
			out.send( text.c_str(), text.length() + 1 );

			// Wait for confirmation
			if( PROTOCOL == SimplexStopWait ) {
				in.receive( buff, BUFFSIZE );
				cout << "SERVER: " << buff << endl;
			}
		} while( text != ":q" );
	} catch( std::runtime_error& e ) {
		cout << e.what() << endl;
	}

	Socket::clean();
	return 0;
}