// 5B: Tomecki (408146), Sztefko (407388), Walawski (406822)
#include "package.hpp"

Package::Package() 
{
    // Not implemented
    throw;
}

Package::Package(ElementID id)
{
    this->id = id;
}

// Nie wiem po co tutaj destruktor... ale w diagramie w skrypcie był zadeklarowany więc dodaję...
Package::~Package() {}

bool Package::operator==(const Package& r)
{
    return this->id == r.id;
}

ElementID Package::get_id()
{
    return this->id;    
}