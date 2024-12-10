//
// Created by grebrune on 12/10/24.
//

#include "Client.hpp"

int Client::register_nick(std::string buff, int fd_cli) {
	if (buff.empty())
		return (send_error(fd_cli, ERR_NONICKNAMEGIVEN), 431);
	if (!_map[fd_cli]._pw_verified)
		return (send(fd_cli, ERR_NOTREGISTRED, sizeof(ERR_NOTREGISTRED), 0), 808);
	std::string charset = "=#&:";
	for (int i = 0; buff[i]; ++i) {
		if (std::string::npos != charset.find(buff[i]) || iswspace(buff[i]) || (i == 0 && isdigit(buff[i])))
			return (send_error(fd_cli, ERR_ERRONEUSNICKNAME(buff)), 432);
	}
	for (std::map<int, info_t>::iterator it = _map.begin() ; it != _map.end(); ++it) {
		if (it->second._nickname == buff)
			return (send_error(fd_cli, ERR_NICKNAMEINUSE(buff)), 432);
	}
	_map[fd_cli]._nickname = buff;
	std::cout << "CLIENT :" << fd_cli << " NICK :" << _map[fd_cli]._nickname << std::endl;
	return (0);
}