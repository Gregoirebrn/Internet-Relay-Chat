//
// Created by grebrune on 12/10/24.
//
#include "Channel.hpp"

void	Channel::JoinArgs(const std::string& buff, std::vector<std::string> &channel_v, std::vector<std::string> &key_v) {
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
}

void	Channel::send_rpl_name(const std::string& channel, int fd_cli) { //get all names of the channel and send them to client
	std::string all_names;
	for (user_t it = _channel[channel].begin(); it != _channel[channel].end(); ++it) {
		if (_channel[channel][it->first])
			all_names += "@" + it->first + " ";
		else
		all_names += it->first + " ";
	}
	SendMessage(fd_cli, RPL_NAMREPLY(_client->GetName(fd_cli), channel, all_names));
	SendMessage(fd_cli, RPL_ENDOFNAMES(_client->GetName(fd_cli), channel));
}

void	Channel::send_rpl_topic(const std::string& channel, const std::string& topic, int fd_cli) {
	if (topic.empty())
		return (SendMessage(fd_cli, RPL_NOTOPIC(_client->GetName(fd_cli), channel)));
	return (SendMessage(fd_cli, RPL_TOPIC(_client->GetName(fd_cli), channel, topic)));
}

void	Channel::CreateChannel(std::string channel, int fd_cli) {
	if (channel[0] != '#')
		return (SendMessage(fd_cli, ERR_NOSUCHCHANNEL(channel)), (void)0);
	std::string charset = "0123456789abcdefghijklmnopqrstuvwxyz_-";
	for (int i = 1; channel[i]; ++i) {
		if (std::string::npos == charset.find(channel[i]))
			return (SendMessage(fd_cli, ERR_BADCHANMASK(channel)), void(0));
	}
	if (std::string::npos != channel.find('\r'))
		channel = channel.substr(0, channel.size() - 1);
	std::vector<std::string> init;
	_all_chan[channel] = (mod_t){.chan_key = "", .topic = "", .set_of_topic = "", \
	.time = "", .in_user = 1 , .max_user = 0, .t_bool = true , .i_bool = false, .invite_list = init};
	_channel[channel][_client->GetName(fd_cli)] = true;
	SendMessage(fd_cli, RPL_JOIN(_client->GetPrefix(fd_cli), channel));
	SendMessage(fd_cli, RPL_NOTOPIC(_client->GetName(fd_cli), channel));
	send_rpl_name(channel, fd_cli);
}

int Channel::CheckModeChannel(vec_t key_it, vec_t chan_it, int fd_cli) {
	if (!_all_chan[*chan_it].chan_key.empty() && key_it->empty()) // check the value of the key
		return (SendMessage(fd_cli, ERR_BADCHANNELKEY(_client->GetName(fd_cli), *chan_it)), 475);
	if (!_all_chan[*chan_it].chan_key.empty() && _all_chan[*chan_it].chan_key != *key_it) // check the value of the key
		return (SendMessage(fd_cli, ERR_BADCHANNELKEY(_client->GetName(fd_cli), *chan_it)), 475);
	if (!_all_chan[*chan_it].chan_key.empty() && _all_chan[*chan_it].chan_key == *key_it)
		++key_it;
	if (_all_chan[*chan_it].max_user != 0 && _all_chan[*chan_it].max_user < _all_chan[*chan_it].in_user + 1) // limit user in
		return (SendMessage(fd_cli, ERR_CHANNELISFULL(_client->GetName(fd_cli), *chan_it)), 471);
	if (_all_chan[*chan_it].i_bool) {// is channel is in invite mod
		if (_all_chan[*chan_it].invite_list.end() ==
			std::find(_all_chan[*chan_it].invite_list.begin(), _all_chan[*chan_it].invite_list.end(), _client->GetName(fd_cli)))
			return (SendMessage(fd_cli, ERR_INVITEONLYCHAN(_client->GetName(fd_cli), *chan_it)), 473);
		if (_all_chan[*chan_it].invite_list.end() != std::remove(_all_chan[*chan_it].invite_list.begin(), _all_chan[*chan_it].invite_list.end(), _client->GetName(fd_cli))) {
			std::cout << "Server: Error: Crash from std::remove, cleared invite list." << std::endl;
			_all_chan[*chan_it].invite_list.clear();
		}
	}
	return (0);
}

void	Channel::LeaveAllChannel(int fd_cli) {
	std::string nick = _client->GetName(fd_cli);
	for (chan_t it = _channel.begin(); it != _channel.end(); ++it) {
		if (_channel[it->first].find(nick) != _channel[it->first].end()) {
			SendChannel(it->first, RPL_PART(_client->GetPrefix(fd_cli), it->first, ":Disconnected"));
			_channel[it->first].erase(nick);
		}
	}
}

int	Channel::Join(const std::string& buff, int fd_cli) {
	std::vector<std::string> channel_v;
	std::vector<std::string> key_v;

	if (buff.find('0') != std::string::npos)
		return (LeaveAllChannel(fd_cli), 404);
	if (buff.size() == 0)
		return (SendMessage(fd_cli, ERR_NEEDMOREPARAMS("JOIN")), 462);//missing params
	JoinArgs(buff, channel_v, key_v); // trim the buff in a vector of channels and keys
	vec_t key_it = key_v.begin();
	for (vec_t it = channel_v.begin(); it != channel_v.end(); ++it) {
		if (_all_chan.find(*it) == _all_chan.end()) {// not a channel so we create a new one
			CreateChannel(*it, fd_cli);
			continue;
		}
		//Check All modes of the channel
		if (CheckModeChannel(key_it, it, fd_cli))
			return (42);
		//Sends Information of the joined channel
		_channel[*it][_client->GetName(fd_cli)] = false;
		_all_chan[*it].in_user++;
		SendChannel(*it, RPL_JOIN(_client->GetPrefix(fd_cli), *it));
		send_rpl_topic(*it, _all_chan[*it].topic, fd_cli);
		send_rpl_name(*it, fd_cli);
	}
	return 0;
}