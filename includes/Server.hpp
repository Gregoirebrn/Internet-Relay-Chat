//
// Created by grebrune on 11/20/24.
//
#pragma once

//C++ libs
#include <poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <vector>
//C libs
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csignal>
//inner lib
#include "Server.hpp"
#include "Server.hpp"
//global variable

bool	g_signal = true;

class Server {
private :
	uint16_t				_port;
	std::string				_password;
	int						_socketfd;
	struct sockaddr_in		_addr;
	std::vector<pollfd>		_pollfds;
	nfds_t					_nfds;
	struct pollfd			_fds[];
public :
	// Constructors & destructor
	Server(char *port, std::string pw);
	~Server();
	// Public methods
	int CreatSocket();
};

int signal_handler();