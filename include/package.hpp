// 5B: Tomecki (408146), Sztefko (407388), Walawski (406822)

#pragma once
#include "types.hpp"

class Package
{
    ElementID id;

public:
    Package();
    Package(ElementID id); 
    Package(Package&& a) noexcept;
    Package(const Package&) = default;

    bool operator==(const Package& r);
    Package& operator=(Package&& a);
    ElementID get_id();
    ElementID get_id() const;

    ~Package();
};