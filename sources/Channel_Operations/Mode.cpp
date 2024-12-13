//
// Created by grebrune on 12/10/24.
//

//#include "Channel.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int mode_i(bool s, std::vector<std::string> v, size_t *j) {

}

int mode_k(bool s, std::vector<std::string> v, size_t *j) {

}

int mode_t(bool s, std::vector<std::string> v, size_t *j) {

}

int mode_o(bool s, std::vector<std::string> v, size_t *j) {

}

int mode_l(bool s, std::vector<std::string> v, size_t *j) {

}

int	exec_mode(bool s, char c, std::vector<std::string> v, size_t *j) {
	std::string	str = "iktol";
	int	*func[5](bool s, std::vector<std::string> v, size_t *j) = {&mode_i, &mode_k, &mode_t, &mode_o, &mode_l};

	for (size_t i = 0; i < 5; i++) {
		if (c == str[i])
			return (func[i](s, v, j));
	}
	return (1);
}

int exec_loop(std::vector<std::string> v) {
	size_t	j = 2;
	bool	s = (v[1][0] == '+');

	for (size_t i = 1; i < v[1].size(); i++) {
		exec_mode(s, v[1][i], v, &j);
	}
	return (0);
}

int	/*Channel::*/Mode(std::string buff) {
	std::vector<std::string>	v;
	std::istringstream	input;

	input.str(buff);
	for (std::string line; std::getline(input, line, ' ');) {
		v.push_back(line);
	}
	if (v[0][0] != '#')
		return (1); //ERR_NOSUCHCHANNEL
	if (v[1][0] != '+' && v[1][0] != '-')
		return (1); //ERR_NEEDMOREPARAMS
	for (size_t i = 1; i < v[1].size(); i++) {
		if (v[1][i] != 'i' && v[1][i] != 'k' && v[1][i] != 't' && v[1][i] != 'o' && v[1][i] != 'l')
			return (1); //ERR_UNKNOWNMODE
	}
	exec_loop(v);
	return (0);
}

int main(int ac, char **av) {
	std::string	input = av[1];
	return (Mode(input));
}