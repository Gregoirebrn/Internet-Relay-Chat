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
	std::map<std::string , std::vector<int> > _all_chan;
public :
	// Constructors & destructor
	Channel();
	~Channel();
	// Public methods
	void	Canal_Operators(std::string buff);
	};

