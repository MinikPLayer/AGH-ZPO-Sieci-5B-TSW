// 5B: Tomecki (408146), Sztefko (407388), Walawski (406822)
#include "test.hpp"

#include "storage_types.hpp"
#include <iostream>

using namespace std;

void utest_all()
{
    cout << "[Test start]" << endl;

    PackageQueue queue(PackageQueueType::LIFO);

    queue.push(Package(1));
    queue.push(Package(2));

    auto ret1 = queue.pop();
    auto ret2 = queue.pop();

    cout << "Ret: " << ret1.get_id() << ", " << ret2.get_id() << endl;
}