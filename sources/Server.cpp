//
// Created by grebrune on 11/20/24.
//

#include "Server.hpp"

// Constructors & destructor
Server::Server(char *port, std::string pw) : _port(static_cast <uint16_t>(std::strtod(port, NULL))), _password(pw), _socketfd(0), _addr(), _nfds(0) {
	//Init sock_addr struct for bind
	_addr.sin_family = AF_INET; // IPv4
	_addr.sin_port = htons(_port); // Port number, convert host to network byte order
	_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all available interfaces (0.0.0.0)

	std::cout << "Server default constructor called!" << std::endl;
}

Server::~Server() {
	std::cout << "Server default destructor called!" << std::endl;
}

// Public methods
int Server::CreatSocket()
{
	int opt = 1;
	
	//Communication domaine en IPv4 ou IPv6 protocol, on choisi IPv4 donc AF_INET pour le domain
	//type de communication en TCP pour le type donc SOCK_STREAM
	//valeur de champs generalement 0 ou 3
	_socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketfd < 0) {
		std::cerr << "Server creation failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	//config _socketfd with setsockopt
	//on declare quel type
	if (setsockopt(_socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		std::cerr << "Setsockopt creation failed: " << strerror(errno) << std::endl;
		close(_socketfd);
		exit(EXIT_FAILURE);
	}
	
	//size of addr struct for addrlen the last parameter
	if (bind(_socketfd, (struct sockaddr *)&_addr, sizeof(_addr))) {
		std::cerr << "Bind creation failed: " << strerror(errno) << std::endl;
		close(_socketfd);
		exit(EXIT_FAILURE);
	}
	//backlog set to 5 because it is the reasonable default number for small server
	if (listen(_socketfd, 5) < 0) {
		std::cerr << "Listen creation failed: " << strerror(errno) << std::endl;
		close(_socketfd);
		exit(EXIT_FAILURE);
	}
	//add the socket to the poll fds
	pollfd server_pollfd;
	server_pollfd.fd = _socketfd;
	server_pollfd.events = POLLIN; // Monitor for incoming connections
	server_pollfd.revents = 0;     // No events yet
	_pollfds.push_back(server_pollfd);
	//add fd to array of _fds
	_fds[_nfds] = server_pollfd;
	//wait for connections to the server
	//change true to the global that is false if a ctrl D or a sigaction ocured
	while (g_signal) {
		//wait to have a action from one of the fds
		if (poll(_fds, _nfds, 1000) < 0)
			exit(EXIT_FAILURE);
		//check if the global value of signal have changed with a if
//		if (g_signal)
//			return ;
		//find the fd that had an event by iterating the list of vector
		for(std::vector<pollfd>::iterator it = _pollfds.begin(); it < _pollfds.end(); it++) {
			if (it->revents == POLLIN) {
				//handle the message of the event
			}
		}
	}
	std::cout << "Server successfully bound to port 8080." << std::endl;
	return 0;
}

int	main(int ac, char **av) {
	if (ac < 3)
		return (std::cout << "Error: Missing arguments." << std::endl, 210);
	signal_handler();
	Server serv(av[1], av[2]);
	serv.CreatSocket();
}
//	if (accept(_socketfd, (struct sockaddr *)&_addr, sizeof(_addr) ) ) < 0) {
//		std::cerr << "Listen creation failed: " << strerror(errno) << std::endl;
//		close(_socketfd);
//		exit(EXIT_FAILURE);
//	}
