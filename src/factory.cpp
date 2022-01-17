// 5B: Tomecki (408146), Sztefko (407388), Walawski (406822)
#include "factory.hpp"
#include <exception>
#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>
#include <functional>
#include <memory>

enum class SenderStatus
{
    NotVisited,
    Visited,
    Verified
};

class DExcept : public std::logic_error {
public:
    DExcept() : std::logic_error("Graph is diconnected") {}
};

bool has_reachable_storehouse(PackageSender* sender, std::map<PackageSender*, SenderStatus>& statuses)
{
    if(statuses[sender] == SenderStatus::Verified)
        return true;

    statuses[sender] = SenderStatus::Verified;

    if(sender->receiver_preferences_.begin() == sender->receiver_preferences_.end())
        throw DExcept();

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
            if(s_ptr == sender || s_ptr == nullptr)
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
        throw DExcept();
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
    catch(const DExcept&)
    {
        return false;
    }
    
    throw DExcept();
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
    //workerCollection.remove_by_id(id);
    for(auto it = rampCollection.begin(); it < rampCollection.end(); it++)
    {
        auto prefs = it->receiver_preferences_.get_preferences();
        for(auto el = prefs.begin(); el != prefs.end(); el++)
        {
            if(el->first->get_id() == id)
            {
                it->receiver_preferences_.remove_receiver(&*(el->first));
                break;
            }
        }
    }
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

struct Argument
{
    std::string title;
    std::string value;

    Argument(std::string t, std::string v) : title(t), value(v) {}
};

struct LineParser
{
private:
    function<bool(Factory&, vector<Argument>)> parserFunc;
public:

    std::string tag;
    size_t argsCount;

    LineParser(std::string t, size_t ac, function<bool(Factory&, vector<Argument>)> f) : parserFunc(f), tag(t), argsCount(ac) {}
    bool Execute(Factory& f, vector<Argument> args)
    {
        if(args.size() != argsCount)
        {
            std::cout << "Wrong arguments count" << std::endl; 
            return false;
        } 
        return parserFunc(f, args);
    }
};

template<class T>
T S2T(std::string s)
{
    std::stringstream ss(s);
    T n;
    ss >> n;

    return n;
}

bool ParseRamp(Factory& f, vector<Argument> args)
{
    ElementID id = S2T<ElementID>(args[0].value);
    TimeOffset deliveryInterval = S2T<TimeOffset>(args[1].value);

    f.add_ramp(Ramp(id, deliveryInterval));

    return true;
}

bool ParseWorker(Factory& f, vector<Argument> args)
{
    ElementID id = S2T<ElementID>(args[0].value);
    TimeOffset processingTime = S2T<TimeOffset>(args[1].value);

    PackageQueueType type = PackageQueueType::FIFO;

    if(args[2].value == "FIFO")
    {
        type = PackageQueueType::FIFO;
    }
    else if(args[2].value == "LIFO")
    {
        type = PackageQueueType::LIFO;
    }
    else
    {
        std::cout << "Unknown queue type" << std::endl;
        return false;
    }

    f.add_worker(Worker(id, processingTime, std::make_unique<PackageQueue>(type)));

    return true;
}

bool ParseStorehouse(Factory& f, vector<Argument> args)
{
    ElementID id = S2T<ElementID>(args[0].value);

    f.add_storehouse(Storehouse(id));

    return true;
}

bool ParseLink(Factory& f, vector<Argument> args)
{
    std::string src = args[0].value;
    std::string dest = args[1].value;

    vector<std::string> srcSplitted = splitString(src, '-');
    vector<std::string> destSplitted = splitString(dest, '-');

    PackageSender* srcObject = nullptr;
    IPackageReceiver* destObject = nullptr;

    ElementID srcId = S2T<ElementID>(srcSplitted[1]);
    ElementID destId = S2T<ElementID>(destSplitted[1]);
    if(srcSplitted[0] == "ramp")
        srcObject = &*f.find_ramp_by_id(srcId);
    else if(srcSplitted[0] == "worker")
        srcObject = &*f.find_worker_by_id(srcId);
    else 
    {
        std::cout << "Unknown source object type" << std::endl;
        return false;
    }

    if(destSplitted[0] == "worker")
        destObject = &*f.find_worker_by_id(destId);
    else if(destSplitted[0] == "store")
        destObject = &*f.find_storehouse_by_id(destId);
    else 
    {
        std::cout << "Unknown destination object type" << std::endl;
        return false;
    }

    srcObject->receiver_preferences_.add_receiver(destObject);

    return true;
}

std::array<LineParser, 4> parsers = {
    LineParser("LOADING_RAMP", 2, ParseRamp),
    LineParser("WORKER", 3, ParseWorker),
    LineParser("STOREHOUSE", 1, ParseStorehouse),
    LineParser("LINK", 2, ParseLink),
};

vector<Argument> GenerateArgs(vector<std::string> parts, int startIndex = 1)
{
    vector<Argument> args;
    for(size_t i = startIndex;i<parts.size();i++)
    {
        auto p = splitString(parts[i], '=');
        if(p.size() != 2)
        {
            std::cout << "Error getting argument from \"" << parts[i] << "\" - wrong split string size" << std::endl;
            return vector<Argument>();
        }

        args.push_back(Argument(p[0], p[1]));
    }

    return args;
}

bool ParseLine(Factory& f, std::string line)
{
    line = removeWhitespaces(line);

    if(line.size() == 0 || line[0] == ';' || line[0] == ' ')
        return true;

    auto data = splitString(line, ' ');
    std::string tag = data[0];
    for(size_t i = 0;i<parsers.size();i++)
    {
        if(parsers[i].tag == tag)
        {
            auto args = GenerateArgs(data);
            return parsers[i].Execute(f, args);
        }
    }

    std::cout << "Parser not found for line \"" << line << "\"" << endl;
    return false;
}

Factory load_factory_structure(std::istream& is)
{
    Factory f;

    std::string s = "";
    for(int i = 0;getline(is, s);i++)
    {
        try
        {     
            if(!ParseLine(f, s))
                std::cout << "Error parsing line at " << i + 1 << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cout << "Exception thrown while parsing file at line " << i + 1 << std::endl;
        }

    }

    return f;
}

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

void save_factory_structure(Factory& f, std::ostream& os)
{
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