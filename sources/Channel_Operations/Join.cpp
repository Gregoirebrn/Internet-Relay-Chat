//
// Created by grebrune on 12/10/24.
//

#include "Channel.hpp"

int Channel::Join(std::string buff, int fd_cli) {
	(void)fd_cli;
	std::string chan = buff.substr(0, buff.find(' '));
	std::string key = buff.substr(0 , buff.size() - 2);
	if (_all_chan.end() == _all_chan.find(chan)) {
		_all_chan[chan].chan_key = key;
		_all_chan[chan].topic = "";
		_all_chan[chan].max_user = 5;
		std::map<std::string , bool> i;
		i[Get_Client_Name(fd_cli)] = false;
		_chan_user[chan] = i;
		return (0);
	}
	return 0;
}