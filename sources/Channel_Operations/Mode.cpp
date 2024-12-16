//
// Created by grebrune on 12/10/24.
//

#include "Channel.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int Channel::mode_i(bool s, std::vector<std::string> v, size_t *j) {

}

int Channel::mode_k(bool s, std::vector<std::string> v, size_t *j) {

}

int Channel::mode_t(bool s, std::vector<std::string> v, size_t *j) {

}

int Channel::mode_o(bool s, std::vector<std::string> v, size_t *j) {

}

int Channel::mode_l(bool s, std::vector<std::string> v, size_t *j) {

}

int	Channel::exec_mode(bool s, char c, std::vector<std::string> v, size_t *j) {
	std::string	str = "iktol";
	int	(Channel::*func[5])(bool s, std::vector<std::string> v, size_t *j) = {&Channel::mode_i, &Channel::mode_k, &Channel::mode_t, &Channel::mode_o, &Channel::mode_l};

	for (size_t i = 0; i < 5; i++) {
		if (c == str[i])
			return ((this->*func[i])(s, v, j));
	}
	return (1);
}

int Channel::exec_loop(std::vector<std::string> v) {
	size_t	j = 2;
	bool	s = (v[1][0] == '+');

	for (size_t i = 1; i < v[1].size(); i++) {
		exec_mode(s, v[1][i], v, &j);
	}
	return (0);
}

int	Channel::Mode(std::string buff, int fd_cli) {
	std::vector<std::string>	v;
	std::istringstream	input;

	input.str(buff);
	for (std::string line; std::getline(input, line, ' ');) {
		v.push_back(line);
	}
	if (v[0][0] != '#')
		return (1); //ERR_NOSUCHCHANNEL
	v[0] = v[0].substr(1, v[0].size());
	if (_all_chan.find(v[0]) == _all_chan.end())
		return (1); //ERR_NOSUCHCHANNEL
	user_t	it = _channel[v[0]].find(_client->GetName(fd_cli));
	if (it == _channel[v[0]].end())
		return (1); //ERR_NOTONCHANNEL
	if (it->second == false)
		return (1); //ERR_CHANOPRIVSNEEDED
	if (v[1][0] != '+' && v[1][0] != '-')
		return (1); //ERR_NEEDMOREPARAMS
	for (size_t i = 1; i < v[1].size(); i++) {
		if (v[1][i] != 'i' && v[1][i] != 'k' && v[1][i] != 't' && v[1][i] != 'o' && v[1][i] != 'l')
			return (1); //ERR_UNKNOWNMODE
	}
	exec_loop(v);
	return (0);
}

//int main(int ac, char **av) {
//	std::string	input = av[1];
//	return (Mode(input));
//}