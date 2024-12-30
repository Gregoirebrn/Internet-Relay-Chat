//
// Created by grebrune on 12/10/24.
//

#include "Channel.hpp"

int Channel::Kick(std::string buff, int fd_cli)
{
	std::string channel = buff.substr(buff.find('#'), buff.find(' '));
	std::string nick = buff.substr((buff.find(' ') + 1), buff.size());
	nick = nick.substr(0, (nick.size() - 1));
	if (nick.find(':')) {
		nick = nick.substr(0, nick.find(' '));
	}
	if (!get_rights(_client->GetName(fd_cli), channel, fd_cli))// check if the client caller have the rights to kick
		return (404);
	user_t found = _channel[channel].find(nick);//search the nickname in the database of the channel
	if (_channel[channel].end() == found)
		return (send_error(fd_cli, ERR_USERNOTINCHANNEL(nick, channel)), 441);// we didn't find him
	_channel[channel].erase(nick);//we did find him
	send_chan_msg(channel, RPL_KICK(_client->GetPrefix(_client->GetFd(nick)), channel, nick));
	return (0);
}