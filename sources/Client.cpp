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

void Client::register_nick(const std::string& buff) {

}

void Client::register_user(const std::string& buff) {

}

void Client::register_pass(const std::string& buff) {

}

int Client::CommandClient(const std::string& buff) {
	void (Client::*fptr[3])(const std::string&) = {&Client::register_nick, &Client::register_user, &Client::register_pass};
	std::string tab_com[] = {"NICK", "USER", "PASS"};

	try {
		std::string command = buff.substr(0, buff.find(' '));
		for (int i = 0; i < 3; ++i) {
			if (command == tab_com[i])
				(this->*fptr[i])(buff);
		}
	}
	catch (const std::out_of_range& ex) {
		std::cout << "Client :" << ex.what() << std::endl;
	}

	return 0;
}


// Public methods