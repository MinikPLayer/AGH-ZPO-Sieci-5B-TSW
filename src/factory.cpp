// 5B: Tomecki (408146), Sztefko (407388), Walawski (406822)
#include "factory.hpp"
#include <exception>

enum class SenderStatus
{
    NotVisited,
    Visited,
    Verified
};

bool Factory::is_consistent()
{
    throw std::exception();
}

void Factory::do_deliveries(Time t)
{
    throw std::exception();
}

void Factory::do_package_passing()
{
    throw std::exception();
}

void Factory::do_work(Time t)
{
    throw std::exception();
}

#pragma region Add or Remove

// Ramp
void Factory::add_ramp(Ramp&& r)
{
    rampCollection.add(std::move(r));
}

void Factory::remove_ramp(ElementID id)
{
    rampCollection.remove_by_id(id);
}

NodeCollection<Ramp>::iterator Factory::find_ramp_by_id(ElementID id)
{
    return rampCollection.find_by_id(id);
}

NodeCollection<Ramp>::const_iterator Factory::ramp_cbegin()
{
    return rampCollection.begin();
}

NodeCollection<Ramp>::const_iterator Factory::ramp_cend()
{
    return rampCollection.end();
}

// Worker
void Factory::add_worker(Worker&& w)
{
    workerCollection.add(std::move(w));
}

void Factory::remove_worker(ElementID id)
{
    workerCollection.remove_by_id(id);
}

NodeCollection<Worker>::iterator Factory::find_worker_by_id(ElementID id)
{
    return workerCollection.find_by_id(id);
}

NodeCollection<Worker>::const_iterator Factory::worker_cbegin()
{
    return workerCollection.begin();
}

NodeCollection<Worker>::const_iterator Factory::worker_cend()
{
    return workerCollection.end();
}

// Storehouse
void Factory::add_storehouse(Storehouse&& s)
{
    storehouseCollection.add(std::move(s));
}

void Factory::remove_storehouse(ElementID id)
{
    storehouseCollection.remove_by_id(id);
}

NodeCollection<Storehouse>::iterator Factory::find_storehouse_by_id(ElementID id)
{
    return storehouseCollection.find_by_id(id);
}

NodeCollection<Storehouse>::const_iterator Factory::storehouse_cbegin()
{
    return storehouseCollection.begin();
}

NodeCollection<Storehouse>::const_iterator Factory::storehouse_cend()
{
    return storehouseCollection.end();
}

#pragma endregion