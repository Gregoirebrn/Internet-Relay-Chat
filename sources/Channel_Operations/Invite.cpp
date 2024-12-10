//
// Created by grebrune on 12/10/24.
//

#include "Channel.hpp"

int Channel::Invite(std::string buff, int fd_cli) {
	std::cout << "BUFF:" << buff << std::endl;
	size_t space = buff.find(' ');
	if (std::string::npos == space)
		return (send_error(fd_cli, ERR_NEEDMOREPARAMS(buff)), 462);//missing params
	std::string nick_target = buff.substr(0, space);
	std::string channel = buff.substr(space, buff.size());
	if (!get_rights(Get_Client_Name(fd_cli), channel, fd_cli))// check if the client caller have the rights to invite
		return (404);
	iter_chan_t found = _chan_user[channel].find(nick_target);//search the nickname in the database of the channel
	if (_chan_user[channel].end() != found)
		return (send_error(fd_cli, ERR_USERONCHANNEL(nick_target, channel)), 443);
//	send_error(fd_cli, RPL_INVITING(Get_Client_Name(fd_cli), nick_target, channel)), 341
	return (0);
}