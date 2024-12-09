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

int	Channel::check_max_joined(int fd_cli, std::vector<std::string> channel_v) { // check with the macro MAX_CHAN if he would be in too many channel
	size_t chan_joined = 0;
	for (chan_user_t it = _chan_user.begin(); it != _chan_user.end() ; ++it) {
		if (it->second.find(Get_Client_Name(fd_cli)) != it->second.end())
			chan_joined++;
	}
	if (chan_joined >= MAX_CHAN + 1)
		return (send_error(fd_cli, ERR_TOOMANYCHANNELS(Get_Client_Name(fd_cli), channel_v.front())), 405);
	return (0);
}

void	Channel::send_rpl_name(std::string channel, std::string topic, int fd_cli) { //get all names of the channel and send them to client
	std::string all_names;
	for (chan_t it = _chan_user[channel].begin(); it != _chan_user[channel].end(); ++it) {
		all_names += it->first + " ";
	}
	send_error(fd_cli, RPL_NAMREPLY(Get_Client_Name(fd_cli), channel, all_names));
}

int	Channel::Join(std::string buff, int fd_cli) {
	std::vector<std::string> channel_v;
	std::vector<std::string> key_v;
	if (get_join_arg(buff, fd_cli, channel_v, key_v)) // trim the buff in a vector of channels and keys
		return (404);
	std::vector<std::string>::iterator key_it = key_v.begin();
	for (std::vector<std::string>::iterator it = channel_v.begin(); it != channel_v.end(); ++it) {
		if (check_max_joined(fd_cli, channel_v))
			return (404);
		if (key_it == key_v.end()) // if their is no keys to the channel
			send_error(fd_cli, ERR_BADCHANNELKEY(Get_Client_Name(fd_cli), *it));
		if (_all_chan.find(*it) == _all_chan.end()) // not a channel
			send_error(fd_cli, ERR_NOSUCHCHANNEL(*it));
		else {
			if (_all_chan[*it].chan_key != *key_it) // check the value of the key
				send_error(fd_cli, ERR_BADCHANNELKEY(Get_Client_Name(fd_cli), *it));
			else { // add the client to the channel
				_chan_user[*it][Get_Client_Name(fd_cli)] = false;
				send_error(fd_cli, RPL_TOPIC(Get_Client_Name(fd_cli), *it, _all_chan[*it].topic)); // ! the topic needs to be set accordingly !
				send_rpl_name(*it, _all_chan[*it].topic, fd_cli);
				send_error(fd_cli, RPL_ENDOFNAMES(Get_Client_Name(fd_cli), *it));
			}
		}
		++key_it;
	}
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
