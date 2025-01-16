//
// Created by grebrune on 12/12/24.
//

#include "Channel.hpp"

int	Channel::Quit(const std::string& buff, int fd_cli) {
	(void)buff;
	std::string nick = _client->GetName(fd_cli);
	for (chan_t it = _channel.begin(); it != _channel.end() ; ++it) {
		if (it->first.find(nick)) {
			SendChannel(it->first, RPL_PART(_client->GetPrefix(fd_cli), it->first, ":Disconnected"));
			std::string str = it->first;
			_all_chan[it->first].in_user--;
			_channel[it->first].erase(nick);
			if (_all_chan[str].in_user == 0) {
				_channel.erase(str);
				_all_chan.erase(str);
				if (_channel.empty())
					break;
				it = _channel.begin();
			}
		}
	}
	_client->Remove(fd_cli);
	return (0);
}
