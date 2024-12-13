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
	std::vector<std::string> targets;
	std::istringstream haystack;
	haystack.str(buff);
	for (std::string line; std::getline(haystack, line, ',');) {
		targets.push_back(line);
	}
	for (vec_t it = targets.begin(); it != targets.end() ; ++it) {
		std::cout << "TARGETS :" << *it << std::endl;
	}
	std::string msg = buff.substr((buff.find(targets.back()) + targets.back().size()), buff.size() - 1);
	for (vec_t it = targets.begin(); it != targets.end() ; ++it) {
		if (it->find('#') == std::string::npos) { //its for one user check the nick exist & get the fd
			int fd_msg = GetFd(*it);
			if (0 > fd_msg)
				return (send_error(fd_cli, ERR_NOSUCHNICK(GetName(fd_cli), *it)), 401);
			send_error(fd_msg, msg);
		} else { // its a channel send to all user in it
			std::string channel = it->substr(1, it->size());
			send_chan_msg(channel, msg);
		}
	}
}