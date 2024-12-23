//
// Created by grebrune on 11/20/24.
//

#include "Server.hpp"

// Constructors & destructor
Server::Server(char *port, std::string pw) : _port(static_cast <uint16_t>(std::strtod(port, NULL))), _password(pw), _addr(), _nfds(0), _cli(pw), _chan(&_cli) {
	//Init sock_addr struct for bind
	_addr.sin_family = AF_INET; // IPv4
	_addr.sin_port = htons(_port); // Port number, convert host to network byte order
	_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all available interfaces (0.0.0.0)
	signal_handler();
//	std::cout << "Server default constructor called!" << std::endl;
}

Server::~Server() {
	close(_socketfd);
//	std::cout << "Server default destructor called!" << std::endl;
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
	if (setsockopt(_socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		std::cerr << "Setsockopt creation failed: " << strerror(errno) << std::endl;
		close(_socketfd);
		exit(EXIT_FAILURE);
	}
	if (bind(_socketfd, (struct sockaddr *)&_addr, sizeof(_addr))) { //size of addr struct for addrlen the last parameter
		std::cerr << "Bind creation failed: " << strerror(errno) << std::endl;
		close(_socketfd);
		exit(EXIT_FAILURE);
	}
	if (listen(_socketfd, 5) < 0) { //backlog set to 5 because it is the reasonable default number for small server
		std::cerr << "Listen creation failed: " << strerror(errno) << std::endl;
		close(_socketfd);
		exit(EXIT_FAILURE);
	}
	_pollfds.push_back((struct pollfd){.fd = _socketfd, .events = POLLIN, .revents = 0}); //add the socket to the poll fds
	_nfds++;
	//change true to the global that is false if a ctrl D or a sigaction ocured
	std::cout << "Server :IRECTION up & running" << std::endl;
	while (g_signal) {
//		std::cout << "WAITING ..." << std::endl;
		if (poll(_pollfds.data(), _nfds, -1) < 0 && !g_signal) //wait to have a action from one of the fds
			break ;
		if (!g_signal) //check if the global value of signal have changed with a if
			break ;
		for(std::vector<pollfd>::iterator it = _pollfds.begin(); it < _pollfds.end(); it++) { //find the fd that had an event by iterating the list of vector
			if (it->revents & POLLIN) {
				if (it->fd == _socketfd) {
					int addr_len = sizeof(_addr);
					int fd_cli = accept(_socketfd, (struct sockaddr *)&_addr, (socklen_t*)&addr_len);
					std::cout << "NEW CLIENT CONNECT :" << fd_cli << std::endl;
					_pollfds.push_back((struct pollfd){.fd = fd_cli, .events = POLLIN, .revents = 0});
					_cli.CreateClient(fd_cli, _addr);
					_nfds++;
					it = _pollfds.begin();
				}
				else //handle the message of the event
					messag_handle(it);
			}
		}
	}
	std::cout << "Server successfully bound to port 8080." << std::endl;
	return 0;
}

void	Server::messag_handle(std::vector<pollfd>::iterator &it) {
	int	n = 512;
	char buff[512 + 1];
	bzero(buff, 513);
	ssize_t ret = recv(it->fd, buff, n, MSG_DONTWAIT);
//	std::cout << "RECV :" << it->fd << std::endl;
	if (!ret) { // client gone suppress it
		std::cout << "SERVER: clear poll & Quit" << std::endl;
		it = _pollfds.erase(it);
		_nfds--;
	}
	else if (ret < 0) // error occured
		std::cerr << "Recv failed: " << strerror(errno) << std::endl;
	else { //message
		std::istringstream message;
		message.str(buff);
		for (std::string line; std::getline(message, line, '\n');) {
//			std::cout << "LINE :" << line << std::endl;
			_cli.CommandClient(line, it->fd); //user nick pass
			_chan.Canal_Operators(line, it->fd); //join mode kick topic invite
		}
//		std::cout << "GET :" << buff << std::endl;
	}
}

void	Server::handler(int sig) {
	(void)sig;
	g_signal = false;
	std::cout << "\rSignal: quit program." << std::endl;
}

int Server::signal_handler() {
	std::signal(SIGINT, &Server::handler);
	std::signal(SIGQUIT, &Server::handler);
	std::signal(SIGPIPE, SIG_IGN);
	return (100);
}

int	main(int ac, char **av) {
	if (ac < 3)
		return (std::cout << "Error: Missing arguments." << std::endl, 210);
	Server serv(av[1], av[2]);
	serv.CreatSocket();
}
//	if (accept(_socketfd, (struct sockaddr *)&_addr, sizeof(_addr) ) ) < 0) {
//		std::cerr << "Listen creation failed: " << strerror(errno) << std::endl;
//		close(_socketfd);
//		exit(EXIT_FAILURE);
//	}


