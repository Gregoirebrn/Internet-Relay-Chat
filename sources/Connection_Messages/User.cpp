//
// Created by grebrune on 12/10/24.
//

#include "Client.hpp"

int Client::register_user(std::string buff, int fd_cli) {
	if (!_map[fd_cli]._pw_verified)
		return (send(fd_cli, ERR_NOTREGISTRED, sizeof(ERR_NOTREGISTRED), 0), 808);
	if (!_map[fd_cli]._pseudo.empty())
		return(send_error(fd_cli, ERR_ALREADYREGISTRED), 462);
	_map[fd_cli]._pseudo = buff;
	std::cout << "CLIENT :" << fd_cli << " PSEUDO :" << _map[fd_cli]._pseudo << std::endl;
	return (0);
}