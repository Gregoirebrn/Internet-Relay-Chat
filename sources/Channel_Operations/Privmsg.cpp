//
// Created by grebrune on 12/13/24.
//

#include "Channel.hpp"

void	Channel::send_chan_msg(std::string channel, std::string msg) {
	for (user_t it = _channel[channel].begin(); it != _channel[channel].end(); ++it) {
//		std::cout << "SEND_CHAN_MSG TO :" << _client->GetFd(it->first) << std::endl;
		send_error(_client->GetFd(it->first), msg);
	}
}

void	Channel::send_msg_to_chan(std::string channel, std::string msg, int to_not_send) {
	for (user_t it = _channel[channel].begin(); it != _channel[channel].end(); ++it) {
//		std::cout << "SEND_CHAN_MSG TO :" << _client->GetFd(it->first) << std::endl;
		if (to_not_send != _client->GetFd(it->first))
			send_error(_client->GetFd(it->first), msg);
	}
}

int	Channel::Privmsg(std::string buff, int fd_cli) {
	std::string channel = buff.substr(0 , buff.find(' '));
	if (channel[0] == '#' || channel[0] == '&') {
		if (_channel.find(channel) == _channel.end())
			send_error(fd_cli, ERR_NOSUCHCHANNEL(channel));
		if (std::string::npos == buff.find(':'))
			return (send_error(fd_cli, ERR_NODOTMSG), 404);
		std::string msg = buff.substr(buff.find(':'), buff.size());
		if (std::string::npos != msg.find('\r'))
			msg = msg.substr(0, (msg.size() -1));
		msg = ":" + _client->GetPrefix(fd_cli) + " PRIVMSG " + channel + " " + msg + "\r\n";
//		std::cout << "MSG1 :" << msg << std::endl;
		send_msg_to_chan(channel, msg, fd_cli);
		return (1);
	}
	if (buff.find(' ') == std::string::npos)
		return (send_error(fd_cli, ERR_NEEDMOREPARAMS("PRIVMSG")), 461);
	buff = buff.substr(0 , buff.size() - 1);
	std::cout << "TRIM BUFF :" << buff << std::endl;
	std::vector<std::string> targets;
	std::istringstream haystack;
	haystack.str(buff);
	for (std::string line; std::getline(haystack, line, ',');) {
		if (line.find(' ') != std::string::npos) {
			targets.push_back(line.substr(0, line.find(' ')));
			break;
		}
		targets.push_back(line);
	}
	for (vec_t it = targets.begin(); it != targets.end() ; ++it) {
		std::cout << "TARGETS :" << *it << std::endl;
	}
	std::string msg = buff.substr((buff.find(targets.back()) + targets.back().size() + 1), buff.size()) + "\r\n";
	std::cout << "MSG :" << msg << std::endl;
	for (vec_t it = targets.begin(); it != targets.end() ; ++it) {
		if (it->find('#') != std::string::npos) { // its a channel send to all user in it
			std::string channel = it->substr(1, it->size());
			std::cout << "CHANNEL :" << channel << std::endl;
			if (_channel.find(channel) == _channel.end())
				send_error(fd_cli, ERR_NOSUCHCHANNEL(channel));
			else
				send_chan_msg(channel, msg);
		}
	}
	return (0);
}