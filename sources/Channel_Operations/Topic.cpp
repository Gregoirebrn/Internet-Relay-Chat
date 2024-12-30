//
// Created by grebrune on 12/10/24.
//

#include "Channel.hpp"

//!!! A Ajouter la verif que le topic peut etre modifier par des non opp

int Channel::Topic(std::string buff, int fd_cli) {
	if (buff.size() <= 2)
		return (send_error(fd_cli, ERR_NEEDMOREPARAMS("TOPIC")), 462);//missing params
	size_t two_dots = buff.find(':');
	if (two_dots == std::string::npos) { //no topic is given, so checking the topic
		std::string channel = buff;
		if (std::string::npos != buff.find('\r'))
			channel = buff.substr(0, buff.size() -1);
		if (_all_chan.find(channel) == _all_chan.end())
			return (send_error(fd_cli, ERR_NOSUCHCHANNEL(channel)), false);
		if (_all_chan[channel].topic.empty())
			return (send_error(fd_cli, RPL_NOTOPIC(_client->GetName(fd_cli), channel)), 331);
		send_error(fd_cli, RPL_TOPIC(_client->GetName(fd_cli), channel, _all_chan[channel].topic));
		return (send_error(fd_cli, RPL_TOPICWHOTIME(_client->GetName(fd_cli), channel, _all_chan[channel].set_of_topic, _all_chan[channel].time)), 331);
	}
	std::string channel = buff.substr(0, buff.find(' '));
	if (_all_chan.find(channel) == _all_chan.end())
		return (send_error(fd_cli, ERR_NOSUCHCHANNEL(channel)), false);
//	std::cout << "SUBBBSSS :" << channel << std::endl;
	std::string topic = buff.substr(two_dots + 1, buff.size()); //trim the two dots
//	std::cout << "NO_CUTS :" << topic << std::endl;
	if (std::string::npos != topic.find('\r'))
		topic = topic.substr(0, topic.size() - 1); //trim the \r
//	std::cout << "CUTS :" << topic << std::endl;
	if (!get_rights(_client->GetName(fd_cli), channel, fd_cli)) //check if the client can mod the topic of the channel
		return (404);
	std::time_t timestamp = std::time(0);   // get time now
	std::stringstream ss;
	ss << timestamp;
	_all_chan[channel].time = ss.str();;   // get time now
	_all_chan[channel].set_of_topic = _client->GetName(fd_cli);   // get time now
	if (_all_chan[channel].topic.empty()) {
		_all_chan[channel].topic = topic;
		return (send_chan_msg(channel, RPL_CHANGETOPIC(_client->GetName(fd_cli), channel, topic)), 0);
	}
	_all_chan[channel].topic = topic;
	return (send_chan_msg(channel, RPL_CHANGETOPIC(_client->GetName(fd_cli), channel, topic)), 0);
}

//	std::cout << "CHAN_TOPIC :" << topic << std::endl;
//	std::cout << "CHAN_NAME  :" << channel << std::endl;
//change of topic
//%C22*%O$t%C26$1%C has changed the topic to: $2%O
//creation of topic
//%C22*%O$tTopic for %C22$1%C set by %C26$2%C (%C24$3%O)