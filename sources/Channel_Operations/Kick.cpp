//
// Created by grebrune on 12/10/24.
//

#include "Channel.hpp"

int Channel::Kick(std::string buff, int fd_cli)
{
	if (std::string::npos == buff.find(' '))
		return (send_error(fd_cli, ERR_NEEDMOREPARAMS("KICK")), 461);
	if (std::string::npos == buff.find('#'))
		return (send_error(fd_cli, ERR_NOSUCHCHANNEL(buff.substr(0, buff.find(' ')))), false);
	std::string channel = buff.substr(buff.find('#'), buff.find(' '));
	std::string nick = buff.substr((buff.find(' ') + 1), buff.size());
	if (std::string::npos != nick.find('\n'))
		nick = nick.substr(0, (nick.size() - 1));
	if (nick.find(':'))
		nick = nick.substr(0, nick.find(' '));
	if (!get_rights(_client->GetName(fd_cli), channel, fd_cli))// check if the client caller have the rights to kick
		return (404);
	if (nick == "bot") {
		_last_question.erase(channel);
		send_chan_msg(channel, RPL_KICKED(_client->GetName(fd_cli), channel, nick));
		return (90);
	}
	user_t found = _channel[channel].find(nick);//search the nickname in the database of the channel
	if (_channel[channel].end() == found)
		return (send_error(fd_cli, ERR_USERNOTINCHANNEL(_client->GetName(fd_cli), channel, nick)), 441);// we didn't find him
	send_chan_msg(channel, RPL_KICKED(_client->GetName(fd_cli), channel, nick));
	_channel[channel].erase(nick);//we did find him
	_all_chan[channel].in_user--;
	if (_all_chan[channel].in_user == 0) {
		_channel.erase(channel);
		_all_chan.erase(channel);
	}
//	send_error(_client->GetFd(nick), RPL_KICKED(_client->GetName(fd_cli), channel, nick));
	return (0);
}