//
// Created by grebrune on 11/26/24.
//

#include "Client.hpp"

// Constructors & destructor
Client::Client() {
	std::cout << "Client default destructor called!" << std::endl;
}

Client::Client(const std::string& password) : _password(password) {
	std::cout << "Client default constructor called!" << std::endl;
}

Client::~Client(void) {
	for (std::map<int , info_t>::iterator it = _map.begin(); it != _map.end(); it++) {
		close(it->first); // close all client fd
	}
	std::cout << "Client second destructor called!" << std::endl;
}

int Client::CreateClient(int fd_cli, sockaddr *pSockaddr) {
	_map[fd_cli] = (info_t){._register = false, ._pw_verified = false, ._addr_cli = pSockaddr};
//	std::cout << "LOG CLIENT PSEUDO :" << _map[fd_cli]._pseudo << std::endl;
	return 0;
}

int Client::CommandClient(std::string buff, int fd_cli)
{
	static int (Client::*fptr[3])(std::string, int fd_cli) = {&Client::register_nick, &Client::register_user, &Client::register_pass};
	static std::string tab_com[] = {"NICK", "USER", "PASS"};

	try {
//		std::cout << "COMMAND :" << buff << " " << buff.size();
		for (int i = 0; i < 3; ++i) {
			if (buff.compare(0, 4, tab_com[i]) == 0) {
				std::size_t pos = buff.find(' ');
				if (pos == std::string::npos) {
					std::string ret = tab_com[i];
					return (send_error(fd_cli, ERR_NEEDMOREPARAMS(ret)), 462);
				}
				std::string arg = buff.substr(pos + 1);
				if (arg.find('\r') != std::string::npos) {
					std::cout << "BEFORE: " << arg << std::endl;
					std::string mod = arg.substr(0, arg.size() - 2);
					arg = mod;
					std::cout << "AFTER : " << mod << std::endl;
				}
				(this->*fptr[i])(arg, fd_cli);
				return (0);
			}
		}
	}
	catch (const std::exception& ex) {
		std::cout << "Client :" << ex.what() << std::endl;
	}
	return 0;
}

// error handler
void	send_error(int fd, std::string error) {
	const char *err = error.c_str();
	send(fd, err, strlen(err), 0);
}

//send error to client