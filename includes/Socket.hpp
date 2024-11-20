//
// Created by grebrune on 11/20/24.
//

#ifndef INTERNET_RELAY_CHAT_SOCKET_HPP
# define INTERNET_RELAY_CHAT_SOCKET_HPP

# include "irc.hpp"

class Socket {
private :
	int						_socketfd;
	struct sockaddr_in		_addr;
	std::vector<pollfd>		_pollfds;
	nfds_t					_nfds;
	struct pollfd			_fds[];
public :
	// Constructors & destructor
	Socket();
	~Socket();
	// Public methods
	int CreatSocket();
};

#endif //INTERNET_RELAY_CHAT_SOCKET_HPP