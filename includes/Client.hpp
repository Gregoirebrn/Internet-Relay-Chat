//
// Created by grebrune on 11/26/24.
//

#pragma once

# include <iostream>
# include <map>
# include <sys/
class Server;

typedef struct info_s {
	std::string nickname;
	std::string pseudo;
} info_t;

class Client {
private :
	std::map<info_t, int> _map;
public :
	// Constructors & destructor
	Client();
	~Client();
	// Public methods
	static int CreateClient(int fd_cli);
};