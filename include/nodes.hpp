// 5B: Tomecki (408146), Sztefko (407388), Walawski (406822)
#pragma once

#include "helpers.hpp"
#include "storage_types.hpp"
#include <map>
#include <optional>
#include <memory>

enum class ReceiverType
{
    WORKER,
    STOREHOUSE
};

class IPackageReceiver
{
protected:
    ElementID id;
public:
    virtual void receive_package(Package&& p) {}
    virtual ElementID get_id() {return id;}
    virtual ElementID get_id() const {return id;}
    // do sprawdzenia
    virtual ReceiverType get_receiver_type(void) {return ReceiverType::WORKER;}

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
    std::optional<Package> opt_ = std::nullopt;

protected:
    void push_package(Package&&);

public:
    ReceiverPreferences receiver_preferences_;

    PackageSender(): receiver_preferences_(default_probability_generator) {}
    PackageSender(const PackageSender&) = default;
    PackageSender(PackageSender&&) = default;

    PackageSender& operator=(PackageSender& p) = default;
    PackageSender& operator=(PackageSender&& p) = default;

    void send_package(void);
    std::optional<Package>& get_sending_buffer(void);
};

class Storehouse : public IPackageReceiver
{
    std::unique_ptr<IPackageQueue> queue;

public:
    Storehouse(ElementID id, std::unique_ptr<IPackageQueue> d = nullptr)
    {
        this->id = id;
        if(d == nullptr)
        {
            d = std::make_unique<PackageQueue>(PackageQueueType::FIFO);
        }
        this->queue = std::move(d);
    }

    void receive_package(Package&& p) override;
};

class Ramp : public PackageSender
{
    ElementID id;
    TimeOffset offset;
    Time d_t;
public:
    Ramp(ElementID id, TimeOffset di) : id(id), offset(di), d_t(0) {}

    void deliver_goods(Time t);
    TimeOffset get_delivery_interval() {return offset;}

    ElementID get_id() const {return id;}
};

class Worker : public PackageSender, public IPackageReceiver
{
    TimeOffset offset;
    Time start_time;
    std::unique_ptr<IPackageQueue> queue;
    std::optional<Package> cur_package;
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q);

    void receive_package(Package &&p) override;

    void do_work(Time t);
    TimeOffset get_processing_duration(void);
    Time get_package_processing_start_time(void);
};
