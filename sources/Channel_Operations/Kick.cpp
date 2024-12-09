//
// Created by grebrune on 12/10/24.
//

#include "Channel.hpp"

int Channel::Kick(std::string buff, int fd_cli) {
	size_t end_wd = buff.find(' ');//search first argument
	if (std::string::npos == end_wd)
		return (send_error(fd_cli, ERR_NEEDMOREPARAMS(buff)), 462);//if we are at the end of the word then the second word is missing
	std::string channel = buff.substr(0, end_wd);
	if (!get_rights(Get_Client_Name(fd_cli), channel, fd_cli))// check if the client caller have the rights to kick
		return (404);
	size_t comma = buff.find(':');// search if a comment is givin
	std::string comm;
	if (std::string::npos == comma) {
		comma = buff.size() - 2;
		comm = "";
	}
	else
		comm = " using " + buff.substr(comma, buff.size() - 2) + " as the reason (comment).";
	std::string nick = buff.substr(end_wd, comma);
	chan_t found = _chan_user[channel].find(nick);//search the nickname in the database of the channel
	if (_chan_user[channel].end() == found)
		return (send_error(fd_cli, ERR_USERNOTINCHANNEL(Get_Client_Name(fd_cli), nick, channel)), 441);// we didn't find him
	_chan_user.erase(nick);//we did find him
	send_error(fd_cli, RPL_SUCCESKICK(nick, channel, comment));
	return (0);
}