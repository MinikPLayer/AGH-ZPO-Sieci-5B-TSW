// 5B: Tomecki (408146), Sztefko (407388), Walawski (406822)
#include <iostream>
#include "test.hpp"
#include "nodes.hpp"
#include "package.hpp"

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
}

int main() {
    //utest_all();
    test();

    int dummy;
    std::cin >> dummy;
    return 0;
}