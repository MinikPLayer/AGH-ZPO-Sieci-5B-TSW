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
    throw std::exception();
}

void Factory::remove_ramp(ElementID id)
{
    throw std::exception();
}

NodeCollection<Ramp>::iterator Factory::find_ramp_by_id(ElementID id)
{
    throw std::exception();
}

NodeCollection<Ramp>::const_iterator Factory::ramp_cbegin()
{
    throw std::exception();
}

NodeCollection<Ramp>::const_iterator Factory::ramp_cend()
{
    throw std::exception();
}

// Worker
void Factory::add_worker(Worker&& w)
{
    throw std::exception();
}

void Factory::remove_worker(ElementID id)
{
    throw std::exception();
}

NodeCollection<Worker>::iterator Factory::find_worker_by_id(ElementID id)
{
    throw std::exception();
}

NodeCollection<Worker>::const_iterator Factory::worker_cbegin()
{
    throw std::exception();
}

NodeCollection<Worker>::const_iterator Factory::worker_cend()
{
    throw std::exception();
}

// Storehouse
void Factory::add_storehouse(Storehouse&& s)
{
    throw std::exception();
}

void Factory::remove_storehouse(ElementID id)
{
    throw std::exception();
}

NodeCollection<Storehouse>::iterator Factory::find_storehouse_by_id(ElementID id)
{
    throw std::exception();
}

NodeCollection<Storehouse>::const_iterator Factory::storehouse_cbegin()
{
    throw std::exception();
}

NodeCollection<Storehouse>::const_iterator Factory::storehouse_cend()
{
    throw std::exception();
}

#pragma endregion