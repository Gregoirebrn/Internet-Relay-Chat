//
// Created by grebrune on 12/10/24.
//

#include "Channel.hpp"

int Channel::Invite(std::string buff, int fd_cli) {
	//trim
	size_t space = buff.find(' ');
	if (std::string::npos == space) //the channel is missing
		return (send_error(fd_cli, ERR_NEEDMOREPARAMS("INVITE")), 462);
	std::string nick_target = buff.substr(0, space);
	std::string channel = buff.substr(space + 1, buff.size());
	if (std::string::npos != channel.find('\r'))
		channel = channel.substr(0, channel.size() - 1);
	//checks
	if (_all_chan.find(channel) == _all_chan.end())
		return (send_error(fd_cli, ERR_NOSUCHCHANNEL(channel)), 403);
	if (!get_rights(_client->GetName(fd_cli), channel, fd_cli)) // check if the client caller have the rights to invite
		return (404);
	if (_client->GetFd(nick_target) == -1)
		return (send_error(fd_cli, ERR_NOSUCHNICK(_client->GetName(fd_cli),nick_target)), 401);
	//valid
	user_t found = _channel[channel].find(nick_target); //search the nickname in the database of the channel
	if (_channel[channel].end() != found)
		return (send_error(fd_cli, ERR_USERONCHANNEL(nick_target, channel)), 443); //already on channel
	_channel[channel][nick_target] = false;
	//send msg
	send_error(_client->GetFd(nick_target), RPL_INVITED(_client->GetName(fd_cli), _client->GetUser(_client->GetFd(nick_target)), channel));
	send_rpl_name(channel, _client->GetFd(nick_target));
	send_error(fd_cli, RPL_INVITING(_client->GetName(fd_cli), nick_target, channel)); //added to channel and welcome
	send_chan_msg(channel, RPL_JOIN(_client->GetUser(_client->GetFd(nick_target)), channel));
	return (0);
}