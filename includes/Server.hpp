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
#include "Client.hpp"
#include "Channel.hpp"
//global variable
bool	g_signal = true;


//class Channel;

class Server {
protected:
	uint16_t				_port;
	std::string				_password;
	int						_socketfd;
	struct sockaddr_in		_addr;
	std::vector<pollfd>		_pollfds;
	nfds_t					_nfds;
	Client					_cli;
	Channel					_chan;
public :
	// Constructors & destructor
	Server(char *port, std::string pw);
	~Server();
	// Public methods
	int				CreatSocket();
	void			messag_handle(std::vector<pollfd>::iterator &it);
	int				signal_handler();
	static void		handler(int sig);
	};

int signal_handler();