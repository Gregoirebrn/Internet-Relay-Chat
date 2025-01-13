//
// Created by grebrune on 11/26/24.
//

#pragma once

# include <iostream>
# include <map>
# include <vector>
# include <sys/socket.h>
# include <csignal>
# include <cstring>
# include <algorithm>
# include <sstream>
# include <netdb.h>
# include <arpa/inet.h>

//RFC
# include "LimitedModernRfc.hpp"

class Server;

typedef struct info_s {
	bool			_register;
	bool			_pw_verified;
	std::string		_nickname;
	std::string		_username;
	std::string		_realname;
	std::string		_hostname;
	std::string		_prefix;
	std::string		_who;
	sockaddr_in		_addr_cli;
} info_t;

typedef std::map<int , info_t>::iterator	list_t;
typedef std::vector<std::string>::iterator	vec_t;


class Client {
protected :
	std::string				_password;
	std::map<int , info_t>	_clients;
public :
	// Constructors & destructor
	Client(const std::string& password);
	~Client();
	// Getter
	int				GetFd(const std::string&);
	std::string		GetName(int fd_cli);
	std::string		GetPrefix(int fd_cli);
	std::string		GetWho(int fd_cli, const std::string& channel);
	bool			IsRegister(int fd_cli);
	// Public methods
	bool	CommandClient(const std::string& buff, int fd_cli);
	int		CreateClient(int fd_cli, sockaddr_in pSockaddr);
	int		Nick(const std::string& buff, int fd_cli);
	int		User(const std::string& buff, int fd_cli);
	int		Pass(const std::string& buff, int fd_cli);
	// Quit handle
	void	Remove(int fd_cli);
	// Send msg
	void	SendServMessage(const std::string& msg);
};

	void	SendMessage(int fd, const std::string& error);
