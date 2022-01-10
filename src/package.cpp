// 5B: Tomecki (408146), Sztefko (407388), Walawski (406822)

#include "package.hpp"
#include <set>

inline static std::set<ElementID> freed_IDs;
inline static std::set<ElementID> assigned_IDs;
Package::Package() 
{
    if(freed_IDs.empty())
    {
        ElementID value = 0;
        if(assigned_IDs.empty())
            value = 1;
        else
            value = *(assigned_IDs.rbegin())+1;

        assigned_IDs.insert(value);
        this->id = value;
    }
    else
    {
        auto it = freed_IDs.begin();
        this->id = *it;
        assigned_IDs.insert(this->id);
        freed_IDs.erase(it);
    }
}

Package::Package(ElementID id)
{
    this->id = id;
    assigned_IDs.insert(id);
}

Package::Package(Package &&a) noexcept {
    this->id = a.id;
    a.id = 0;
}

Package::~Package() 
{
    if(this->id != 0)
    {
        freed_IDs.insert(this->id);
        assigned_IDs.erase(this->id);
    }
}

Package &Package::operator=(Package &&a){
    if(this->id != 0 ){
        freed_IDs.insert(this->id);
        assigned_IDs.erase(this->id);
    }
    this->id = a.id;
    a.id = 0;
    return *this;
}

bool Package::operator==(const Package& r)
{
    return this->id == r.id;
}

ElementID Package::get_id()
{
    return this->id;    
}