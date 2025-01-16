//
// Created by grebrune on 12/10/24.
//

#include "Client.hpp"

int Client::User(const std::string& buff, int fd_cli) {
	if (!_clients[fd_cli]._pw_verified)
		return (SendMessage(fd_cli, ERR_PWNOTCHECK), 808);
	if (buff.size() < 4)
		return (SendMessage(fd_cli, ERR_NEEDMOREPARAMS("USER")), 461);
	if (_clients[fd_cli]._pw_verified && !_clients[fd_cli]._username.empty())
			return (SendMessage(fd_cli, ERR_ALREADYREGISTRED(_clients[fd_cli]._username)), 462);
	try {
		if (std::string::npos == buff.find(' '))
			return (SendMessage(fd_cli, ERR_NEEDMOREPARAMS("USER")), 461);
		std::string user = buff.substr(0, buff.find(' '));
		if (user.empty())
			return (SendMessage(fd_cli, ERR_NEEDMOREPARAMS("USER")), 461);
		std::string trim = buff.substr(buff.find(' '), buff.size());
		if (trim.find(" 0 * ") == std::string::npos)
			return (SendMessage(fd_cli, ERR_NEEDMOREPARAMS("USER")), 461);
		std::string rn =  buff.substr(buff.find('*') + 2, buff.size());
		_clients[fd_cli]._realname = rn;
		if (10 < user.size())
			user = user.substr(0, 10);
		_clients[fd_cli]._username = user;
	}
	catch (std::exception &e) {
		return (SendMessage(fd_cli, ERR_NEEDMOREPARAMS("USER")), 461);
	}
	if (!_clients[fd_cli]._nickname.empty()){
		_clients[fd_cli]._prefix = _clients[fd_cli]._nickname + "!" + _clients[fd_cli]._username + "@" + _clients[fd_cli]._hostname;
		_clients[fd_cli]._register = true;
		return (SendMessage(fd_cli, RPL_WELCOME(_clients[fd_cli]._nickname, _clients[fd_cli]._nickname)), 1);
	}
	return (0);
}
