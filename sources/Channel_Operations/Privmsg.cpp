//
// Created by grebrune on 12/13/24.
//

#include "Channel.hpp"

void	Channel::send_chan_msg(std::string channel, std::string msg) {
	for (user_t it = _channel[channel].begin(); it != _channel[channel].end(); ++it) {
//		std::cout << "SEND_CHAN_MSG TO :" << _client->GetFd(it->first) << std::endl;
		send_error(_client->GetFd(it->first), msg);
	}
}

void	Channel::send_msg_to_chan(std::string channel, std::string msg, int to_not_send) {
	for (user_t it = _channel[channel].begin(); it != _channel[channel].end(); ++it) {
//		std::cout << "SEND_CHAN_MSG TO :" << _client->GetFd(it->first) << std::endl;
		if (to_not_send != _client->GetFd(it->first))
			send_error(_client->GetFd(it->first), msg);
	}
}

int	Channel::Privmsg(std::string buff, int fd_cli) {
	//trim message
	if (std::string::npos == buff.find(':'))
		return (send_error(fd_cli, ERR_NOTEXTTOSEND(_client->GetName(fd_cli))), 412);
	std::string msg = buff.substr(buff.find(':'), buff.size());
	if (std::string::npos != msg.find('\r'))
		msg = msg.substr(0, (msg.size() -1));
	//send to channel
	std::string receiver = buff.substr(0 , buff.find(' '));
	if (receiver[0] == '#') {
		if (_channel.find(receiver) == _channel.end())
			send_error(fd_cli, ERR_NOSUCHCHANNEL(receiver));
		msg = ":" + _client->GetPrefix(fd_cli) + " PRIVMSG " + receiver + " " + msg + "\n";
		send_msg_to_chan(receiver, msg, fd_cli);
		return (1);
	}
	//send to client
	std::cout << "PRIVMSG" << std::endl;
	std::cout << "MSG" << msg << std::endl;
	msg = msg.substr(1, msg.size());
	std::cout << "MSG" << msg << std::endl;
	std::cout << "REC" << receiver << std::endl;
	if (!_client->GetFd(receiver))
		send_error(fd_cli, ERR_NOSUCHNICK(_client->GetName(fd_cli), receiver));
	send_error(_client->GetFd(receiver), RPL_PRIVMSG(_client->GetPrefix(fd_cli), receiver, msg));
	return (0);
}