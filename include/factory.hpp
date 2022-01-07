// 5B: Tomecki (408146), Sztefko (407388), Walawski (406822)
#pragma once

#include "nodes.hpp"

template<class T>
class NodeCollection
{
public:
    using container_t = typename std::vector<T>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;
    
    void add(T&& node);
    void remove_by_id(ElementID id);
    NodeCollection<T>::iterator find_by_id(ElementID id);
    //iteratory
};

class Factory
{
private:
    template<class T>
    void remove_receiver(NodeCollection<T>& collection, ElementID id);

public:
    bool is_consistent(void);
    void do_deliveries(Time);
    void do_package_passing(void);
    void do_work(Time);

    void add_ramp(Ramp&&);
    void remove_ramp(ElementID id);
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id);
    NodeCollection<Ramp>::const_iterator ramp_cbegin();
    NodeCollection<Ramp>::const_iterator ramp_cend();

    // dla pozostałych wezlow ma być niby analogicznie
    void add_worker(Worker&&);
    void remove_worker(ElementID id);
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id);
    NodeCollection<Worker>::const_iterator worker_cbegin();
    NodeCollection<Worker>::const_iterator worker_cend();

    void add_storehouse(Storehouse&&);
    void remove_storehouse(ElementID id);
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id);
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin();
    NodeCollection<Storehouse>::const_iterator storehouse_cend();
};