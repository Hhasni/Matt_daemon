
#include "tintinreporter.h"
#include <iostream>

Tintin_reporter::Tintin_reporter( void )
{
	flags = 0;
}

Tintin_reporter::Tintin_reporter( Tintin_reporter &tr )
{
	path = tr.path;
	stream.open( path.c_str(), ofstream::app );
        if ( stream.fail() )
                flags = TR_BAD;
        else
	{
		tr.stream.flush();
		stream.seekp( tr.stream.tellp() );
                flags = TR_READY;
	}
}

Tintin_reporter::Tintin_reporter( const char *s )
{
	path.append( s );
	stream.open( s, ofstream::app );
	if ( stream.fail() )
		flags = TR_BAD;
	else
		flags = TR_READY;
}

void Tintin_reporter::timestamp( void )
{
        time_t rawtime;
        struct tm *timeinfo;
        char buffer[300];

        time( &rawtime );
        timeinfo = localtime( &rawtime );
        strftime( buffer, 300, "[ %d/%m/%Y - %H:%M:%S ]", timeinfo );
	
	stream << buffer;
}

Tintin_reporter &Tintin_reporter::operator=( Tintin_reporter &tr )
{
	path.clear();
	stream.close();
        path = tr.path;
        stream.open( path.c_str(), ofstream::app );
        if ( stream.fail() )
                flags = TR_BAD;
        else
        {
                tr.stream.flush();
                stream.seekp( tr.stream.tellp() );
                flags = TR_READY;
        }
}

Tintin_reporter &Tintin_reporter::operator<<( trcode trc )
{
	if ( !( flags & TR_READY ) )
		return *this;

	timestamp();
	stream << " [ ";
	switch ( ( qword ) trc )
	{
		case 0:
			stream << "INFO";
			break;
		case 1:
			stream << "ERROR";
			break;
		case 2:
			stream << "LOG";
			break;
		default:
			stream << "??";
	}
	stream << " ]";
	if ( name.length() )
		stream << " - " << name;
	stream << ": ";
	return *this;
}

Tintin_reporter &Tintin_reporter::operator<<( const char *s )
{
	if ( !( flags & TR_READY ) )
		return *this;

	stream << s;
	return *this;
}

Tintin_reporter &Tintin_reporter::operator<<( int n )
{
	if ( !( flags & TR_READY ) )
		return *this;

	stream << n;
	return *this;
}

Tintin_reporter &Tintin_reporter::operator<<( char c )
{
	if ( !( flags & TR_READY ) )
		return *this;

	stream << c;
	return *this;
}

void Tintin_reporter::flush( void )
{
	stream.flush();
}

void Tintin_reporter::setname( const char *s )
{
	name.append( s );
}

void Tintin_reporter::setname( string &s )
{
	name = s;
}

bool Tintin_reporter::isbad( void )
{
	if ( flags & TR_BAD )
		return true;
	return false;
}

bool Tintin_reporter::isready( void )
{
	if ( flags & TR_READY )
		return true;
	return false;
}

bool Tintin_reporter::open( const char *s )
{
        path.clear();
        stream.close();
        path.append( s );
        stream.open( s, ofstream::app );
        if ( stream.fail() )
                flags = TR_BAD;
        else
                flags = TR_READY;
}

void Tintin_reporter::close( void )
{
	stream.close();
	flags = 0;
}

Tintin_reporter::~Tintin_reporter( void )
{
	stream.close();
}
