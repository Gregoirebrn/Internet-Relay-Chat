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
	send(fd_cli, "Enter nickname :", 16, );
	return 0;
}

int Client::CommandClient(std::string buff) {
		void	(Server::*fptr[])() = {&co_kick(buff), Server::co_invite, &Server::co_topic, &Server::co_mode};
	std::string tab_com[] = {"DEBUG", "ERROR", "INFO", "WARNING"};
	return 0;
}

// Public methods