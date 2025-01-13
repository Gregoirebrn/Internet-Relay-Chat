//
// Created by grebrune on 11/26/24.
//

#include "Client.hpp"

// Constructors & destructor
Client::Client(const std::string& password) : _password(password) {
}

Client::~Client(void) {
	for (list_t it = _clients.begin(); it != _clients.end(); it++) {
		close(it->first); // close all client fd
	}
}

//getter
int	Client::GetFd(const std::string& nick) {
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

std::string Client::GetWho(int fd_cli, const std::string& channel) {
	_clients[fd_cli]._who = _clients[fd_cli]._nickname + " " + channel + " " + _clients[fd_cli]._username + " ";
	_clients[fd_cli]._who += _clients[fd_cli]._hostname + " irc_server " + _clients[fd_cli]._nickname + " H@~ :0 " + _clients[fd_cli]._realname;
	return _clients[fd_cli]._who;
}

int Client::CreateClient(int fd_cli, sockaddr_in addr_srv) {
	_clients[fd_cli] = (info_t){._register = false, ._pw_verified = false, ._nickname = "", \
	._username = "", ._realname = "", ._hostname = "", ._prefix = "", ._who = "", ._addr_cli = addr_srv};
	char host[1024];

	char *client_ip = inet_ntoa(addr_srv.sin_addr);
	int res = getnameinfo((sockaddr *)&addr_srv, sizeof(addr_srv), host, 1024, NULL, 0, 0);
	if (res != 0)
		return (std::cout << "Client: Error: Get name info failed :" << gai_strerror(res) << std::endl, 4040);
	_clients[fd_cli]._hostname = client_ip;
	return 0;
}

bool Client::CommandClient(const std::string& buff, int fd_cli)
{
	static int (Client::*fptr[4])(const std::string&, int fd_cli) = {&Client::Nick, \
	&Client::User, &Client::Pass};
	static std::string tab_com[] = {"NICK", "USER", "PASS"};

	try {
		for (int i = 0; i < 3; ++i) {
			if (!buff.compare(0, tab_com[i].size(), tab_com[i])) {
				if (std::string::npos == buff.find(' ')) { //send nothing to the command so she throw the apropriate error
					(this->*fptr[i])("", fd_cli);
					return true ;
				}
				std::string arg = buff.substr((buff.find(' ') + 1), buff.size());
				if (arg.find('\r') != std::string::npos) // if we are on Hexchat
					arg = arg.substr(0, arg.size() - 1);
				(this->*fptr[i])(arg, fd_cli);
				return true ;
			}
		}
	}
	catch (const std::exception& ex) {
		std::cout << "Client: Error:" << ex.what() << std::endl;
	}
	return false;
}

void	Client::Remove(int fd_cli) {
	_clients.erase(fd_cli);
}

// error Handler
void	SendMessage(int fd, const std::string& error) {
	const char *err = error.c_str();
	send(fd, err, strlen(err), 0);
}

//send error to client
void	Client::SendServMessage(const std::string& msg) {
	for (list_t it = _clients.begin(); it != _clients.end(); ++it) {
		SendMessage(it->first, msg);
	}
}