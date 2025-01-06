//
// Created by grebrune on 12/10/24.
//

#include "Channel.hpp"

int Channel::mode_i(int fd_cli, bool s, std::vector<std::string> v, size_t *j, all_chan_t itc) {
	(void) v;
	(void) j;
	(void) fd_cli;

	if (s)
		itc->second.i_bool = true;
	else
		itc->second.i_bool = false;
	return (0);
}

int Channel::mode_k(int fd_cli, bool s, std::vector<std::string> v, size_t *j, all_chan_t itc) {
	if (s && *j > v.size() - 1)
		return (send_error(fd_cli, ERR_NEEDMOREPARAMS("MODE +k")), 461);
	if (s)
	{
		itc->second.chan_key = v[*j];
		(*j)++;
	}
	else
		itc->second.chan_key = "";
	return (0);
}

int Channel::mode_t(int fd_cli, bool s, std::vector<std::string> v, size_t *j, all_chan_t itc) {
	(void) v;
	(void) j;
	(void)fd_cli;
	if (s)
		itc->second.t_bool = true;
	else
		itc->second.t_bool = false;
	return (0);
}

int Channel::mode_o(int fd_cli, bool s, std::vector<std::string> v, size_t *j, all_chan_t itc) {
	(void)itc;
	if (*j > v.size() - 1)
		return (send_error(fd_cli, ERR_NEEDMOREPARAMS("MODE +o")), 461);
	user_t it = _channel[v[0]].find(v[*j]);
	if (it == _channel[v[0]].end())
		return (send_error(fd_cli, ERR_USERNOTINCHANNEL(_client->GetName(fd_cli), v[1])), 441); // TRY TO CONFIRM IF v[1]
	if (s)
		it->second = true;
	else
		it->second = false;
	return (0);
};

int Channel::mode_l(int fd_cli, bool s, std::vector<std::string> v, size_t *j, all_chan_t itc) {
	std::istringstream	iss(v[*j]);

	if (s && *j > v.size() -1)
		return (send_error(fd_cli, ERR_NEEDMOREPARAMS("MODE +l")), 461);
	if (s)
	{
		iss >> itc->second.max_user;
		(*j)++;
	}
	else
		itc->second.max_user = 0;
	return (0);
}

int	Channel::exec_mode(int fd_cli, bool s, char c, std::vector<std::string> v, size_t *j, all_chan_t itc) {
	std::string	str = "iktol";
	int	(Channel::*func[5])(int fd_cli, bool s, std::vector<std::string> v, size_t *j, all_chan_t itc) = {&Channel::mode_i, &Channel::mode_k, &Channel::mode_t, &Channel::mode_o, &Channel::mode_l};

	for (size_t i = 0; i < 5; i++) {
		if (c == str[i]) {
			std::cout << "MODE CHOICE " << i << "--" << std::endl;
			return ((this->*func[i])(fd_cli, s, v, j, itc));
		}
	}
	return (send_error(fd_cli, ERR_UNKNOWNMODE(_client->GetName(fd_cli))), 472);
}

int Channel::exec_loop(int fd_cli, std::vector<std::string> v, all_chan_t itc) {
	size_t	j = 2;
	bool	s = (v[1][0] == '+');
	int		err = 0;

	for (size_t i = 1; i < v[1].size(); i++) {
		std::cout << "MODE FORR " << i << "--" << std::endl;
		err += exec_mode(fd_cli, s, v[1][i], v, &j, itc);
		if (err != 0)
			break ;
	}
	return (err);
}

int	Channel::Mode(std::string buff, int fd_cli) {
	// For the creation of a channel
	std::string channel = buff;
	if (std::string::npos != buff.find("\n"))
		std::string channel = buff.substr(0, (buff.size() -1));
	std::cout << "MODE CHANNEL :" << channel << std::endl;
	if (_channel.find(channel) != _channel.end())
		return (send_error(fd_cli, RPL_CREAMODE(channel)), 404);
	// Trim
	std::vector<std::string>	v;
	std::istringstream	input;

	input.str(buff);
	for (std::string line; std::getline(input, line, ' ');) {
		v.push_back(line);
	}
	// Finder
	if (v[0][0] != '#')
		return (send_error(fd_cli, ERR_NOSUCHCHANNEL(v[0])), 403);
	v[0] = v[0].substr(0, v[0].size());
	all_chan_t itc = _all_chan.find(v[0]);
	if (itc == _all_chan.end())
		return (send_error(fd_cli, ERR_NOSUCHCHANNEL(v[0])), 403);
//	user_t it = _channel[v[0]].find(_client->GetName(fd_cli));
	if (_channel[v[0]].find(_client->GetName(fd_cli)) == _channel[v[0]].end())
		return (send_error(fd_cli, ERR_NOTONCHANNEL(v[0])), 442);
	std::cout << "MODE :::::" << _channel[v[0]][_client->GetName(fd_cli)] << std::endl;
	if (!_channel[v[0]][_client->GetName(fd_cli)])
		return (send_error(fd_cli, ERR_CHANOPRIVSNEEDED(_client->GetName(fd_cli), v[0])), 482);
	if (v[1][0] != '+' && v[1][0] != '-')
		return (send_error(fd_cli, ERR_NEEDMOREPARAMS("MODE")), 461);
	exec_loop(fd_cli, v, itc);
	return (0);
}