//
// Created by grebrune on 12/10/24.
//

#include "Channel.hpp"

int Channel::mode_i(int fd_cli, bool s, std::vector<std::string> v, size_t *j, all_chan_t itc) {
	(void) v;
	(void) j;
	(void) fd_cli;

	if (s) {
		itc->second.i_bool = true;
		return (send_chan_msg(v[0], RPL_MODE(v[0], _client->GetName(fd_cli), " +i\n")), 0);
	}
	else {
		itc->second.i_bool = false;
		return (send_chan_msg(v[0], RPL_MODE(v[0], _client->GetName(fd_cli), " -i\n")), 0);
	}
}

int Channel::mode_k(int fd_cli, bool s, std::vector<std::string> v, size_t *j, all_chan_t itc) {
	if (s && *j > v.size() - 1)
		return (SendMessage(fd_cli, ERR_NEEDMODPARAMS(_client->GetName(fd_cli), v[0], "MODE +k")), 461);
	if (s)
	{
		itc->second.chan_key = v[*j];
		(*j)++;
		return (send_chan_msg(v[0], RPL_LMODE(v[0], _client->GetName(fd_cli), " +k ", itc->second.chan_key)), 0);
	}
	else {
		itc->second.chan_key = "";
		return (send_chan_msg(v[0], RPL_MODE(v[0], _client->GetName(fd_cli), " -k\n")), 0);
	}
}

int Channel::mode_t(int fd_cli, bool s, std::vector<std::string> v, size_t *j, all_chan_t itc) {
	(void) v;
	(void) j;
	(void)fd_cli;
	if (s) {
		itc->second.t_bool = true;
		return (send_chan_msg(v[0], RPL_MODE(v[0], _client->GetName(fd_cli), " +t\n")), 0);
	}
	else {
		itc->second.t_bool = false;
		return (send_chan_msg(v[0], RPL_MODE(v[0], _client->GetName(fd_cli), " -t\n")), 0);
	}
}

int Channel::mode_o(int fd_cli, bool s, std::vector<std::string> v, size_t *j, all_chan_t itc) {
	(void)itc;
	if (*j > v.size() - 1)
		return (SendMessage(fd_cli, ERR_NEEDMODPARAMS(_client->GetName(fd_cli), v[0], "MODE +o")), 461);
	user_t it = _channel[v[0]].find(v[*j]);
	if (it == _channel[v[0]].end())
		return (SendMessage(fd_cli, ERR_USERNOTINCHANNEL(_client->GetName(fd_cli), v[0], v[*j])), 441); // TRY TO CONFIRM IF v[1]
	if (s) {
		it->second = true;
		return (send_chan_msg(v[0], RPL_OMODE(v[0], _client->GetName(fd_cli), " +o ", v[*j])), 0);
	}
	else {
		it->second = false;
		return (send_chan_msg(v[0], RPL_OMODE(v[0], _client->GetName(fd_cli), " -o ", v[*j])), 0);
	}
}

int Channel::mode_l(int fd_cli, bool s, std::vector<std::string> v, size_t *j, all_chan_t itc) {

	if (s && *j > v.size() -1)
		return (SendMessage(fd_cli, ERR_NEEDMODPARAMS(_client->GetName(fd_cli), v[0], "MODE +l")), 461);
	if (s)
	{
		std::istringstream	iss(v[*j]);
		iss >> itc->second.max_user;
		(*j)++;
		return (send_chan_msg(v[0], RPL_LMODE(v[0], _client->GetName(fd_cli), " +l ", iss.str())), 0);
	}
	else {
		itc->second.max_user = 0;
		return (send_chan_msg(v[0], RPL_MODE(v[0], _client->GetName(fd_cli), " -l\n")), 0);
	}
}

int	Channel::exec_mode(int fd_cli, bool s, char c, std::vector<std::string> v, size_t *j, all_chan_t itc) {
	std::string	str = "iktol";
	int	(Channel::*func[5])(int fd_cli, bool s, std::vector<std::string> v, size_t *j, all_chan_t itc) = {&Channel::mode_i, \
	&Channel::mode_k, &Channel::mode_t, &Channel::mode_o, &Channel::mode_l};

	for (size_t i = 0; i < 5; i++) {
		if (c == str[i]) {
			return ((this->*func[i])(fd_cli, s, v, j, itc));
		}
	}
	return (SendMessage(fd_cli, ERR_UNKNOWNMODE(_client->GetName(fd_cli), v[0], c)), 472);
}

int Channel::exec_loop(int fd_cli, std::vector<std::string> v, all_chan_t itc) {
	size_t	j = 2;
	bool	s = (v[1][0] == '+');
	int		err = 0;

	for (size_t i = 1; i < v[1].size(); i++) {
		err += exec_mode(fd_cli, s, v[1][i], v, &j, itc);
		if (err != 0)
			break ;
	}
	return (err);
}

std::string Channel::GetMode(const std::string& chan) {
	std::string mode;

	mode = "+";
	if (_all_chan[chan].i_bool)
		mode += "i";
	if (!_all_chan[chan].chan_key.empty())
		mode += "k";
	if (_all_chan[chan].t_bool)
		mode += "t";
	if (_all_chan[chan].max_user != 0)
		mode += "l";
	return (mode);
}

int	Channel::Mode(const std::string& buff, int fd_cli) {
	// For the creation of a channel
	std::string channel = buff;
	if (std::string::npos != buff.find('\n'))
		std::string channel = buff.substr(0, (buff.size() -1));
	if (_channel.find(channel) != _channel.end())
		return (SendMessage(fd_cli, RPL_CHANNELMODEIS(_client->GetName(fd_cli), channel, GetMode(channel))), 404);
	// Trim
	std::vector<std::string>	v;
	std::istringstream	input;

	input.str(buff);
	for (std::string line; std::getline(input, line, ' ');) {
		v.push_back(line);
	}
	// Finder
	if (v[0][0] != '#')
		return (SendMessage(fd_cli, ERR_NOSUCHCHANNEL(v[0])), 403);
	v[0] = v[0].substr(0, v[0].size());
	all_chan_t itc = _all_chan.find(v[0]);
	if (itc == _all_chan.end())
		return (SendMessage(fd_cli, ERR_NOSUCHCHANNEL(v[0])), 403);
	if (_channel[v[0]].find(_client->GetName(fd_cli)) == _channel[v[0]].end())
		return (SendMessage(fd_cli, ERR_NOTONCHANNEL(v[0])), 442);
	if (!_channel[v[0]][_client->GetName(fd_cli)])
		return (SendMessage(fd_cli, ERR_CHANOPRIVSNEEDED(_client->GetName(fd_cli), v[0])), 482);
	if (v[1][0] != '+' && v[1][0] != '-')
		return (SendMessage(fd_cli, ERR_NEEDMODPARAMS(_client->GetName(fd_cli), v[0], "MODE")), 461);
	exec_loop(fd_cli, v, itc);
	return (0);
}