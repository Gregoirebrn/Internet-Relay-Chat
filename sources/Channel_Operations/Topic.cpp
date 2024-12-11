//
// Created by grebrune on 12/10/24.
//

#include "Channel.hpp"

void	Channel::send_rpl_topic(std::string channel, std::string topic, int fd_cli) {
	for (chan_t it = _chan_user[channel].begin(); it != _chan_user[channel].end() ; ++it) {

	}
	send_error(fd_cli, RPL_TOPIC(Get_Client_Name(fd_cli), channel, topic));
}

int Channel::Topic(std::string buff, int fd_cli) {
	if (buff.size() <= 2)
		return (send_error(fd_cli, ERR_NEEDMOREPARAMS(buff)), 462);//missing params
	size_t space = buff.find(' ');
	if (std::string::npos == space) {
		std::string channel = buff.substr(0, buff.size() -2);
		if (_all_chan.find(channel) == _all_chan.end())
			return (send_error(fd_cli, ERR_NOSUCHCHANNEL(channel)), false);
		if (_all_chan[channel].topic.empty())
			return (send_error(fd_cli, RPL_NOTOPIC(Get_Client_Name(fd_cli), channel)), 331);
		return (send_error(fd_cli, RPL_TOPIC(Get_Client_Name(fd_cli), channel, _all_chan[channel].topic)), 331);
	}
	std::string channel = buff.substr(0, space);
	std::string topic = buff.substr(space, buff.size() - 2);
	if (!get_rights(Get_Client_Name(fd_cli), channel, fd_cli)) //check if the client can mod the topic of the channel
		return (404);
	_all_chan[channel].topic = topic;
	if (_all_chan[channel].topic.empty())
		return (send_error(fd_cli, ERR_NEEDMOREPARAMS(buff)), 462);
	return (send_rpl_topic(channel, topic, fd_cli), 462); // send mod of topic to every client of the channel
}
