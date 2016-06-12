
#include <globalshit.h>

int ref::lock;
int ref::sock;
Tintin_reporter ref::log( "/var/log/matt_daemon" );

const char *sigs[32] =
{
	NULL,
	"SIGHUP",
	"SIGINT",
	"SIGQUIT",
	"SIGILL",
	"SIGTRAP",
	"SIGABRT",
	"SIGBUS",
	"SIGFPE",
	"SIGKILL",
	"SIGUSR1",
	"SIGSEGV",
	"SIGUSR2",
	"SIGPIPE",
	"SIGALRM",
	"SIGTERM",
	"SIGSTKFLT",
	"SIGCHLD",
	"SIGCONT",
	"SIGSTOP",
	"SIGTSTP",
	"SIGTTIN",
	"SIGTTOU",
	"SIGURG",
	"SIGXCPU",
	"SIGXFSZ",
	"SIGVTALRM",
	"SIGPROF",
	"SIGWINCH",
	"SIGPWR",
	"SIGSYS",
};

void quit( int ec )
{
	ref::log << TRINFO << "Quitting" << LF;
        flock( ref::lock, LOCK_UN );
        close( ref::lock );
        unlink( "/var/lock/Matt_daemon.lock" );
        close( ref::sock );
        ref::log.close();
	exit( ec );
}

void sighandler( int whatsig )
{
	ref::log << TRINFO << "Received a " << sigs[whatsig] << LF;
	quit( EXIT_FAILURE );
}

int startsrv( int port )
{
	int sock;
	struct protoent *proto;
	struct sockaddr_in sin;

	if ( !( proto = getprotobyname( "tcp" ) ) )
		return ( -1 );
	if ( ( sock = socket( PF_INET, SOCK_STREAM, (*proto).p_proto ) ) == -1 )
		return ( -1 );
	sin.sin_family = AF_INET;
	sin.sin_port = htons( port );
	sin.sin_addr.s_addr = htonl( INADDR_ANY );
	if ( bind( sock, ( const struct sockaddr * ) &sin, sizeof( sin ) ) )
		return ( -1 );
	listen( sock, 16 );
	return ( sock );
}

int number2( void *av )
{
	fd_set set, refset;
	int sswap, nfds = ref::sock + 1, nbusers = 0, sock;
	struct user users[3];
	const char *s;

	ref::log << TRINFO << "Started, Daemon PID : " << getpid() << LF;

	FD_ZERO( &refset );
	FD_SET( ref::sock, &refset );

masteru:ref::log.flush();
	set = refset;

	select( nfds, &set, NULL, NULL, NULL );

	// Catching connection
	if ( FD_ISSET( ref::sock, &set ) )
	{
		struct sockaddr_in sin;
		unsigned int sinlen;

		if ( ( sock = accept( ref::sock, ( struct sockaddr * )&sin, &sinlen ) ) == -1 )
		{
			ref::log << TRERROR << "Failure on master socket" << LF;
			quit( EXIT_FAILURE );
		}
		if ( nbusers >= 3 )
		{
			ref::log << TRINFO << "New connection denied, already reached maximum simultaneous connections" << LF;
			close( sock );
		}
		else
		{
			ref::log << TRINFO << "Client n°";
			if ( users[0].getsock() == -1 )
			{
				users[0].setsock( sock );
				ref::log << '0';
			}
			else if ( users[1].getsock() == -1 )
			{
				users[1].setsock( sock );
				ref::log << '1';
			}
			else
			{
				users[2].setsock( sock );
				ref::log << '2';
			}
			ref::log << " connected" << LF;
			FD_SET( sock, &refset );
			if ( sock > ( nfds - 1 ) )
				nfds = sock + 1;
			nbusers++;
		}
	}
	// end catching connection

	for ( int i = 0 ; i < 3 ; i++ )
	{
		if ( FD_ISSET( users[i].getsock(), &set ) )
		{
			if ( !users[i].in() )
			{
				ref::log << TRINFO << "Client n°" << i << " disconnected" << LF;
				FD_CLR( sock = users[i].getsock(), &refset );
				users[i].setsock( -1 );
				close( sock );
				if ( sock == ( nfds - 1 ) )
				{
					nfds = ref::sock;
					for ( int j = 0 ; j < 3 ; j++ )
					{
						if ( j == i )
							continue;
						if ( users[j].getsock() > nfds )
							nfds = users[j].getsock();
					}
					nfds++;
				}
				nbusers--;
			}
			else while ( users[i].hasstr() )
			{
				if ( !strcmp( s = users[i].getstr(), "quit" ) )
				{
					ref::log << TRINFO << "Requested quit by " << i << LF;
					goto kthxbye;
				}
				ref::log << TRLOG << i << " > " << s << LF;
			}
		}
	}
	goto masteru;

kthxbye:flock( ref::lock, LOCK_UN );
	close( ref::lock );
	unlink( "/var/lock/Matt_daemon.lock" );
	close( ref::sock );
	ref::log << TRINFO << "Quitting" << LF;
	ref::log.close();
	return EXIT_SUCCESS;
}

int main( void )
{
	pid_t pid;
	void *cstack;

	if ( getuid() )
	{
		cout << RED << "I only start in root mode, come back when you're on" << DEFAULT << LF;
		return EXIT_FAILURE;
	}

	signal( SIGHUP, &sighandler );
	signal( SIGINT, &sighandler );
	signal( SIGQUIT, &sighandler );
	signal( SIGILL, &sighandler );
	signal( SIGTRAP, &sighandler );
	signal( SIGABRT, &sighandler );
	signal( SIGBUS, &sighandler );
	signal( SIGFPE, &sighandler );
	//signal( SIGKILL, &sighandler );
	signal( SIGUSR1, &sighandler );
	signal( SIGSEGV, &sighandler );
	signal( SIGUSR2, &sighandler );
	signal( SIGPIPE, &sighandler );
	signal( SIGALRM, &sighandler );
	signal( SIGTERM, &sighandler );
	signal( SIGSTKFLT, &sighandler );
	//signal( SIGCHLD, &sighandler );
	//signal( SIGCONT, &sighandler );
	//signal( SIGSTOP, &sighandler );
	signal( SIGTSTP, &sighandler );
	signal( SIGTTIN, &sighandler );
	signal( SIGTTOU, &sighandler );
	signal( SIGURG, &sighandler );
	signal( SIGXCPU, &sighandler );
	signal( SIGXFSZ, &sighandler );
	signal( SIGVTALRM, &sighandler );
	signal( SIGPROF, &sighandler );
	//signal( SIGWINCH, &sighandler );
	signal( SIGPWR, &sighandler );
	signal( SIGSYS, &sighandler );

	if ( ref::log.isbad() )
	{
		cout << RED << "Can't open log file, so long bro" << LF;
		return EXIT_FAILURE;
	}

	ref::log.setname( "Matt_daemon" );
	ref::log << TRINFO << "Started" << LF;

	if ( ( ref::lock = open( "/var/lock/Matt_daemon.lock", O_RDONLY | O_CREAT ) ) == -1 )
	{
		cout << RED << "Can't open /var/lock/Matt_daemon.lock, bye bye :3" << LF;
		ref::log << TRERROR << "File locked" << LF;
		return EXIT_FAILURE;
	}

	if ( flock( ref::lock, LOCK_EX | LOCK_NB ) )
	{
		cout << RED << "Can't lock /var/lock/Matt_daemon.lock, i'm outa here" << LF;
		ref::log << TRERROR << "File locked" << LF;
		quit( EXIT_FAILURE );
	}

	ref::log << TRINFO << "Creating server" << LF;

	if ( ( ref::sock = startsrv( 4242 ) ) == -1 )
	{
		cout << RED << "Can't open port, i give up man" << LF;
		quit( EXIT_FAILURE );
	}

	ref::log << TRINFO << "Server created" << LF;
	ref::log << TRINFO << "Entering Daemon mode" << LF;

	if ( !( cstack = malloc( 16384 ) ) )
	{
		cout << RED << "Can't prepare stack for da daemon, shit" << LF;
		quit( EXIT_FAILURE );
	}

	cstack += 16384;

	if ( !( pid = clone( ( int (*)( void * ) ) &number2, cstack, CLONE_VM, NULL ) ) || ( pid == -1 ) )
	{
		cout << RED << "How not to Daemon mode, too bad ..." << LF;
		quit( EXIT_FAILURE );
	}
	_exit( EXIT_SUCCESS );
}
