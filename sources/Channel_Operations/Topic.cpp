//
// Created by grebrune on 12/10/24.
//

#include "Channel.hpp"

int Channel::Topic(std::string buff, int fd_cli) {
	if (buff.size() <= 2)
		return (send_error(fd_cli, ERR_NEEDMOREPARAMS("TOPIC")), 462);//missing params
	size_t two_dots = buff.find(':');
	if (two_dots == std::string::npos) { //no topic is given, so checking the topic
		std::string channel = buff.substr(0, buff.size() -1);
		if (channel[0] != '#')
			return (send_error(fd_cli, ERR_NOSUCHCHANNEL(channel)), false);
		channel = channel.substr(1, channel.size());
		if (_all_chan.find(channel) == _all_chan.end())
			return (send_error(fd_cli, ERR_NOSUCHCHANNEL(channel)), false);
		if (_all_chan[channel].topic.empty())
			return (send_error(fd_cli, RPL_NOTOPIC(GetName(fd_cli), channel)), 331);
		return (send_error(fd_cli, RPL_TOPIC(GetName(fd_cli), channel, _all_chan[channel].topic)), 331);
	}
	std::string channel = buff.substr(0, buff.find(' '));
	if (channel[0] != '#')
		return (send_error(fd_cli, ERR_NOSUCHCHANNEL(channel)), false);
	channel = channel.substr(1, channel.size());
//	std::cout << "SUBBBSSS :" << channel << std::endl;
	std::string topic = buff.substr(two_dots + 1, buff.size()); //trim the two dots
//	std::cout << "NO_CUTS :" << topic << std::endl;
	topic = topic.substr(0, topic.size() - 1); //trim the \r
//	std::cout << "CUTS :" << topic << std::endl;
	if (!get_rights(_client->GetName(fd_cli), channel, fd_cli)) //check if the client can mod the topic of the channel
		return (404);
	_all_chan[channel].topic = topic;
//	std::cout << "CHAN_TOPIC :" << topic << std::endl;
//	std::cout << "CHAN_NAME  :" << channel << std::endl;
	return (send_error(fd_cli, RPL_TOPIC(_client->GetName(fd_cli), channel, topic)), 332); // send mod of topic to every client of the channel
}
