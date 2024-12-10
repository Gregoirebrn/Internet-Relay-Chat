//
// Created by grebrune on 12/10/24.
//

#include "Client.hpp"

int Client::register_pass(std::string arg, int fd_cli) {
	if (_map[fd_cli]._pw_verified)
		return (send(fd_cli, ERR_ALREADYREGISTRED, sizeof(ERR_ALREADYREGISTRED), 0), 462);
	if (arg.empty() || arg.size() <= 1) {
		send(fd_cli, ERR_ALREADYREGISTRED, sizeof(ERR_ALREADYREGISTRED), 0);
		return (462); // handle error
	}
	if (_password == arg)
		_map[fd_cli]._pw_verified = true;
	if (!_map[fd_cli]._pw_verified)
		return (send_error(fd_cli, ERR_PASSWDMISMATCH), 464);
	return (0);
}