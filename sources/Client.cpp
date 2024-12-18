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
	for (std::map<int , info_t>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		close(it->first); // close all client fd
	}
	std::cout << "Client second destructor called!" << std::endl;
}

//getter
std::map<int , info_t> Client::GetClients(){
	return (this->_clients);
}

int	Client::GetFd(std::string nick) {
	for (list_t it = _clients.begin(); it != _clients.end(); ++it) {
		std::cout << "FINDER_FD " << it->first << ":" << it->second._nickname << std::endl;
		if (it->second._nickname == nick) {
			std::cout << "IT_FIRST :" << it->first << std::endl;
			return (it->first);
		}
	}
//	for (list_t it = _clients.begin(); it != _clients.end(); ++it) {
//		std::cout << "---------FINDER_FD " << it->first << ":" << it->second._nickname << std::endl;
//		std::cout << "---------IT_FIRST :" << it->first << std::endl;
//	}
//	std::cout << "EMPTYYYYYYYYYYYYYYYYYY" << std::endl;
	return (-1);
}

std::string Client::GetName(int fd_cli) {
	if (_clients[fd_cli]._nickname.empty())
		return ("");
	return _clients[fd_cli]._nickname;
}

std::string Client::GetPrefix(int fd_cli) {
	return _clients[fd_cli]._prefix;
}

int Client::CreateClient(int fd_cli, sockaddr *ClientAddr) {
	char host[NI_MAXHOST];
	_clients[fd_cli] = (info_t){._register = false, ._pw_verified = false, \
	._nickname = "", ._pseudo = "", ._realname = "", ._hostname = "", ._prefix = "", \
	._addr_cli = ClientAddr};
	int res = getnameinfo(ClientAddr, sizeof(ClientAddr), host, NI_MAXHOST, NULL, 0, 0);
	if (res != 0)
		return (std::cout << "Client :getnameinfo failed :" << gai_strerror(res) << std::endl, 4040);
	_clients[fd_cli]._hostname = host;
	_clients[fd_cli]._prefix = _clients[fd_cli]._nickname + "!" + _clients[fd_cli]._pseudo + "@" + _clients[fd_cli]._hostname;
	std::cout << "CLIENT_PRFIX :" << _clients[fd_cli]._prefix << std::endl;
	return 0;
}

int Client::CommandClient(std::string buff, int fd_cli)
{
	static int (Client::*fptr[4])(std::string, int fd_cli) = {&Client::register_nick, &Client::register_user, \
	&Client::register_pass, &Client::send_private};
	static std::string tab_com[] = {"NICK", "USER", "PASS", "PRIVMSG"};

	try {
//		std::cout << "COMMAND :" << buff << " " << buff.size();
		for (int i = 0; i < 4; ++i) {
			if (!buff.compare(0, tab_com[i].size(), tab_com[i])) {
				std::size_t pos = buff.find(' ');
				if (pos == std::string::npos) {
					std::string ret = tab_com[i];
					return (send_error(fd_cli, ERR_NEEDMOREPARAMS(tab_com[i])), 462);
				}
//				if (pos + 1 >= buff.size())
//					throw std::exception() ;
				std::string arg = buff.substr(pos + 1);
				if (arg.find('\r') != std::string::npos) {
//					std::cout << "BEFORE:" << arg << std::endl;
					std::string mod = arg.substr(0, arg.size() - 1);
					arg = mod;
//					std::cout << "AFTER :" << mod << std::endl;
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

void	Client::Remove(int fd_cli) {
	_clients.erase(fd_cli);
}

// error handler
void	send_error(int fd, std::string error) {
	const char *err = error.c_str();
	send(fd, err, strlen(err), 0);
}

//send error to client
int	Client::send_cli_msg(std::string nick, std::string msg, int fd_sender) {
//	std::cout << "ITERATOR :" << nick << std::endl;
	int fd_msg = GetFd(nick);
//	std::cout << "FDCLIENT:" << fd_msg << std::endl;
	std::string full_msg = ":" + GetName(fd_sender) + " PRIVMSG " + nick + " " + msg;
	if (fd_msg > 0)
		return (send_error(fd_msg, full_msg), 0);
	return (1);
}

int	Client::send_private(std::string buff, int fd_cli) {
	if (buff.find(' ') == std::string::npos)
		return (send_error(fd_cli, ERR_NEEDMOREPARAMS(buff)), 461);
	buff = buff.substr(0 , buff.size() - 1);
	std::vector<std::string> targets;
	std::istringstream haystack;
	haystack.str(buff);
	for (std::string line; std::getline(haystack, line, ',');) {
		if (line.find(' ') != std::string::npos) {
			targets.push_back(line.substr(0, line.find(' ')));
			break;
		}
		targets.push_back(line);
	}
//	for (vec_t it = targets.begin(); it != targets.end() ; ++it) {
//		std::cout << "TARGETS :" << *it << std::endl;
//	}
	std::string msg = buff.substr((buff.find(targets.back()) + targets.back().size() + 1), buff.size());
	for (vec_t it = targets.begin(); it != targets.end() ; ++it) {
		if (it->find('#') == std::string::npos) { //its for one user check the nick exist & get the fd
			if (send_cli_msg(*it, msg, fd_cli))
				send_error(fd_cli, ERR_NOSUCHNICK(GetName(fd_cli), *it));
		}
	}
	return (0);
}
