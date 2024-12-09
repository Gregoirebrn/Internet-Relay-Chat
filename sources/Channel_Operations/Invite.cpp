//
// Created by grebrune on 12/10/24.
//

#include "Channel.hpp"

int Channel::Invite(std::string buff, int fd_cli) {
	size_t space = buff.find(' ');
	if (std::string::npos == space) //the channel is missing
		return (send_error(fd_cli, ERR_NEEDMOREPARAMS(buff)), 462);
	std::string nick_target = buff.substr(0, space);
	std::string channel = buff.substr(space, buff.size() - 2);
	if (!get_rights(Get_Client_Name(fd_cli), channel, fd_cli)) // check if the client caller have the rights to invite
		return (404);
	chan_t found = _chan_user[channel].find(nick_target); //search the nickname in the database of the channel
	if (_chan_user[channel].end() != found)
		return (send_error(fd_cli, ERR_USERONCHANNEL(nick_target, channel)), 443); //already on channel
	return (send_error(fd_cli, RPL_INVITING(Get_Client_Name(fd_cli), nick_target, channel)), 341); //added to channel and welcome
}