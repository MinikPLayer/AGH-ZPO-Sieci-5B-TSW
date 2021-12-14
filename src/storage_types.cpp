#include "storage_types.hpp"
#include <stdexcept>

void IPackageStockpile::push(Package& p)
{
    pile.push_back(p);
}

bool IPackageStockpile::empty()
{
    return pile.size() == 0;
}

size_t IPackageStockpile::size()
{
    return pile.size();
}

PackageQueueType IPackageQueue::get_queue_type()
{
    return this->type;
}

PackageQueue::PackageQueue(PackageQueueType type)
{
    this->type = type;
}

Package PackageQueue::pop()
{
    if(empty())
        throw std::runtime_error("Pile is empty!");

    int index = 0;
    // FIFO
    if(type == PackageQueueType::FIFO)
        index = 0;
    // LIFO
    else
        index = pile.size() - 1;

    Package p = pile[index];
    pile.erase(pile.begin() + index);

    return p;
}