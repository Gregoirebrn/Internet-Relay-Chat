//
// Created by grebrune on 12/10/24.
//

#include "Channel.hpp"

int Channel::Kick(std::string buff, int fd_cli) {
	std::string channel = buff.substr(buff.find('#'), buff.find(' '));
	std::cout << "CHANNEL :" << channel << std::endl;
	std::string nick = buff.substr((buff.find(':') + 1), buff.size());
	std::cout << "NICK :" << nick << std::endl;
	nick = nick.substr(0, (nick.size() - 1));
	std::cout << "NICK 1.5:" << nick << std::endl;
	if (nick.find(':')) {
		nick = nick.substr(0, nick.find(' '));
		std::cout << "NICK 2:" << nick << std::endl;
	}
	if (!get_rights(_client->GetName(fd_cli), channel, fd_cli))// check if the client caller have the rights to kick
		return (404);
	for (user_t it =_channel[channel].begin(); it !=_channel[channel].end(); ++it) {
		std::cout << "USERS --" << it->first << std::endl;
	}
	user_t found = _channel[channel].find(nick);//search the nickname in the database of the channel
	if (_channel[channel].end() == found)
		return (send_error(fd_cli, ERR_USERNOTINCHANNEL(nick, channel)), 441);// we didn't find him
	_channel[channel].erase(nick);//we did find him
	send_error(_client->GetFd(nick), RPL_SUCCESKICK(_client->GetPrefix(fd_cli) , nick, channel);
	send_error(fd_cli, RPL_SUCCESKICK(_client->GetPrefix(fd_cli) , nick, channel);
	return (0);
}
//	std::string nick = buff.substr(end_wd, comma);
//	std::cout << "KICK_CHANNEL :" << channel << std::endl;
//		comm = " using " + buff.substr(comma, buff.size() - 2) + " as the reason (comment).";

//HEXCHAT
// << KICK irc_test #chanada :goyo
// mod to take not the first arg but the second
