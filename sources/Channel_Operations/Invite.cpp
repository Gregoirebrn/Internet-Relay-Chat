//
// Created by grebrune on 12/10/24.
//

#include "Channel.hpp"

int Channel::Invite(std::string buff, int fd_cli) {
	size_t space = buff.find(' ');
	if (std::string::npos == space) //the channel is missing
		return (send_error(fd_cli, ERR_NEEDMOREPARAMS("INVITE")), 462);
	std::string nick_target = buff.substr(0, space);
	std::string channel = buff.substr(space + 1, buff.size());
	channel = channel.substr(0, channel.size() - 1);
//	std::cout << "IVNTE CHANNEL :" << channel << std::endl;
	if (channel[0] != '#')
		return (send_error(fd_cli, ERR_NOSUCHCHANNEL(channel)), 403);
	channel = channel.substr(1, channel.size());
//	std::cout << "IVNTE CHANNEL 2 :" << channel << std::endl;
	if (!get_rights(_client->GetName(fd_cli), channel, fd_cli)) // check if the client caller have the rights to invite
		return (404);
	if (_client->GetFd(nick_target) == -1)
		return (send_error(fd_cli, ERR_NOSUCHNICK(_client->GetName(fd_cli),nick_target)), 401);
	user_t found = _channel[channel].find(nick_target); //search the nickname in the database of the channel
	if (_channel[channel].end() != found)
		return (send_error(fd_cli, ERR_USERONCHANNEL(nick_target, channel)), 443); //already on channel
	send_error(_client->GetFd(nick_target), RPL_INVITING(_client->GetName(fd_cli), nick_target, channel));
	return (send_error(fd_cli, RPL_INVITING(_client->GetName(fd_cli), nick_target, channel)), 341); //added to channel and welcome
}