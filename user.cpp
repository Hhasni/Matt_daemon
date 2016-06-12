
#include <globalshit.h>

user::user( void )
{
	sock = -1;
}

user::user( user &usr )
{
	sock = usr.sock;
	while ( !buff.empty() )
		buff.pop();
}

user::~user( void )
{
}

user &user::operator=( user &usr )
{
	sock = usr.sock;
	while ( !buff.empty() )
		buff.pop();
	return *this;
}

bool user::in( void )
{
	int rc;

	if ( ( rc = recv( sock, rdbuff, BUFF_SIZE, 0 ) ) < 1 )
		return false;

	for ( char *min = rdbuff, *max = rdbuff + rc ; min < max ; min++ )
	{
		if ( *min == LF )
			lfc++;
		buff.push( *min );
	}
	return true;
}

bool user::hasstr( void )
{
	return lfc;
}

const char *user::getstr( void )
{
	if ( !lfc )
		return NULL;

	char c;

	strbuff.clear();
	while ( ( c = buff.front() ) != LF )
	{
		strbuff.push_back( c );
		buff.pop();
	}
	buff.pop();
	lfc--;
	return strbuff.c_str();
}

int user::getsock( void )
{
	return sock;
}

void user::setsock( int fd )
{
	sock = fd;
}
