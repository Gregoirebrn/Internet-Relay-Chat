//
// Created by grebrune on 12/10/24.
//

#include "Channel.hpp"

int Channel::Invite(const std::string& buff, int fd_cli) {
	//trim
	size_t space = buff.find(' ');
	if (std::string::npos == space) //the channel is missing
		return (SendMessage(fd_cli, ERR_NEEDMOREPARAMS("INVITE")), 462);
	std::string nick_target = buff.substr(0, space);
	std::string channel = buff.substr(space + 1, buff.size());
	if (std::string::npos != channel.find('\r'))
		channel = channel.substr(0, channel.size() - 1);
	//checks
//	if (!_client->GetFd(nick_target))
//		return (SendMessage(fd_cli, ERR_NOSUCHNICK(_client->GetName(fd_cli), nick_target)), 401);
	if (_all_chan.find(channel) == _all_chan.end())
		return (SendMessage(fd_cli, ERR_NOSUCHCHANNEL(channel)), 403);
	if (!CheckRights(_client->GetName(fd_cli), channel, fd_cli)) // check if the client caller have the rights to invite
		return (404);
	user_t found = _channel[channel].find(nick_target); //search the nickname in the database of the channel
	if (_channel[channel].end() != found)
		return (SendMessage(fd_cli, ERR_USERONCHANNEL(_client->GetName(fd_cli), nick_target, channel)), 443); //already on channel
	//if bot
	if ("bot" == nick_target) {
		if (_last_question.find(channel) != _last_question.end())
			return (SendMessage(fd_cli, ERR_USERONCHANNEL(_client->GetName(fd_cli), nick_target, channel)), 443); //already on channel
		SendMessage(fd_cli, RPL_INVITING(_client->GetPrefix(fd_cli), nick_target, channel)); //added to channel and welcome
		send_chan_msg(channel, RPL_JOIN(nick_target, channel));
		_last_question[channel] = "";
		FoundNextQuestion(channel);
		return (0);
	}
	//send msg
	SendMessage(_client->GetFd(nick_target), RPL_INVITED(_client->GetPrefix(fd_cli), nick_target, channel));
	SendMessage(fd_cli, RPL_INVITING(_client->GetPrefix(fd_cli), nick_target, channel)); //added to channel and welcome
	_all_chan[channel].invite_list.push_back(nick_target);
	return (0);
}