//
// Created by grebrune on 12/10/24.
//

#include "Client.hpp"

int Client::Pass(const std::string& buff, int fd_cli) {
	if (_clients[fd_cli]._pw_verified)
		return (SendMessage(fd_cli, ERR_ALREADYREGISTRED), 462);
	if (buff.empty() || buff.size() <= 1)
		return (SendMessage(fd_cli, ERR_NEEDMOREPARAMS("PASS")), 461);
	if (_password == buff)
		_clients[fd_cli]._pw_verified = true;
	if (!_clients[fd_cli]._pw_verified)
		return (SendMessage(fd_cli, ERR_PASSWDMISMATCH), 464);
	return (0);
}