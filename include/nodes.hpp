// 5B: Tomecki (408146), Sztefko (407388), Walawski (406822)
#pragma once

#include "helpers.hpp"
#include "storage_types.hpp"
#include <map>
#include <optional>
#include <memory>

class IPackageReceiver
{
protected:
    ElementID id;
public:
    virtual void receive_package(Package&& p) {}
    virtual ElementID get_id() {return id;}

    //poniższy kod wymaga zainkludowania pliku "config.hpp"
    //#if (defined EXERCISE_ID && EXERCISE_ID != EXERCISE_ID_NODES)
    //    ReceiverType get_receiver_type(void);
    //#endif
};

class ReceiverPreferences
{
    using preferences_t = std::map<IPackageReceiver*, double>;
    using iterator = preferences_t::iterator;
    using const_iterator = preferences_t::const_iterator;

    ProbabilityGenerator gen;
public:
    preferences_t preferences;

    ReceiverPreferences(ProbabilityGenerator pg);
    ReceiverPreferences();

    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver(void);
    preferences_t& get_preferences(void);

    iterator begin() { return preferences.begin(); }
    iterator end() { return preferences.end(); }
    const_iterator cbegin() { return preferences.cbegin(); }
    const_iterator cend() { return preferences.cend(); }
};

class PackageSender
{
protected:
    void push_package(Package&&);

public:
    ReceiverPreferences receiver_preferences_;

    PackageSender() {}
    void send_package(void);
    std::optional<Package>& get_sending_buffer(void);
};

class Storehouse : public IPackageReceiver
{
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = nullptr)
    {
        this->id = id;
    }
};

class Ramp : public PackageSender
{
    ElementID id;
    TimeOffset offset;
public:
    Ramp(ElementID id, TimeOffset di) {this->id = id; this->offset = di;}
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval(void);

    ElementID get_id() {return id;}
};

class Worker : public PackageSender, public IPackageReceiver
{
    TimeOffset offset;
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q)
        { this->id = id; this->offset = pd;}

    void do_work(Time t);
    TimeOffset get_processing_duration(void);
    Time get_package_processing_start_time(void);
};

enum class ReceiverType
{
    WORKER,
    STOREHOUSE
};