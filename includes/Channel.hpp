//
// Created by grebrune on 12/2/24.
//

#pragma once

# include <iostream>
# include <vector>
//internal libs
# include "Client.hpp"

class Server;

class Channel {
private :
	std::map<std::string , std::vector<std::string> > _all_chan;
public :
	// Constructors & destructor
	Channel();
	~Channel();
	// Public methods
	int		Canal_Operators(std::string buff, int fd_cli);
	int		Kick(std::string buff, int fd_cli);
	int		Invite(std::string buff, int fd_cli);
	int		Topic(std::string buff, int fd_cli);
	int		Mode(std::string buff, int fd_cli);
	};

