//
// Created by grebrune on 12/2/24.
//

#pragma once

# include <iostream>
# include <vector>
#include <bits/stdc++.h>
//internal libs
# include "Client.hpp"
//# include "Server.hpp"

class Server;

typedef struct mod_s {
	std::string		chan_key;
	std::string		topic;
	size_t			max_user;
}	mod_t;

typedef std::map<std::string , std::map<std::string, bool> >::iterator iter_chan_user_t;
typedef std::map<std::string, bool>::iterator iter_chan_t;

class Channel : public Client{
private :
	std::map<std::string, std::map<std::string, bool> >	_chan_user;
	std::map<std::string, mod_t>						_all_chan;
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
	int		Join(std::string buff, int fd_cli);
	//utils for commands
	bool	get_rights(std::string name, std::string chan, int fd_cli);
	};

