//
// Created by grebrune on 12/10/24.
//

#include "Channel.hpp"

bool Channel::Top_Right(const std::string& name, const std::string& channel, int fd_cli) {
	if (_all_chan.end() == _all_chan.find(channel))
		return (SendMessage(fd_cli, ERR_NOSUCHCHANNEL(channel)), true);
	for (chan_t it = _channel.begin(); it != _channel.end(); ++it) {
		if (it->first == channel) {
			user_t i = it->second.begin();
			while (i != it->second.end() && i->first != name)
				i++;
			if (i == it->second.end())
				return (SendMessage(fd_cli, ERR_NOTONCHANNEL(channel)), true);
		}
	}
	if (!_all_chan[channel].t_bool && !_channel[channel][name])
		return (SendMessage(fd_cli, ERR_CHANOPRIVSNEEDED(_client->GetName(fd_cli), channel)), true);
	return false;
}

int Channel::Topic(const std::string& buff, int fd_cli) {
	if (buff.size() <= 2)
		return (SendMessage(fd_cli, ERR_NEEDMOREPARAMS("TOPIC")), 462);//missing params
	size_t two_dots = buff.find(':');
	if (two_dots == std::string::npos) { //no topic is given, so checking the topic
		std::string channel = buff;
		if (std::string::npos != buff.find('\r'))
			channel = buff.substr(0, buff.size() -1);
		if (_all_chan.find(channel) == _all_chan.end())
			return (SendMessage(fd_cli, ERR_NOSUCHCHANNEL(channel)), false);
		if (_all_chan[channel].topic.empty())
			return (SendMessage(fd_cli, RPL_NOTOPIC(_client->GetName(fd_cli), channel)), 331);
		SendMessage(fd_cli, RPL_TOPIC(_client->GetName(fd_cli), channel, _all_chan[channel].topic));
		return (SendMessage(fd_cli, RPL_TOPICWHOTIME(_client->GetName(fd_cli), channel, _all_chan[channel].set_of_topic,
													 _all_chan[channel].time)), 331);
	}
	std::string channel = buff.substr(0, buff.find(' '));
	if (_all_chan.find(channel) == _all_chan.end())
		return (SendMessage(fd_cli, ERR_NOSUCHCHANNEL(channel)), false);
	std::string topic = buff.substr(two_dots + 1, buff.size()); //trim the two dots
	if (std::string::npos != topic.find('\r'))
		topic = topic.substr(0, topic.size() - 1); //trim the \r
	if (Top_Right(_client->GetName(fd_cli), channel, fd_cli)) //check if the client can mod the topic of the channel
		return (404);
	std::time_t timestamp = std::time(0); // get time now
	std::stringstream ss;
	ss << timestamp;
	_all_chan[channel].time = ss.str(); //time stamp of the modification of the topic, so we can print RPL_TOPICWHOTIME
	_all_chan[channel].set_of_topic = _client->GetName(fd_cli);
	if (_all_chan[channel].topic.empty()) {
		_all_chan[channel].topic = topic;
		return (SendChannel(channel, RPL_CHANGETOPIC(_client->GetName(fd_cli), channel, topic)), 0);
	}
	_all_chan[channel].topic = topic;
	return (SendChannel(channel, RPL_CHANGETOPIC(_client->GetName(fd_cli), channel, topic)), 0);
}