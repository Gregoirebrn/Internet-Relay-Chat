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

class Server {
protected:
	Channel					_chan;
	Client					_cli;
	std::vector<pollfd>		_pollfds;
	struct sockaddr_in		_addr;
	std::string				_password;
	nfds_t					_nfds;
	uint16_t				_port;
	int						_socketfd;
public :
	// Constructors & destructor
	Server(char *port, const std::string &pw);
	~Server();
	// Public methods
	int				CreatSocket();
	void			LoopServer();
	void			MessageHandler(std::vector<pollfd>::iterator &it);
	int				SignalHandler();
	static void		Handler(int sig);
};