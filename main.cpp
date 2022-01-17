// 5B: Tomecki (408146), Sztefko (407388), Walawski (406822)
#include <iostream>
#include "test.hpp"
#include "nodes.hpp"
#include "package.hpp"
#include "helpers.hpp"
#include "factory.hpp"
#include "simulation.hpp"
#include "reports.hpp"
#include <set>
#include <fstream>

using namespace std;

void test()
{  
    Worker w(1, 2, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    Time t = 1;

    cout << "1\n";

    w.receive_package(Package(1));
    cout << "1.5\n";
    w.do_work(t);

    cout << "2\n";
    ++t;
    w.receive_package(Package(2));
    w.do_work(t);

    cout << "3\n";
    auto& buffer = w.get_sending_buffer();

    cout << "T" << endl;

    cout << &(buffer) << endl;
}

void test_split()
{
    std::string t = "Ala ma kota";
    auto ret = splitString(t, ' ');
    for(int i = 0;i<(int)ret.size();i++)
        cout << i << ") " << ret[i] << endl;
}

Factory test_io(bool output = true)
{
    ifstream f("struct-input.txt");
    if(!f.good())
    {
        cout << "Cannot open test struct file" << endl;
        return Factory();
    }
    Factory factory = load_factory_structure(f);

    ofstream fout("struct-gen.txt");
    if(!fout.good())
    {
        cout << "Cannot open output struct file for writing" << endl;
        return Factory();
    }
    //save_factory_structure(factory, fout);
    //fout.close();
    if(output)
        save_factory_structure(factory, cout);

    return factory;
}

void test_sim()
{
    auto factory = test_io(false);
    SpecificTurnsReportNotifier spec_notifier(std::set<Time>{1});
    simulate(factory, 10, [&spec_notifier](Factory& f, TimeOffset t_offset) {
        if (spec_notifier.should_generate_report(t_offset)) {
            generate_structure_report(f, std::cout);
        }
    });
    
}

int main() {
    //utest_all();
    //test_io();
    test_sim();

    int dummy;
    std::cin >> dummy;
    return 0;
}