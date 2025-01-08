//
// Created by grebrune on 12/10/24.
//

#include "Client.hpp"

int Client::register_user(std::string buff, int fd_cli) {
	if (!_clients[fd_cli]._pw_verified)
		return (send_error(fd_cli, ERR_PWNOTCHECK), 808);
	if (buff.size() < 4)
		return (send_error(fd_cli, ERR_NEEDMOREPARAMS("USER")), 461);
	if (!_clients[fd_cli]._username.empty())
		return(send_error(fd_cli, ERR_ALREADYREGISTRED), 462);
	try {
		if (std::string::npos == buff.find(' '))
			throw (Client::GradeTooHighException());
		std::string ps = buff.substr(0, buff.find(' '));
		if (ps.empty())
			throw (Client::GradeTooHighException());
		std::string trim = buff.substr(buff.find(' '), buff.size());
		if (trim.find(" 0 * ") == std::string::npos)
			throw (Client::GradeTooHighException());
		std::string rn =  buff.substr(buff.find('*') + 2, buff.size());
		_clients[fd_cli]._realname = rn;
		if (USERLEN < ps.size())
			ps = ps.substr(0, USERLEN);
		_clients[fd_cli]._username = ps;
	}
	catch (std::exception &e) {
		return (send_error(fd_cli, ERR_NEEDMOREPARAMS("USER")), 461);
	}
	if (!_clients[fd_cli]._nickname.empty()){
		_clients[fd_cli]._prefix = _clients[fd_cli]._nickname + "!" + _clients[fd_cli]._username + "@" + _clients[fd_cli]._hostname;
//		std::cout << "---INFO------------:" << std::endl;
//		std::cout << "---PSEUDO :" << _clients[fd_cli]._username << std::endl;
//		std::cout << "---NICK   :" << _clients[fd_cli]._nickname << std::endl;
//		std::cout << "---PREFIX :" << _clients[fd_cli]._prefix << std::endl;
//		std::cout << "---END-------------:" << std::endl;
		_clients[fd_cli]._register = true;
		return (send_error(fd_cli, RPL_WELCOME(_clients[fd_cli]._nickname, _clients[fd_cli]._nickname)), 462);
	}
	return (0);
}

const char *Client::GradeTooHighException::what() const throw() {
	return ("The grade is Too High.");
}
