//
// Created by grebrune on 12/10/24.
//

#include "Channel.hpp"

int Channel::mode_i(bool s, std::vector<std::string> v, size_t *j, all_chan_t itc) {
	(void) v;
	(void) j;

	if (s)
		itc->second.i_bool = true;
	else
		itc->second.i_bool = false;
	return (0);
}

int Channel::mode_k(bool s, std::vector<std::string> v, size_t *j, all_chan_t itc) {
	if (*j > v.size())
		return (1); // ERR_NEEDMOREPARAMS (461)
	if (s)
	{
		itc->second.chan_key = v[*j];
		(*j)++;
	}
	else
		itc->second.chan_key = "";
	return (0);
}

int Channel::mode_t(bool s, std::vector<std::string> v, size_t *j, all_chan_t itc) {
	(void) v;
	(void) j;

	if (s)
		itc->second.t_bool = true;
	else
		itc->second.t_bool = false;
	return (0);
}

int Channel::mode_o(bool s, std::vector<std::string> v, size_t *j, all_chan_t itc) {
	if (*j > v.size())
		return (1); // ERR_NEEDMOREPARAMS (461)
	user_t	it = _channel[v[0]].find(v[*j]);
	if (it == _channel[v[0]].end())
		return (1); // ERR_USERNOTINCHANNEL (441)
	if (s)
		it->second = true;
	else
		it->second = false;
	return (0);
};

int Channel::mode_l(bool s, std::vector<std::string> v, size_t *j, all_chan_t itc) {
	std::istringstream	iss(v[*j]);

	if (*j > v.size())
		return (1); // ERR_NEEDMOREPARAMS (461)
	if (s)
	{
		iss >> itc->second.max_user;
		(*j)++;
	}
	else
		itc->second.max_user = 0;
	return (0);
}

int	Channel::exec_mode(bool s, char c, std::vector<std::string> v, size_t *j, all_chan_t itc) {
	std::string	str = "iktol";
	int	(Channel::*func[5])(bool s, std::vector<std::string> v, size_t *j, all_chan_t itc) = {&Channel::mode_i, &Channel::mode_k, &Channel::mode_t, &Channel::mode_o, &Channel::mode_l};

	for (size_t i = 0; i < 5; i++) {
		if (c == str[i])
			return ((this->*func[i])(s, v, j, itc));
	}
	return (1); // ERR_UNKNOWNMODE (472)
}

int Channel::exec_loop(std::vector<std::string> v, all_chan_t itc) {
	size_t	j = 2;
	bool	s = (v[1][0] == '+');
	int		err = 0;

	for (size_t i = 1; i < v[1].size(); i++) {
		err += exec_mode(s, v[1][i], v, &j, itc);
	}
	return (err);
}

int	Channel::Mode(std::string buff, int fd_cli) {
	std::vector<std::string>	v;
	std::istringstream	input;

	input.str(buff);
	for (std::string line; std::getline(input, line, ' ');) {
		v.push_back(line);
	}
	if (v[0][0] != '#')
		return (1); // ERR_NOSUCHCHANNEL (403)
	v[0] = v[0].substr(1, v[0].size());
	all_chan_t	itc = _all_chan.find(v[0]);
	if (itc == _all_chan.end())
		return (1); // ERR_NOSUCHCHANNEL (403)
	user_t	it = _channel[v[0]].find(_client->GetName(fd_cli));
	if (it == _channel[v[0]].end())
		return (1); // ERR_NOTONCHANNEL (442)
	if (!it->second)
		return (1); // ERR_CHANOPRIVSNEEDED (482)
	if (v[1][0] != '+' && v[1][0] != '-')
		return (1); // ERR_NEEDMOREPARAMS (461)
	exec_loop(v, itc);
	return (0);
}
