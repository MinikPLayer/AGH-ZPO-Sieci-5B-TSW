// 5B: Tomecki (408146), Sztefko (407388), Walawski (406822)
#include "nodes.hpp"

ReceiverPreferences::ReceiverPreferences(ProbabilityGenerator pg)
{
    this->gen = pg;
}

ReceiverPreferences::ReceiverPreferences()
{
    ProbabilityGenerator(default_probability_generator());
}

void ReceiverPreferences::add_receiver(IPackageReceiver* r)
{
    throw exception();
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r)
{
    throw exception();
}

IPackageReceiver* ReceiverPreferences::choose_receiver()
{
    throw exception();
}

ReceiverPreferences::preferences_t& ReceiverPreferences::get_preferences()
{
    throw exception();
}

void PackageSender::push_package(Package&& p)
{
    throw exception();  
}

void PackageSender::send_package()
{
    throw exception();  
}

std::optional<Package>& PackageSender::get_sending_buffer()
{
    throw exception();  
}

void Ramp::deliver_goods(Time t)
{
    throw exception();  
}

TimeOffset Ramp::get_delivery_interval()
{
    throw exception();  
}

void Worker::do_work(Time t)
{
    throw exception();  
}

TimeOffset Worker::get_processing_duration()
{
    throw exception();  
}

Time Worker::get_package_processing_start_time()
{
    throw exception();  
}