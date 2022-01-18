// 5B: Tomecki (408146), Sztefko (407388), Walawski (406822)
#include "reports.hpp"
#include "factory.hpp"

void printReceivers(PackageSender r, std::ostream& os)
{
    os << "  Receivers:" << std::endl;
    auto prefs = r.receiver_preferences_.get_preferences();
    for(auto j = prefs.begin();j != prefs.end();j++)
    {
        auto el = (*j).first;
        auto type = el->get_receiver_type();
        string typeStr = "";
        switch (type)
        {
            case ReceiverType::WORKER:
                typeStr = "worker";
                break;

            case ReceiverType::STOREHOUSE:
                typeStr = "storehouse";
                break;
            
            default:
                std::cout << "Unknown receiver type" << endl;
                return;
        }

        os << "    " << typeStr << " #" << el->get_id() << std::endl;
    }
}


void generate_structure_report(const Factory& fact, std::ostream& os)
{
    Factory& f = (Factory&)fact;

    os << std::endl << "== LOADING RAMPS ==" << std::endl << std::endl;
    for(auto i = f.ramp_cbegin();i < f.ramp_cend();i++)
    {
        auto ramp = *i;
        os << "LOADING RAMP #" << ramp.get_id() << std::endl;
        os << "  Delivery interval: " << ramp.get_delivery_interval() << std::endl;

        printReceivers(ramp, os);

        os << std::endl;
    }

    os << std::endl << "== WORKERS ==" << std::endl << std::endl;
    for(auto i = f.worker_cbegin(); i < f.worker_cend(); i++)
    {
        os << "WORKER #" << i->get_id() << std::endl;
        os << "  Processing time: " << i->get_processing_duration() << std::endl;
        os << "  Queue type: ";
        std::string typeStr = "";
        auto recType = i->getQueueType();
        switch(recType)
        {
            case PackageQueueType::FIFO:
                typeStr = "FIFO";
                break;
            case PackageQueueType::LIFO:
                typeStr = "LIFO";
                break;
            default:
                std::cout << "Unknown package queue type" << std::endl;
                return;
        }
        os << typeStr << std::endl;

        printReceivers(*i, os);
        os << std::endl;
    }

    os << std::endl << "== STOREHOUSES ==" << std::endl << std::endl;
    for(auto i = f.storehouse_cbegin(); i < f.storehouse_cend(); i++)
        os << "STOREHOUSE #" << i->get_id() << std::endl << std::endl;
}

void generate_simulation_turn_report(const Factory&, std::ostream&, Time)
{

}
