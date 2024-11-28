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
	std::cout << "LOG CLIENT PSEUDO :" << _map[fd_cli]._pseudo << std::endl;
	return 0;
}

int Client::register_nick(std::string buff, int fd_cli) {
	if (!_map[fd_cli]._pw_verified)
		return (send(fd_cli, ERR_NEEDPASS, sizeof(ERR_NEEDPASS), 0), 808);
	_map[fd_cli]._nickname = buff.substr(5, buff.find(' '));
	std::cout << "CLIENT :" << fd_cli << " NICK :" << _map[fd_cli]._nickname << std::endl;
	return (0);
}

int Client::register_user(std::string buff, int fd_cli) {
	if (!_map[fd_cli]._pw_verified)
		return (send(fd_cli, ERR_NEEDPASS, sizeof(ERR_NEEDPASS), 0), 808);
	_map[fd_cli]._pseudo = buff.substr(5, buff.find(' '));
	std::cout << "CLIENT :" << fd_cli << " PSEUDO :" << _map[fd_cli]._pseudo << std::endl;
	return (0);
}

int Client::register_pass(const std::string arg, int fd_cli) {
	std::cout << arg << std::endl;
	if (_map[fd_cli]._pw_verified)
		return (send(fd_cli, ERR_ALREADYREGISTRED, sizeof(ERR_ALREADYREGISTRED), 0), 462);
	if (arg.empty() || arg.size() <= 1) {
		send(fd_cli, ERR_ALREADYREGISTRED, sizeof(ERR_ALREADYREGISTRED), 0);
		return (462); // handle error
	}
	if (_password == arg)
		_map[fd_cli]._pw_verified = true;
	std::cout << "CLIENT :" << fd_cli << " Password :" << _map[fd_cli]._pw_verified << std::endl;
	if (!_map[fd_cli]._pw_verified)
		return (send_error(fd_cli, ERR_NEEDMOREPARAMS("PASS")), 462);
	return (0);
}

int Client::CommandClient(std::string buff, int fd_cli)
{
	static int (Client::*fptr[3])(std::string, int fd_cli) = {&Client::register_nick, &Client::register_user, &Client::register_pass};
	static std::string tab_com[] = {"NICK", "USER", "PASS"};

	(void)fd_cli;
	try {
//		std::cout << "COMMAND :" << ":" << buff << " " << buff.size() << std::endl;
		for (int i = 0; i < 3; ++i) {
			if (buff.compare(0, 4, tab_com[i]) == 0) {
				std::size_t pos = buff.find(' ');
				if (pos == std::string::npos)
					return (send_error(fd_cli, ERR_NEEDMOREPARAMS((char *)tab_com[i])), 462);
				std::string arg = buff.substr(pos + 1);
//				std::cout << "ARGGGG :" << arg << std::endl;
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
void Client::send_error(int fd, const char *error) {
	send(fd, error, strlen(error), 0);
}

//send error to client