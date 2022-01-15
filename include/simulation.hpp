// 5B: Tomecki (408146), Sztefko (407388), Walawski (406822)
#pragma once
#include "factory.hpp"

// funkcja globalna
void simulate(Factory& f, TimeOffset d, std::function<void (Factory&, Time)>);
