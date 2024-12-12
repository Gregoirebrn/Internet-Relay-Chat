//
// Created by grebrune on 11/26/24.
//

#pragma once

# include <iostream>
# include <map>
# include <sys/socket.h>
# include <csignal>
# include <cstring>
# 	include <algorithm>

//RFC
# include "rfc_1459.hpp"

class Server;

typedef struct info_s {
	bool			_register;
	bool			_pw_verified;
	std::string		_nickname;
	std::string		_pseudo;
	sockaddr		*_addr_cli;
} info_t;

class Client {
private :
	std::string				_password;
	std::map<int , info_t>	_clients;
public :
	// Constructors & destructor
	Client();
	~Client();
	//getter
	std::string Get_Client_Name(int fd_cli);
	Client(const std::string& password);
	// Public methods
	int CreateClient(int fd_cli, sockaddr *pSockaddr);
	int CommandClient(std::string buff, int fd_cli);
	int register_nick(std::string buff, int fd_cli);
	int register_user(std::string buff, int fd_cli);
	int register_pass(std::string buff, int fd_cli);
	//quit handle
	void	Remove(int fd_cli);
	};

	void	send_error(int fd, std::string error);
