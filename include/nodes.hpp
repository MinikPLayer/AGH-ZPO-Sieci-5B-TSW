// 5B: Tomecki (408146), Sztefko (407388), Walawski (406822)
#pragma once

#include "helpers.hpp"
#include "storage_types.hpp"
#include <map>
#include <optional>
#include <memory>

class IPackageReceiver
{
public:
    virtual void receive_package(Package&& p) {}
    virtual ElementID get_id() {return ElementID(-1);}

    //poniższy kod wymaga zainkludowania pliku "config.hpp"
    //#if (defined EXERCISE_ID && EXERCISE_ID != EXERCISE_ID_NODES)
    //    ReceiverType get_receiver_type(void);
    //#endif
};

class ReceiverPreferences
{
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

public:
    preferences_t preferences;

    ReceiverPreferences(ProbabilityGenerator pg);
    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver(void);
    preferences_t& get_preferences(void);
};

class PackageSender
{
protected:
    void push_package(Package&&);

public:
    ReceiverPreferences receiver_preferences_;

    PackageSender(PackageSender&&);
    void send_package(void);
    std::optional<Package>& get_sending_buffer(void);
};

class Storehouse : IPackageReceiver
{
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d);
};

class Ramp : public PackageSender
{
public:
    Ramp(ElementID id, TimeOffset di);
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval(void);
    ElementID get_id(void);
};

class Worker : public PackageSender, IPackageReceiver
{
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q);
    void do_work(Time t);
    TimeOffset get_processing_duration(void);
    Time get_package_processing_start_time(void);
};

enum class ReceiverType
{
    WORKER,
    STOREHOUSE
};