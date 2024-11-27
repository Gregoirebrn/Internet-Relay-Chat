//
// Created by grebrune on 11/26/24.
//

#pragma once

# include <iostream>
# include <map>
# include <sys/socket.h>

class Server;

typedef struct info_s {
	bool			_register;
	std::string		_nickname;
	std::string		_pseudo;
} info_t;

class Client {
private :
	std::map<info_t, int> _map;
public :
	// Constructors & destructor
	Client();
	~Client();
	// Public methods
	int CreateClient(int fd_cli);
	int CommandClient(const std::string& buff);
	void register_nick(const std::string& buff);
	void register_user(const std::string& buff);
	void register_pass(const std::string& buff);
};