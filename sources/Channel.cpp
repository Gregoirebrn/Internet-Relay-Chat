//
// Created by grebrune on 12/2/24.
//

#include "Channel.hpp"

// Constructors & destructor
Channel::Channel(Client *cli) : _client(cli){
	srand( (unsigned)time(NULL) );
//init bot quiz
	std::ifstream file("Questions_Answers.txt");
	if (!file.is_open()) {
		std::cout << "Server: Error: Can't open Questions_Answers.txt." << std::endl;
		return;
	}
	for (std::string line; getline(file, line);) {
		std::istringstream stream(line);
		std::string date;
		std::string value;

		if (std::getline(stream, date, '\n')) {
			value = date.substr(date.find('?'), date.find('\n'));
			value = value.substr(2, value.size());
			date = date.substr(0, date.find('?'));
			date += "?\n";
			_quiz[date] = value;
		}
		else
			std::cerr << "Server: Error: bad input => " << line << "|" << value  << std::endl;
	}
	file.close();
}

// getters
bool Channel::CheckRights(const std::string& name, const std::string& chan, int fd_cli) {
	if (_all_chan.end() == _all_chan.find(chan))
		return (SendMessage(fd_cli, ERR_NOSUCHCHANNEL(chan)), false);
	for (chan_t it = _channel.begin(); it != _channel.end(); ++it) {
		if (it->first == chan) {
			user_t i = it->second.begin();
			while (i != it->second.end() && i->first != name)
				i++;
			if (i == it->second.end())
				return (SendMessage(fd_cli, ERR_NOTONCHANNEL(chan)), false);
			if (i->second)
				return true;
			return (SendMessage(fd_cli, ERR_CHANOPRIVSNEEDED(_client->GetName(fd_cli), chan)), false);
		}
	}
	return false;
}

// Public methods
bool	Channel::Canal_Operators(const std::string& buff, int fd_cli) {
	int	(Channel::*fptr[])(const std::string &, int ) = {&Channel::Kick, &Channel::Invite, \
		&Channel::Topic, &Channel::Mode, &Channel::Join, &Channel::Quit, &Channel::Privmsg, &Channel::Who};
	static std::string commands[] = {"KICK", "INVITE", "TOPIC", "MODE", "JOIN", "QUIT", "PRIVMSG", "WHO"};
	for (size_t i = 0; i < 8; ++i) {
		try {
			if (!buff.compare(0, commands[i].size(), commands[i])) {
				if (std::string::npos == buff.find(' ')) { //send nothing to the command so she throw the apropriate error
					(this->*fptr[i])("", fd_cli);
					return (false);
				}
				std::string arg = buff.substr((buff.find(' ') + 1), buff.size());
				if (arg.find('\r') != std::string::npos) // if we aree on Hexchat
					arg = arg.substr(0, arg.size() - 1);
				if (!_client->IsRegister(fd_cli))
					return (SendMessage(fd_cli, ERR_NOTREGISTRED), 404);
				(this->*fptr[i])(arg, fd_cli);
				if (i == 5) // if the command quit was executed we want to return true so the server destroy the client
					return true;
				return false;
			}
		}
		catch (std::exception &exe) {
			std::cout << "Channel: " << exe.what() << std::endl;
		}
	}
	return (false);
}

void	Channel::SendQuestion(const std::string &channel, const std::string &question) {
	std::string msg = ":bot PRIVMSG " + channel + " " + question + "\n";
	send_chan_msg(channel, msg);
}

void	Channel::FoundNextQuestion(const std::string &channel) {
	if (!_last_question[channel].empty() && _quiz.end() != _quiz.find(_last_question[channel])) {
		quiz_t it = _quiz.find(_last_question[channel]);
		if (_quiz.end() != it++) {
			it++;
			_last_question[channel] = it->first;
			SendQuestion(channel, it->first);
			return ;
		}
		else {
			quiz_t beg = _quiz.begin();
			_last_question[channel] = beg->first;
		}

	}
	size_t n = static_cast<size_t>(rand()) % (_quiz.size() - 1);
	size_t i = 0;
	for (quiz_t it = _quiz.begin(); it != _quiz.end(); ++it) {
		if (n == i)
			_last_question[channel] = it->first;
		i++;
	}
	SendQuestion(channel, _last_question[channel]);
}

void	Channel::RecMessage(const std::string &channel, std::string &msg, int fd) {
	if (_last_question.find(channel) == _last_question.end())
		return ;
	if (msg == ":bot")
		return (SendQuestion(channel, _last_question[channel]), (void)0);
	if (std::string::npos != msg.find(':'))
		msg = msg.substr(1, msg.size());
	if (!_last_question[channel].empty() && std::string::npos != _quiz[_last_question[channel]].find(msg)) {
		std::string good_answer_msg = _client->GetName(fd) + " has found the answer !\n";
		SendQuestion(channel, good_answer_msg);
		FoundNextQuestion(channel);
	}
}