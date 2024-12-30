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
#include <netdb.h>

//RFC
# include "rfc_1459.hpp"

class Server;

typedef struct info_s {
	bool			_register;
	bool			_pw_verified;
	std::string		_nickname;
	std::string		_username;
	std::string		_realname;
	std::string		_hostname;
	std::string		_prefix;
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
	Client();
	~Client();
	//getter
	std::map<int , info_t>	GetClients();
	int						GetFd(std::string);
	std::string				GetName(int fd_cli);
	std::string				GetPrefix(int fd_cli);
	std::string				GetUser(int fd_cli);
	bool					IsRegister(int fd_cli);
	Client(const std::string& password);
	// Public methods
	int CreateClient(int fd_cli, sockaddr_in pSockaddr);
	int CommandClient(std::string buff, int fd_cli);
	int register_nick(std::string buff, int fd_cli);
	int register_user(std::string buff, int fd_cli);
	int register_pass(std::string buff, int fd_cli);
	//quit handle
	void	Remove(int fd_cli);
	//send msg
	void	send_all_serv(std::string msg);
	int		send_private(std::string buff, int fd_cli);
	int		send_cli_msg(std::string nick, std::string msg, int fd_sender);
	class GradeTooHighException : public std::exception {
	public:
		const char* what() const throw();
	};
};

	void	send_error(int fd, std::string error);
