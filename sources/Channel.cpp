//
// Created by grebrune on 12/2/24.
//

#include "Channel.hpp"

// Constructors & destructor
Channel::Channel(void) {
	std::cout << "Channel default constructor called!" << std::endl;
}

Channel::~Channel(void) {
	std::cout << "Channel default destructor called!" << std::endl;
}

// Public methods
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
	iter_chan_t found = _chan_user[channel].find(nick);//search the nickname in the database of the channel
	if (_chan_user[channel].end() == found)
		return (send_error(fd_cli, ERR_USERNOTINCHANNEL(Get_Client_Name(fd_cli), nick, channel)), 441);// we didn't find him
	_chan_user.erase(nick);//we did find him
	send_error(fd_cli, RPL_SUCCESKICK(nick, channel, comment));
	return (0);
}


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

int Channel::Topic(std::string buff, int fd_cli) {
	std::cout << "BUFF:" << buff << std::endl;
	size_t space = buff.find(' ');
	if (std::string::npos == space) {
		std::string channel = buff.substr(0, buff.size() -2);
		if (_all_chan[channel].topic.empty())
			return (send_error(fd_cli, RPL_NOTOPIC(Get_Client_Name(fd_cli), channel)), 331);
		else
			return (send_error(fd_cli, RPL_TOPIC(Get_Client_Name(fd_cli), channel, _all_chan[channel].topic)), 331);
	}
	std::string channel = buff.substr(0, space);
	std::string topic = buff.substr(space, buff.size());
	std::map<std::string, bool>::iterator it;
	for (it = _chan_user[channel].begin(); it != _chan_user[channel].end(); ++it) {
		if (it->first == Get_Client_Name(fd_cli)) {
			if (!it->second)
				return (send_error(fd_cli, ERR_CHANOPRIVSNEEDED(channel)), 403);
			else {
				_all_chan[channel].topic = topic;
				if (_all_chan[channel].topic.empty())
					return (send_error(fd_cli, ERR_NEEDMOREPARAMS(buff)), 462);
				return (send_error(fd_cli, RPL_TOPIC(Get_Client_Name(fd_cli), channel, topic)), 462);
			}
		}
	}
	if (it == _chan_user[channel].end())
		return (send_error(fd_cli, ERR_NOTONCHANNEL(channel)), 442);
	std::cout << "CHANNEL:" << channel << std::endl;
	std::cout << "TOPIC:" << topic << std::endl;
	if (_all_chan[channel].topic.empty())
		return (send_error(fd_cli, ERR_NEEDMOREPARAMS(buff)), 462);
	return (0);
}

int Channel::Mode(std::string buff, int fd_cli) {
	(void)fd_cli;
	(void)buff;
	return (0);
}

int Channel::Join(std::string buff, int fd_cli) {
	(void)fd_cli;
	std::string chan = buff.substr(0, buff.find(' '));
	std::string key = buff.substr(0 , buff.size() - 2);

	std::cout << chan << std::endl;
	std::cout << key << std::endl;
	if (_all_chan.end() == _all_chan.find(chan)) {
		_all_chan[chan].chan_key = key;
		_all_chan[chan].topic = "";
		_all_chan[chan].max_user = 5;
		std::map<std::string , bool> i;
		i[Get_Client_Name(fd_cli)] = false;
		_chan_user[chan] = i;
		return (0);
	}
	return 0;
}

int	Channel::Canal_Operators(std::string buff, int fd_cli) {
	int	(Channel::*fptr[])(std::string, int ) = {&Channel::Kick, &Channel::Invite, &Channel::Topic, &Channel::Mode, &Channel::Join};
	std::string tab_com[] = {"KICK", "INVITE", "TOPIC", "MODE", "JOIN"};

	for (int i = 0; i < 4; ++i) {
		try {
			if (!buff.compare(0, tab_com[i].size(), tab_com[i])) {
				std::string arg = buff.substr(tab_com[i].size() + 1, buff.size());
				return ((this->*fptr[i])(arg, fd_cli), 1);
			}
		}
		catch (std::exception &exe) {
			std::cout << "Channel: " << exe.what() << std::endl;
		}
	}
	return (0);
}

//utils for commands
bool Channel::get_rights(std::string name, std::string channel, int fd_cli) {
	if (_all_chan.end() == _all_chan.find(channel))
		return (send_error(fd_cli, ERR_NOSUCHCHANNEL(channel)), false);
	for (iter_chan_user_t it = _chan_user.begin(); it != _chan_user.end(); ++it) {
		if (it->first == channel) {
			iter_chan_t i = it->second.begin();
			while (i != it->second.end() && i->first != name)
				i++;
			if (i == it->second.end())
				return false;
			if (i->second)
				return true;
			return (send_error(fd_cli, ERR_CHANOPRIVSNEEDED(channel)), false);
		}
	}
	return false;
}

