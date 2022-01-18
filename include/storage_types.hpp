// 5B: Tomecki (408146), Sztefko (407388), Walawski (406822)

#pragma once
#include "package.hpp"
#include <vector>
using namespace std;

enum class PackageQueueType
{
    FIFO,
    LIFO
};

class IPackageStockpile
{
public:
    using container_t = typename std::vector<Package>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

protected:
    container_t pile;

public:
    const_iterator cbegin() const {return pile.cbegin();}
    const_iterator cend() const {return pile.cend();}

    iterator begin() {return pile.begin();}
    iterator end() {return pile.end();}

    void push(Package&& p);
    bool empty();
    size_t size();
};

class IPackageQueue : public IPackageStockpile
{
protected:
    PackageQueueType type;
public:
    virtual Package pop() = 0;
    PackageQueueType get_queue_type() const;
};

class PackageQueue : public IPackageQueue
{
public:
    PackageQueue(PackageQueueType type);
    Package pop();
};