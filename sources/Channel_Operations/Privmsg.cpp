//
// Created by grebrune on 12/13/24.
//

#include "Channel.hpp"

void	Channel::send_chan_msg(std::string channel, std::string msg) {
	for (user_t it = _channel[channel].begin(); it != _channel[channel].end(); ++it) {
		send_error(GetFd(it->first), msg);
	}
}

int	Channel::Privmsg(std::string buff, int fd_cli) {
	if (buff.find(' ') == std::string::npos)
		return (send_error(fd_cli, ERR_NEEDMOREPARAMS(buff)), 461);
	buff = buff.substr(0 , buff.size() - 1);
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
//	for (vec_t it = targets.begin(); it != targets.end() ; ++it) {
//		std::cout << "TARGETS :" << *it << std::endl;
//	}
	std::string msg = buff.substr((buff.find(targets.back()) + targets.back().size() + 1), buff.size()) + "\r\n";
	for (vec_t it = targets.begin(); it != targets.end() ; ++it) {
		if (it->find('#') != std::string::npos) { // its a channel send to all user in it
			std::string channel = it->substr(1, it->size());
			if (_channel.find(channel) == _channel.end())
				send_error(fd_cli, ERR_NOSUCHCHANNEL(channel));
			else
				send_chan_msg(channel, msg);
		}
	}
	return (0);
}