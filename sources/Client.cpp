//
// Created by grebrune on 11/26/24.
//

#include "Client.hpp"

// Constructors & destructor
Client::Client() {
//	std::cout << "Client default destructor called!" << std::endl;
}

Client::Client(const std::string& password) : _password(password) {
//	std::cout << "Client default constructor called!" << std::endl;
}

Client::~Client(void) {
	for (std::map<int , info_t>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		close(it->first); // close all client fd
	}
//	std::cout << "Client second destructor called!" << std::endl;
}

//getter
std::map<int , info_t> Client::GetClients(){
	return (this->_clients);
}

int	Client::GetFd(std::string nick) {
	for (list_t it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->second._nickname == nick) {
			return (it->first);
		}
	}
	return (0);
}

std::string Client::GetName(int fd_cli) {
	if (_clients[fd_cli]._nickname.empty())
		return ("");
	return _clients[fd_cli]._nickname;
}

bool Client::IsRegister(int fd_cli) {
	return _clients[fd_cli]._register;
}

std::string Client::GetPrefix(int fd_cli) {
	return _clients[fd_cli]._prefix;
}

std::string Client::GetUser(int fd_cli) {
	if (_clients[fd_cli]._username.empty())
		return ("");
	return _clients[fd_cli]._username;
}

std::string Client::GetWho(int fd_cli, std::string channel) {
	_clients[fd_cli]._who = _clients[fd_cli]._nickname + " " + channel + " " + _clients[fd_cli]._username + " ";
	_clients[fd_cli]._who += _clients[fd_cli]._hostname + " irc_server " + _clients[fd_cli]._nickname + " H@~ :0 " + _clients[fd_cli]._realname;
	return _clients[fd_cli]._who;
}

int Client::CreateClient(int fd_cli, sockaddr_in addr_srv) {
	_clients[fd_cli] = (info_t){._register = false, ._pw_verified = false, ._nickname = "", ._username = "", ._realname = "", ._hostname = "", ._prefix = "", ._addr_cli = addr_srv};
	char host[1024];

	char *client_ip = inet_ntoa(addr_srv.sin_addr);
//	std::cout << "Client IP: " << client_ip << std::endl;
	int res = getnameinfo((sockaddr *)&addr_srv, sizeof(addr_srv), host, 1024, NULL, 0, 0);
	if (res != 0)
		return (std::cout << "Client :getnameinfo failed :" << gai_strerror(res) << std::endl, 4040);
	_clients[fd_cli]._hostname = client_ip;
	return 0;
}

int Client::CommandClient(std::string buff, int fd_cli)
{
	// Fast checkin
	if (buff == "ff\r") {
		register_pass("pass", fd_cli);
		register_nick("defni", fd_cli);
		register_user("defus 0 * :noreal", fd_cli);
		return (3);
	}
	if (buff == "fff\r") {
		register_pass("pass", fd_cli);
		register_nick("fn", fd_cli);
		register_user("fu 0 * :norh", fd_cli);
		return (3);
	}
	//End Fast Check
	static int (Client::*fptr[4])(std::string, int fd_cli) = {&Client::register_nick, &Client::register_user, \
	&Client::register_pass};
	static std::string tab_com[] = {"NICK", "USER", "PASS"};

	try {
		for (int i = 0; i < 3; ++i) {
			if (!buff.compare(0, tab_com[i].size(), tab_com[i])) {
				if (std::string::npos == buff.find(' ')) { //send nothing to the command so she throw the apropriate error
					(this->*fptr[i])("", fd_cli);
					return (0);
				}
				std::string arg = buff.substr((buff.find(' ') + 1), buff.size());
				if (arg.find('\r') != std::string::npos) // if we aree on Hexchat
					arg = arg.substr(0, arg.size() - 1);
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
void	Client::send_all_serv(std::string msg) {
//	std::cout << "---ALL_SERV---" << std::endl;
	for (list_t it = _clients.begin(); it != _clients.end(); ++it) {
		send_error(it->first, msg);
	}
}
