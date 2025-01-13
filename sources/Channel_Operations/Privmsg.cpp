//
// Created by grebrune on 12/13/24.
//

#include "Channel.hpp"

void	Channel::SendChannel(const std::string& channel, const std::string& msg) {
	for (user_t it = _channel[channel].begin(); it != _channel[channel].end(); ++it) {
//		std::cout << "SEND_CHAN_MSG TO :" << _client->GetFd(it->first) << std::endl;
		if ("bot" == it->first)
			continue;
		SendMessage(_client->GetFd(it->first), msg);
	}
}

void	Channel::send_msg_to_chan(const std::string& channel, const std::string& msg, int to_not_send) {
	for (user_t it = _channel[channel].begin(); it != _channel[channel].end(); ++it) {
//		std::cout << "SEND_CHAN_MSG TO :" << _client->GetFd(it->first) << std::endl;
		if ("bot" == it->first)
			continue;
		if (to_not_send != _client->GetFd(it->first))
			SendMessage(_client->GetFd(it->first), msg);
	}
}

int	Channel::Privmsg(const std::string& buff, int fd_cli) {
	//trim message
	if (std::string::npos == buff.find(':'))
		return (SendMessage(fd_cli, ERR_NOTEXTTOSEND(_client->GetName(fd_cli))), 412);
	std::string msg = buff.substr(buff.find(':'), buff.size());
	if (std::string::npos != msg.find('\r'))
		msg = msg.substr(0, (msg.size() -1));
	//send to channel
	std::string receiver = buff.substr(0 , buff.find(' '));
	if (receiver[0] == '#') {
		if (_channel.find(receiver) == _channel.end())
			SendMessage(fd_cli, ERR_NOSUCHCHANNEL(receiver));
		std::string base_msg = msg;
		msg = ":" + _client->GetPrefix(fd_cli) + " PRIVMSG " + receiver + " " + msg + "\r\n";
		send_msg_to_chan(receiver, msg, fd_cli);
		if (_channel.end() != _channel.find(receiver)) {
			RecMessage(receiver, base_msg, fd_cli);
		}
		return (1);
	}
	//send to client
	msg = msg.substr(1, msg.size());
	if (!_client->GetFd(receiver))
		SendMessage(fd_cli, ERR_NOSUCHNICK(_client->GetName(fd_cli), receiver));
	SendMessage(_client->GetFd(receiver), RPL_PRIVMSG(_client->GetPrefix(fd_cli), receiver, msg));
	return (0);
}