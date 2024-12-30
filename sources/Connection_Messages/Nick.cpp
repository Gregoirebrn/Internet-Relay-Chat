//
// Created by grebrune on 12/10/24.
//

#include "Client.hpp"

int Client::register_nick(std::string buff, int fd_cli) {
	if (!_clients[fd_cli]._pw_verified)
		return (send_error(fd_cli, ERR_PWNOTCHECK), 808);
	if (buff.empty())
		return (send_error(fd_cli, ERR_NONICKNAMEGIVEN), 431);
	std::string charset = "=#&:";
	for (int i = 0; buff[i]; ++i) {
		if (std::string::npos != charset.find(buff[i]) || iswspace(buff[i]) || (i == 0 && isdigit(buff[i])))
			return (send_error(fd_cli, ERR_ERRONEUSNICKNAME(buff)), 432);
	}
	for (list_t it = _clients.begin() ; it != _clients.end(); ++it) {
		if (it->second._nickname == buff)
			return (send_error(fd_cli, ERR_NICKNAMEINUSE(buff)), 432);
	}
	if (!_clients[fd_cli]._nickname.empty()) {
		std::string old_prefix = _clients[fd_cli]._prefix;
		std::string old_nickname = _clients[fd_cli]._nickname;
		_clients[fd_cli]._nickname = buff;
		_clients[fd_cli]._prefix = _clients[fd_cli]._nickname + "!" + _clients[fd_cli]._username + "@" + _clients[fd_cli]._hostname;
		send_all_serv(RPL_PRENICK(old_prefix, buff));
		return (send_all_serv(RPL_NICK(old_nickname, buff)), 404);
	}
	_clients[fd_cli]._nickname = buff;
	if (!_clients[fd_cli]._username.empty()){
		_clients[fd_cli]._prefix = _clients[fd_cli]._nickname + "!" + _clients[fd_cli]._username + "@" + _clients[fd_cli]._hostname;
		std::cout << "---INFO------------:" << std::endl;
		std::cout << "---PSEUDO :" << _clients[fd_cli]._username << std::endl;
		std::cout << "---NICK   :" << _clients[fd_cli]._nickname << std::endl;
		std::cout << "---PREFIX :" << _clients[fd_cli]._prefix << std::endl;
		std::cout << "---END-------------:" << std::endl;
		_clients[fd_cli]._register = true;
		return (send_error(fd_cli, RPL_WELCOME(_clients[fd_cli]._username, _clients[fd_cli]._nickname, _clients[fd_cli]._prefix)), 462);
	}
	return (0);
}