//
// Created by grebrune on 11/20/24.
//

#include "Server.hpp"

// Constructors & destructor
Server::Server(char *port, const std::string &pw) : _chan(&_cli), _cli(pw), _pollfds(), _addr(), _socketfd() {
	_port = static_cast <uint16_t>(std::strtod(port, NULL));
	_password = pw;
	_nfds = 0;
	//Init sock_addr struct for bind
	_addr.sin_family = AF_INET; // IPv4
	_addr.sin_port = htons(_port); // Port number, convert host to network byte order
	_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all available interfaces (0.0.0.0)
	SignalHandler();
}

Server::~Server() {
	close(_socketfd);
}

// Public methods
int Server::CreatSocket()
{
	//Communication domain in IPv4 or IPv6 protocol, for IPv4 it's AF_INET
	//type of communication in TCP so SOCK_STREAM
	//valeur de champs generalement 0 ou 3
	int opt = 1;
	_socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketfd < 0) {
		std::cerr << "Server: Error: Server creation failed: " << strerror(errno) << std::endl;
		return (1);
	}
	if (setsockopt(_socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		std::cerr << "Server: Error: Setsockopt creation failed: " << strerror(errno) << std::endl;
		close(_socketfd);
		return (1);
	}
	if (bind(_socketfd, (struct sockaddr *)&_addr, sizeof(_addr))) { //size of addr struct for addrlen the last parameter
		std::cerr << "Server: Error: Bind creation failed: " << strerror(errno) << std::endl;
		close(_socketfd);
		return (1);
	}
	if (listen(_socketfd, 5) < 0) { //backlog set to 5 because it is the reasonable default number for small server
		std::cerr << "Server: Error: Listen creation failed: " << strerror(errno) << std::endl;
		close(_socketfd);
		return (1);
	}
	_pollfds.push_back((struct pollfd){.fd = _socketfd, .events = POLLIN, .revents = 0}); //add the socket to the poll fds
	_nfds++;
	return 0;
}

void	Server::MessageHandler(std::vector<pollfd>::iterator &it) {
	int	n = 512;
	char buff[512 + 1];
	bzero(buff, 513);
	ssize_t ret = recv(it->fd, buff, n, MSG_DONTWAIT);
	if (!ret) { // client gone suppress it
		_pollfds.erase(it);
		_nfds--;
		_chan.Quit("", it->fd);
		_cli.Remove(it->fd);
	}
	else if (ret < 0) // error occured
		std::cerr << "Server: Error: Recv failed: " << strerror(errno) << std::endl;
	else { //message
		std::istringstream message;
		message.str(buff);
		for (std::string line; std::getline(message, line, '\n');) {
			_cli.CommandClient(line, it->fd); //user nick pass
			if (_chan.Canal_Operators(line, it->fd)) { //join mode kick topic invite
				_pollfds.erase(it);
				_nfds--;
			}
		}
	}
}

void	Server::Handler(int sig) {
	(void)sig;
	std::cerr << "\rServer: Irection down." << std::endl;
	g_signal = false;
}

int Server::SignalHandler() {
	std::signal(SIGINT, &Server::Handler);
	std::signal(SIGQUIT, &Server::Handler);
	std::signal(SIGPIPE, SIG_IGN);
	return (100);
}

void Server::LoopServer()
{
	//change true to the global that is false if a ctrl D or a sigaction ocured
	std::cout << "Server: Irection up." << std::endl;
	while (g_signal) {
		if (poll(_pollfds.data(), _nfds, -1) < 0 && !g_signal) //wait to have a action from one of the fds
			break ;
		if (!g_signal) //check if the global value of signal have changed with a if
			break ;
		for(std::vector<pollfd>::iterator it = _pollfds.begin(); it < _pollfds.end(); it++) { //find the fd that had an event by iterating the list of vector
			if (it->revents & POLLIN && it->fd == _socketfd) {
				int addr_len = sizeof(_addr);
				int fd_cli = accept(_socketfd, (struct sockaddr *)&_addr, (socklen_t*)&addr_len);
				_pollfds.push_back((struct pollfd){.fd = fd_cli, .events = POLLIN, .revents = 0});
				_cli.CreateClient(fd_cli, _addr);
				_nfds++;
				it = _pollfds.begin();
			} else if (it->revents & POLLIN) //handle the message of the event
				MessageHandler(it);
		}
	}
}

int	main(int ac, char **av) {
	if (ac < 3)
		return (std::cout << "Server: Error: Missing arguments." << std::endl, 1);
	if (atoi(av[1]) < 1024 || atoi(av[1]) > 65535)
		return (std::cout << "Server: Error: Bad number of port, range from 1024 to 65535." << std::endl, 2);
	Server serv(av[1], av[2]);
	serv.CreatSocket();
	serv.LoopServer();
	return (0);
}
