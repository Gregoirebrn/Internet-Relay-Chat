//
// Created by grebrune on 11/27/24.
//

#pragma once

#define ERR_NEEDMOREPARAMS(command) ("461 : " + std::string(command) + " :Not enough parameters")
#define ERR_ALREADYREGISTRED "462 :You may not reregister"

//nickname
//#define ERR_NONICKNAMEGIVEN
//#define ERR_ERRONEUSNICKNAME
//#define ERR_NICKNAMEINUSE
//#define ERR_NICKCOLLISION

//kick
//#define ERR_NOSUCHCHANNEL
//#define ERR_BADCHANMASK
//#define ERR_CHANOPRIVSNEEDED
//#define ERR_NOTONCHANNEL

//mode
//#define RPL_CHANNELMODEIS
//#define ERR_NOSUCHNICK
//#define ERR_KEYSET
//#define RPL_BANLIST
//#define RPL_ENDOFBANLIST
//#define ERR_UNKNOWNMODE
//#define ERR_USERSDONTMATCH
//#define RPL_UMODEIS
//#define ERR_UMODEUNKNOWNFLAG

//invite
//#define ERR_USERONCHANNEL
//#define RPL_INVITING
//#define RPL_AWAY

//topic
//#define RPL_NOTOPIC
//#define RPL_TOPIC