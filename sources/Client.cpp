//
// Created by grebrune on 11/26/24.
//

#include "Client.hpp"

// Constructors & destructor
Client::Client(void) {
	std::cout << "Client default constructor called!" << std::endl;
}

Client::~Client(void) {
	std::cout << "Client default destructor called!" << std::endl;
}

int Client::CreateClient(int fd_cli) {
	send(fd_cli, );
	return 0;
}

// Public methods