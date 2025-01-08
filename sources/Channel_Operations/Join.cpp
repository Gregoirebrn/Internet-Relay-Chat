//
// Created by grebrune on 12/10/24.
//

#include "Channel.hpp"
#include <iostream>
#include <sstream>
#include <string>

int	Channel::get_join_arg(std::string buff, int fd_cli, std::vector<std::string> &channel_v, std::vector<std::string> &key_v) {
	(void)fd_cli;
	if (buff.find(' ') == std::string::npos)
		key_v.push_back("");
	else {
		if (buff.find(' ') != std::string::npos) {
			std::istringstream keys;
			keys.str(buff.substr((buff.find(' ') + 1), buff.size()));
			for (std::string key; std::getline(keys, key, ',');) {
				key_v.push_back(key.substr(0, key.size()));
			}
		}
	}
	std::istringstream haystack;
	haystack.str(buff.substr(0, buff.find(' ')));
	for (std::string channel; std::getline(haystack, channel, ',');) {
		channel_v.push_back(channel.substr(0, channel.size()));
	}
//	for (vec_t it = channel_v.begin(); it != channel_v.end(); ++it) {
//		std::cout << "CHA_V :" << *it << std::endl;
//	}
//	for (vec_t it = key_v.begin(); it != key_v.end(); ++it) {
//		std::cout << "KEY_V :" << *it << std::endl;
//	}
	return (0);
}

void	Channel::send_rpl_name(std::string channel, int fd_cli) { //get all names of the channel and send them to client
	std::string all_names;
	for (user_t it = _channel[channel].begin(); it != _channel[channel].end(); ++it) {
		if (_channel[channel][it->first])
			all_names += "@" + it->first + " ";
		else
		all_names += it->first + " ";
	}
	send_error(fd_cli, RPL_NAMREPLY(_client->GetName(fd_cli), channel, all_names));
	send_error(fd_cli, RPL_ENDOFNAMES(_client->GetName(fd_cli), channel));
}

void	Channel::send_rpl_topic(std::string channel, std::string topic, int fd_cli) {
	if (topic.empty())
		return (send_error(fd_cli, RPL_NOTOPIC(_client->GetName(fd_cli), channel)));
	return (send_error(fd_cli, RPL_TOPIC(_client->GetName(fd_cli), channel, topic)));
}

void	Channel::CreateChannel(std::string channel, int fd_cli) {
	if (channel[0] != '#')
		return (send_error(fd_cli, ERR_NOSUCHCHANNEL(channel)), (void)0);
	std::string charset = "0123456789abcdefghijklmnopqrstuvwxyz_-";
	for (int i = 1; channel[i]; ++i) {
		if (std::string::npos == charset.find(channel[i]))
			return (send_error(fd_cli, ERR_BADCHANMASK(channel)), void(0));
	}
	if (std::string::npos != channel.find('\r'))
		channel = channel.substr(0, channel.size() - 1);
	_all_chan[channel] = (mod_t){.chan_key = "", .topic = "", .set_of_topic = "", .time = "", .in_user = 1 , .max_user = 0, .t_bool = true , .i_bool = false};
	_channel[channel][_client->GetName(fd_cli)] = true;
	send_error(fd_cli, RPL_JOIN(_client->GetPrefix(fd_cli), channel));
	send_error(fd_cli, RPL_NOTOPIC(_client->GetName(fd_cli), channel));
	send_rpl_name(channel, fd_cli);
}

int	Channel::Join(std::string buff, int fd_cli) {
	std::vector<std::string> channel_v;
	std::vector<std::string> key_v;

	if (buff.size() == 0)
		return (send_error(fd_cli, ERR_NEEDMOREPARAMS("JOIN")), 462);//missing params
	get_join_arg(buff, fd_cli, channel_v, key_v); // trim the buff in a vector of channels and keys

	vec_t key_it = key_v.begin();
	for (vec_t it = channel_v.begin(); it != channel_v.end(); ++it) {
		if (_all_chan.find(*it) == _all_chan.end()) {// not a channel so we create a new one
			CreateChannel(*it, fd_cli);
			continue;
		}
//		std::cout << "KEYIMEPTY:" << *key_it << "--" << std::endl;
//		std::cout << "BUTISITREALEY EMPTY:" << key_it->empty() << std::endl;
//		std::cout << "KKK:" << _all_chan[*it].chan_key.empty() << ":" << !_all_chan[*it].chan_key.empty()<< std::endl;
		if (!_all_chan[*it].chan_key.empty() && key_it->empty()) // check the value of the key
			return (send_error(fd_cli, ERR_BADCHANNELKEY(_client->GetName(fd_cli), *it)), 475);
		if (!_all_chan[*it].chan_key.empty() && _all_chan[*it].chan_key != *key_it) // check the value of the key
			return (send_error(fd_cli, ERR_BADCHANNELKEY(_client->GetName(fd_cli), *it)), 475);
		if (!_all_chan[*it].chan_key.empty() && _all_chan[*it].chan_key == *key_it)
			++key_it;
		if (_all_chan[*it].max_user != 0 && _all_chan[*it].max_user < _all_chan[*it].in_user + 1) // limit user in
			return (send_error(fd_cli, ERR_CHANNELISFULL(_client->GetName(fd_cli), *it)), 471);
		if (_all_chan[*it].i_bool ) {// is channel is in invite mod
			if (_all_chan[*it].invite_list.end() == std::find(_all_chan[*it].invite_list.begin(), _all_chan[*it].invite_list.end(),_client->GetName(fd_cli)))
				return (send_error(fd_cli, ERR_INVITEONLYCHAN(_client->GetName(fd_cli), *it)), 473);
			std::remove(_all_chan[*it].invite_list.begin(), _all_chan[*it].invite_list.end(), _client->GetName(fd_cli));
		}
		_channel[*it][_client->GetName(fd_cli)] = false;
		_all_chan[*it].in_user++;
		send_chan_msg(*it, RPL_JOIN(_client->GetPrefix(fd_cli), *it));
		send_rpl_topic(*it, _all_chan[*it].topic, fd_cli); // ! the topic needs to be set accordingly !
		send_rpl_name(*it, fd_cli);
	}
	return 0;
}

//	std::cout << "PRINT BOOL I :" << _all_chan["#chanada"].i_bool << std::endl;