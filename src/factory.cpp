// 5B: Tomecki (408146), Sztefko (407388), Walawski (406822)
#include "factory.hpp"
#include <exception>

enum class SenderStatus
{
    NotVisited,
    Visited,
    Verified
};

bool has_reachable_storehouse(PackageSender* sender, std::map<PackageSender*, SenderStatus>& statuses)
{
    if(statuses[sender] == SenderStatus::Verified)
        return true;

    statuses[sender] = SenderStatus::Verified;

    if(sender->receiver_preferences_.begin() == sender->receiver_preferences_.end())
        throw exception();

    bool alo = false;
    auto prefs = sender->receiver_preferences_;

    for(auto r : prefs.get_preferences())
    {
        auto type = r.first->get_receiver_type();
        if(type == ReceiverType::WORKER)
        {
            IPackageReceiver* r_ptr = r.first;
            auto w_ptr = dynamic_cast<Worker*>(r_ptr);
            auto s_ptr = dynamic_cast<PackageSender*>(w_ptr);
            if(s_ptr == sender)
                continue;

            alo = true;
            if(statuses[s_ptr] == SenderStatus::NotVisited)
                has_reachable_storehouse(s_ptr, statuses);
        }
    }

    statuses[sender] = SenderStatus::Verified;
    if(alo)
        return true;
    else
        return false;
}

bool Factory::is_consistent()
{
    std::map<PackageSender*, SenderStatus> statuses;

    for(auto it = &(*workerCollection.begin()), end = &(*workerCollection.end()); it < end; it++)
        statuses.insert({it, SenderStatus::NotVisited});

    for(auto it = &(*rampCollection.begin()), end = &(*rampCollection.end()); it < end; it++)
        statuses.insert({it, SenderStatus::NotVisited});

    try
    {
        for(auto it = &(*rampCollection.begin()), end = &(*rampCollection.end()); it < end; it++)
            if(!has_reachable_storehouse(dynamic_cast<PackageSender*>(it), statuses))
                return false;
    }
    catch(const std::exception& e)
    {
        return false;
    }
    
    return true;
}

void Factory::do_deliveries(Time t)
{
    for(auto i = rampCollection.begin(); i < rampCollection.end(); i++) 
        i->deliver_goods(t);
}

void Factory::do_package_passing()
{
    for(auto i = rampCollection.begin(); i < rampCollection.end(); i++) 
        i->send_package();

    for(auto i = workerCollection.begin(); i < workerCollection.end(); i++) 
        i->send_package();
}

void Factory::do_work(Time t)
{
    for(auto i = workerCollection.begin(); i < workerCollection.end(); i++) 
        i->do_work(t);
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