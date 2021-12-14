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
protected:
    vector<Package> pile;

public:
    void push(Package& p);
    bool empty();
    size_t size();
};

class IPackageQueue : public IPackageStockpile
{
protected:
    PackageQueueType type;
public:
    virtual Package pop() = 0;
    PackageQueueType get_queue_type();
};

class PackageQueue : public IPackageQueue
{
public:
    PackageQueue(PackageQueueType type);
    Package pop();
};