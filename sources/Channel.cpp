//
// Created by grebrune on 12/2/24.
//

#include "Channel.hpp"

// Constructors & destructor
Channel::Channel(void) {
	std::cout << "Channel default constructor called!" << std::endl;
}

Channel::~Channel(void) {
	std::cout << "Channel default destructor called!" << std::endl;
}

// Public methods
void	Channel::Canal_Operators(std::string buff) {
	void	(Server::*fptr[])() = {&Channel::co_kick(buff), &Channel::co_invite, &Channel::co_topic, &Channel::co_mode};
	std::string tab_com[] = {"KICK", "ERROR", "INFO", "WARNING"};
}