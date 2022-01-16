// 5B: Tomecki (408146), Sztefko (407388), Walawski (406822)
#include "factory.hpp"
#include <exception>
#include <stdexcept>
#include <string>
#include <iostream>
#include <functional>

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
    int argsCount;

    LineParser(std::string t, int ac, function<bool(Factory&, vector<Argument>)> f) : tag(t), argsCount(ac), parserFunc(f) {}
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

bool ParseRamp(Factory& f, vector<Argument> args)
{
    return false;
}

bool ParseWorker(Factory& f, vector<Argument> args)
{
    return false;
}

bool ParseStorehouse(Factory& f, vector<Argument> args)
{
    return false;
}

bool ParseLink(Factory& f, vector<Argument> args)
{
    return false;
}

std::array<LineParser, 4> parsers = {
    LineParser("LOADING_RAMP", 2, ParseRamp),
    LineParser("WORKER", 3, ParseRamp),
    LineParser("STOREHOUSE", 1, ParseStorehouse),
    LineParser("LINK", 2, ParseLink),
};

vector<Argument> GenerateArgs(vector<std::string> parts, int startIndex = 1)
{
    vector<Argument> args;
    for(int i = startIndex;i<parts.size();i++)
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

bool ParseLine(Factory& f, std::string line, int lineNumber)
{
    if(line.size() == 0 || line[0] == ';' || line[0] == '\r' || line[0] == ' ')
        return true;

    auto data = splitString(line, ' ');
    std::string tag = data[0];
    for(int i = 0;i<parsers.size();i++)
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
        if(!ParseLine(f, s, i))
            std::cout << "Error parsing line at " << i + 1 << std::endl;

    return f;
}

void save_factory_structure(Factory& f, std::ostream& os)
{
    throw exception();
}