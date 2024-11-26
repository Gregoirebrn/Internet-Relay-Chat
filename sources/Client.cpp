//
// Created by grebrune on 11/26/24.
//

#include "Client.hpp"

// Constructors & destructor
Client::Client(void) {
    std::cout << "Client default constructor called!" << std::endl;
}

Client::Client(const Client &copy) {
    std::cout << "Client copy constructor called!" << std::endl;
    *this = copy;
}

Client::~Client(void) {
    std::cout << "Client default destructor called!" << std::endl;
}

// Overloaded operators
Client &Client::operator=(const Client &src) {
    if (this != &src)
    {
    }
    return (*this);
}

// Public methods