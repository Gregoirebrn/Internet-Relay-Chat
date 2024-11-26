//
// Created by grebrune on 11/26/24.
//

#ifndef INTERNET_RELAY_CHAT_CLIENT_HPP
# define INTERNET_RELAY_CHAT_CLIENT_HPP
# include <iostream>
class Client {
private :    
public :
    // Constructors & destructor
    Client();
    Client(const Client &copy);
    ~Client();

    // Overloaded operators
    Client &operator=(const Client &src);
    
    // Public methods
};

#endif //INTERNET_RELAY_CHAT_CLIENT_HPP