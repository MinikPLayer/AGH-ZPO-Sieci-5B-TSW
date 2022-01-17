// 5B: Tomecki (408146), Sztefko (407388), Walawski (406822)
#include "simulation.hpp"
#include "factory.hpp"
#include <exception>
#include <stdexcept>

void simulate(Factory& f, TimeOffset d, std::function<void (Factory&, Time)>)
{
    #if !TEST_INTERNAL_GR
    if(!f.is_consistent())
        throw std::logic_error("Factory is not consistent");
    #endif

    for(int i = 1; i <= d; i++)
    {
        f.do_deliveries(i);
        f.do_package_passing();
        f.do_work(i);
    }
}