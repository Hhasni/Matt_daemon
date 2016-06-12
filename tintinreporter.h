
#ifndef TINTIN_REPORTER_H
#define TINTIN_REPORTER_H

#include <fstream>
#include <asmtypes.h>

using namespace std;

#define TR_BAD 1
#define TR_READY 2

typedef void * trcode;

#define TRINFO ( ( void * ) 0 )
#define TRERROR ( ( void * ) 1 )
#define TRLOG ( ( void * ) 2 )

class Tintin_reporter
{
	private:
		ofstream stream;
		string name;
		string path;
		byte flags;

		void timestamp( void );
	public:
		// Constructors
		Tintin_reporter( void );
		Tintin_reporter( Tintin_reporter & );
		Tintin_reporter( const char * );

		// Destructor
		~Tintin_reporter( void );

		// File handle
		bool open( const char * );
		void close( void );

		// Output
		Tintin_reporter &operator=( Tintin_reporter & );
		Tintin_reporter &operator<<( trcode );
		Tintin_reporter &operator<<( const char * );
		Tintin_reporter &operator<<( int );
		Tintin_reporter &operator<<( char );
		void flush( void );

		// Setters
		void setname( const char * );
		void setname( string & );

		// Getters
		bool isbad( void );
		bool isready( void );
};

#endif
