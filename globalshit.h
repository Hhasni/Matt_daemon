
#ifndef GLOBALSHIT_H
#define GLOBALSHIT_H

#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/select.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <iostream>
#include <sched.h>
#include <netdb.h>
#include <signal.h>
#include <queue>

#include <colors.h>
#include <ascii.h>
#include <asmtypes.h>

#include <tintinreporter.h>

using namespace std;

#define CLONE_VM 0x00000100

#pragma GCC diagnostic ignored "-Wpointer-arith"

#define BUFF_SIZE 8

#define YAY cout << "Breakpoint passed yay :3" << LF

struct ref
{
	static int lock;
	static int sock;
	static Tintin_reporter log;
};

class user
{
	private:
		int lfc;
		int sock;
		char rdbuff[BUFF_SIZE];
		string strbuff;
		queue<char> buff;
	public:
		user( void );
		user( user & );
		~user( void );
		user &operator=( user & );
		bool in( void );
		bool hasstr( void );
		const char *getstr( void );
		int getsock( void );
		void setsock( int );
};

#endif
