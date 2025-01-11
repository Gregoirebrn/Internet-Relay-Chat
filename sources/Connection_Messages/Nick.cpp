//
// Created by grebrune on 12/10/24.
//

#include "Client.hpp"

int Client::Nick(const std::string& buff, int fd_cli) {
	if (!_clients[fd_cli]._pw_verified)
		return (SendMessage(fd_cli, ERR_PWNOTCHECK), 808);
	if (buff.empty())
		return (SendMessage(fd_cli, ERR_NONICKNAMEGIVEN), 431);
	std::string charset = "=#&:";
	for (int i = 0; buff[i]; ++i) {
		if (std::string::npos != charset.find(buff[i]) || iswspace(buff[i]) || (i == 0 && isdigit(buff[i])))
			return (SendMessage(fd_cli, ERR_ERRONEUSNICKNAME(buff)), 432);
	}
	if ("bot" == buff)
		return (SendMessage(fd_cli, ERR_NICKNAMEINUSE(buff)), 432);
	for (list_t it = _clients.begin() ; it != _clients.end(); ++it) {
		if (it->second._nickname == buff)
			return (SendMessage(fd_cli, ERR_NICKNAMEINUSE(buff)), 432);
	}
	if (!_clients[fd_cli]._nickname.empty()) {
		SendServMessage(RPL_PRENICK(_clients[fd_cli]._prefix, buff));
		SendServMessage(RPL_NICK(_clients[fd_cli]._nickname, buff));
		_clients[fd_cli]._nickname = buff;
		_clients[fd_cli]._prefix = _clients[fd_cli]._nickname + "!" + _clients[fd_cli]._username + "@" + _clients[fd_cli]._hostname;
		return (404);
	}
	_clients[fd_cli]._nickname = buff;
	if (!_clients[fd_cli]._username.empty()) {
		_clients[fd_cli]._prefix = _clients[fd_cli]._nickname + "!" + _clients[fd_cli]._username + "@" + _clients[fd_cli]._hostname;
		_clients[fd_cli]._register = true;
		return (SendMessage(fd_cli, RPL_WELCOME(_clients[fd_cli]._nickname, _clients[fd_cli]._nickname)), 1);
	}
	return (0);
}