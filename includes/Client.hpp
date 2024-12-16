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

//RFC
# include "rfc_1459.hpp"

class Server;

typedef struct info_s {
	bool			_register;
	bool			_pw_verified;
	std::string		_nickname;
	std::string		_pseudo;
	std::string		_realname;
	sockaddr		*_addr_cli;
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
	Client(const std::string& password);
	// Public methods
	int CreateClient(int fd_cli, sockaddr *pSockaddr);
	int CommandClient(std::string buff, int fd_cli);
	int register_nick(std::string buff, int fd_cli);
	int register_user(std::string buff, int fd_cli);
	int register_pass(std::string buff, int fd_cli);
	//quit handle
	void	Remove(int fd_cli);
	//send msg
	int		send_private(std::string buff, int fd_cli);
	int		send_cli_msg(std::string nick, std::string msg, int fd_sender);
	class GradeTooHighException : public std::exception {
	public:
		const char* what() const throw();
	};
};

	void	send_error(int fd, std::string error);
