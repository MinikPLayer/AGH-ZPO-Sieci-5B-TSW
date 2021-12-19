// 5B: Tomecki (408146), Sztefko (407388), Walawski (406822)
#pragma once
#include "types.hpp"

class Package
{
    ElementID id;

public:
    Package();
    Package(ElementID id); 

    bool operator==(const Package& r);
    ElementID get_id();

    ~Package();
};