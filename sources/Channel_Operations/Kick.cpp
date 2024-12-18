//
// Created by grebrune on 12/10/24.
//

#include "Channel.hpp"

int Channel::Kick(std::string buff, int fd_cli) {
	std::string channel = buff.substr((buff.find('#') + 1), (buff.find(' ') - 1));
	std::cout << "CHANNEL :" << channel << std::endl;
	std::string nick = buff.substr((buff.find(':') + 1), (buff.size() - 1));
	std::cout << "NICK :" << nick << std::endl;
	if (!get_rights(_client->GetName(fd_cli), channel, fd_cli))// check if the client caller have the rights to kick
		return (404);
	user_t found = _channel[channel].find(nick);//search the nickname in the database of the channel
	if (_channel[channel].end() == found)
		return (send_error(fd_cli, ERR_USERNOTINCHANNEL(_client->GetName(fd_cli), nick, channel)), 441);// we didn't find him
	_channel.erase(nick);//we did find him
	send_error(fd_cli, RPL_SUCCESKICK(nick, channel, comment));
	return (0);
}
//	std::string nick = buff.substr(end_wd, comma);
//	std::cout << "KICK_CHANNEL :" << channel << std::endl;
//		comm = " using " + buff.substr(comma, buff.size() - 2) + " as the reason (comment).";

//HEXCHAT
// << KICK irc_test #chanada :goyo
// mod to take not the first arg but the second
