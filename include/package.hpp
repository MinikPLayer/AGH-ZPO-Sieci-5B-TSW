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