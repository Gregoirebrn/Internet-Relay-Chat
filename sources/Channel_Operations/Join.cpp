//
// Created by grebrune on 12/10/24.
//

#include "Channel.hpp"
#include <iostream>
#include <sstream>
#include <string>

int	Channel::get_join_arg(std::string buff, int fd_cli, std::vector<std::string> &channel_v, std::vector<std::string> &key_v) {
	if (buff.find(' ') == std::string::npos)
		return (send_error(fd_cli, ERR_NEEDMOREPARAMS(buff)), 462);//missing params
	std::istringstream haystack;
	haystack.str(buff);
	for (std::string channel; std::getline(haystack, channel, ',');) {
		if (channel.find(' ') != std::string::npos) {
			channel_v.push_back(channel.substr(0, channel.find(' ')));
			break;
		}
		channel_v.push_back(channel.substr(0, channel.size() - 1));
	}
	haystack.str(buff.substr(buff.find(' '), buff.size() -2));
	for (std::string key; std::getline(haystack, key, ',');) {
		if (key.find(',') != std::string::npos) {
			if (key.find(' ') == 2)
				key_v.push_back(key.substr(1, key.size() -1));
			else
				key_v.push_back(key.substr(0, key.size() -1));
		}
		else
			key_v.push_back(key.substr(0, key.size()));
	}
	return (0);
}

int	Channel::check_max_joined(int fd_cli, std::vector<std::string> channel_v) {
	size_t chan_joined = 0;
	for (iter_chan_user_t it = _chan_user.begin(); it != _chan_user.end() ; ++it) {
		if (it->second.find(Get_Client_Name(fd_cli)) != it->second.end())
			chan_joined++;
	}
	if (chan_joined >= MAX_CHAN + 1)
		return (send_error(fd_cli, ERR_TOOMANYCHANNELS(Get_Client_Name(fd_cli), channel_v.front())), 405);
	return (0);
}

int	Channel::Join(std::string buff, int fd_cli) {
	std::vector<std::string> channel_v;
	std::vector<std::string> key_v;
	if (get_join_arg(buff, fd_cli, channel_v, key_v))
		return (404);
	std::vector<std::string>::iterator key_it = key_v.begin();
	for (std::vector<std::string>::iterator it = channel_v.begin(); it != channel_v.end(); ++it) {
		if (check_max_joined(fd_cli, channel_v))
			return (404);
		if (_all_chan.find(*it) == _all_chan.end())
			send_error(fd_cli, ERR_NOSUCHCHANNEL(*it));
		else {
			if (_all_chan[*it].chan_key != *key_it)
				send_error(fd_cli, ERR_BADCHANNELKEY(Get_Client_Name(fd_cli), *it));
			else
				iter_chan_t _chan_user[*it];
		}
		++key_it;
		if (key_it == key_v.end())
	}
	//message of welcome
	return 0;
}
//	/// test
//	std::cout << std::endl << "CHANNEL" << std::endl;
//	for (std::vector<std::string>::iterator i = channel_v.begin(); i != channel_v.end(); ++i)
//		std::cout << ':' << *i << std::endl;
//	std::cout << "KEY" << std::endl;
//	for (std::vector<std::string>::iterator i = key_v.begin(); i != key_v.end(); ++i)
//		std::cout << ':' << *i << std::endl;
//	/// test
